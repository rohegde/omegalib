
/* Copyright (c) 2005-2010, Stefan Eilemann <eile@equalizergraphics.com> 
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "thread.h"

#include "os.h"
#include "debug.h"
#include "lock.h"
#include "log.h"
#include "rng.h"
#include "scopedMutex.h"
#include "executionListener.h"

#include <co/base/lock.h>

#include <errno.h>
#include <pthread.h>
#include <algorithm>

// Experimental Win32 thread pinning
#ifdef _WIN32
//#  define EQ_WIN32_THREAD_AFFINITY
#endif

namespace co
{
namespace base
{

struct ThreadIDPrivate
{
    pthread_t pthread;
};

namespace
{
static Lock& _listenerLock()
{
    static Lock lock;
    return lock;
}

typedef std::vector< ExecutionListener* >  ExecutionListenerVector;

static ExecutionListenerVector& _listeners()
{
    static ExecutionListenerVector listeners;
    return listeners;
}
}

static pthread_key_t _createCleanupKey();
void                 _notifyStopping( void* arg );

static pthread_key_t _cleanupKey = _createCleanupKey();

pthread_key_t _createCleanupKey()
{
    const int error = pthread_key_create( &_cleanupKey, _notifyStopping );
    if( error )
    {
        EQERROR
            << "Can't create thread-specific key for thread cleanup handler: " 
            << strerror( error ) << std::endl;
        EQASSERT( !error );
    }
    return _cleanupKey;
}

Thread::Thread()
        : _state( STATE_STOPPED )
{
}

Thread::~Thread()
{
}

void* Thread::runChild( void* arg )
{
    Thread* thread = static_cast<Thread*>(arg);
    thread->_runChild();
    return 0; // not reached
}

void Thread::_runChild()
{
    setDebugName( className( this ));
    pinCurrentThread();
    _id._data->pthread = pthread_self();

    if( !init( ))
    {
        EQWARN << "Thread " << className( this ) << " failed to initialize"
               << std::endl;
        _state = STATE_STOPPED;
        pthread_exit( 0 );
        EQUNREACHABLE;
    }

    _state = STATE_RUNNING;
    EQINFO << "Thread " << className( this ) << " successfully initialized"
           << std::endl;
    pthread_setspecific( _cleanupKey, this ); // install cleanup handler
    _notifyStarted();

    run();
    EQINFO << "Thread " << className( this ) << " finished" << std::endl;
    this->exit();

    EQUNREACHABLE;
}

void Thread::_notifyStarted()
{
    // make copy of vector so that listeners can add/remove listeners.
    _listenerLock().set();
    const ExecutionListenerVector listeners = _listeners();
    _listenerLock().unset();

    EQVERB << "Calling " << listeners.size() << " thread started listeners"
           << std::endl;
    for( ExecutionListenerVector::const_iterator i = listeners.begin();
         i != listeners.end(); ++i )
    {
        (*i)->notifyExecutionStarted();
    }
}

void _notifyStopping( void* )
{ 
    Thread::_notifyStopping();
}

void Thread::_notifyStopping()
{
    pthread_setspecific( _cleanupKey, 0 );

    // make copy of vector so that listeners can add/remove listeners.
    _listenerLock().set();
    ExecutionListenerVector listeners = _listeners();
    _listenerLock().unset();

    // Call them in reverse order so that symmetry is kept
    for( ExecutionListenerVector::reverse_iterator i = listeners.rbegin();
         i != listeners.rend(); ++i )
    {   
        (*i)->notifyExecutionStopping();
    }
}

bool Thread::start()
{
    if( _state != STATE_STOPPED )
        return false;

    _state = STATE_STARTING;

    pthread_attr_t attributes;
    pthread_attr_init( &attributes );
    pthread_attr_setscope( &attributes, PTHREAD_SCOPE_SYSTEM );

    int nTries = 10;
    while( nTries-- )
    {
        const int error = pthread_create( &_id._data->pthread, &attributes,
                                          runChild, this );

        if( error == 0 ) // succeeded
        {
            EQVERB << "Created pthread " << this << std::endl;
            break;
        }
        if( error != EAGAIN || nTries==0 )
        {
            EQWARN << "Could not create thread: " << strerror( error )
                   << std::endl;
            return false;
        }
    }

    _state.waitNE( STATE_STARTING );
    return (_state != STATE_STOPPED);
}

void Thread::exit()
{
    EQASSERTINFO( isCurrent(), "Thread::exit not called from child thread" );
    EQINFO << "Exiting thread " << base::className( this ) << std::endl;

    _state = STATE_STOPPING;
    pthread_exit( 0 );
    EQUNREACHABLE;
}

void Thread::cancel()
{
    EQASSERTINFO( !isCurrent(), "Thread::cancel called from child thread" );

    EQINFO << "Cancelling thread " << base::className( this ) << std::endl;
    _state = STATE_STOPPING;

    pthread_cancel( _id._data->pthread );
    EQUNREACHABLE;
}

bool Thread::join()
{
    if( _state == STATE_STOPPED )
        return false;
    if( isCurrent( )) // can't join self
        return false;

    _state.waitNE( STATE_RUNNING );
    _state = STATE_STOPPED;

    EQINFO << "Joined thread " << className( this ) << std::endl;
    return true;
}

bool Thread::isCurrent() const
{
    return pthread_equal( pthread_self(), _id._data->pthread );
}

ThreadID Thread::getSelfThreadID()
{
    ThreadID threadID;
    threadID._data->pthread = pthread_self();
    return threadID;
}

void Thread::addListener( ExecutionListener* listener )
{
    ScopedMutex<> mutex( _listenerLock() );
    _listeners().push_back( listener );
}

bool Thread::removeListener( ExecutionListener* listener )
{
    ScopedMutex<> mutex( _listenerLock() );

    ExecutionListenerVector::iterator i = find( _listeners().begin(),
                                                     _listeners().end(),
                                                     listener );
    if( i == _listeners().end( ))
        return false;

    _listeners().erase( i );
    return true;
}

void Thread::removeAllListeners()
{
    _listenerLock().set();

    EQINFO << _listeners().size() << " thread listeners active" << std::endl;
    for( ExecutionListenerVector::const_iterator i = _listeners().begin();
         i != _listeners().end(); ++i )
    {
        EQINFO << "    " << className( *i ) << std::endl;
    }
    _listenerLock().unset();
    
    _notifyStopping();

    _listenerLock().set();
    _listeners().clear();
    _listenerLock().unset();
}

void Thread::yield()
{
#ifdef _MSC_VER
    ::Sleep( 0 );
#elif defined (Darwin)
    ::pthread_yield_np();
#else
    ::sched_yield();
#endif
}

void Thread::pinCurrentThread()
{
#ifdef EQ_WIN32_THREAD_AFFINITY
    static Lock lock;
    ScopedMutex<> mutex( lock );

    static DWORD_PTR processMask = 0;
    static DWORD_PTR processor   = 0;
    if( processMask == 0 )
    {
        // Get available processors
        DWORD_PTR systemMask;
        if( GetProcessAffinityMask( GetCurrentProcess(), &processMask, 
            &systemMask ) == 0 )
        {
            EQWARN << "Can't get usable processor mask" << std::endl;
            return;
        }
        EQINFO << "Available processors 0x" << hex << processMask << dec <<endl;

        // Choose random starting processor: Multiple Eq apps on the same node
        // would otherwise use the same processor for the same thread
        unsigned nProcessors = 0;
        for( DWORD_PTR i = 1; i != 0; i <<= 1 )
        {
            if( processMask & i )
                ++nProcessors;
        }
        EQINFO << nProcessors << " available processors" << std::endl;

        unsigned chance = RNG().get< unsigned >();
        processor = 1 << (chance % nProcessors);
        EQINFO << "Starting with processor " << processor << std::endl;
    }
    EQASSERT( processMask != 0 );

    while( true )
    {
        processor <<= 1;
        if( processor == 0 ) // wrap around
            processor = 1;

        if( processor & processMask ) // processor is available
        {
            if( SetThreadAffinityMask( GetCurrentThread(), processor ) == 0 )
                EQWARN << "Can't set thread processor" << std::endl;
            EQINFO << "Pinned thread to processor 0x" << hex << processor << dec
                   << std::endl;
            return;
        }
    }
#endif
}

#ifdef _WIN32
#ifndef MS_VC_EXCEPTION
#  define MS_VC_EXCEPTION 0x406D1388
#endif

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

#endif

void Thread::setDebugName( const std::string& name )
{
    EQINFO << "Thread " << name << std::endl;
#ifdef _MSC_VER
#  ifndef NDEBUG
    ::Sleep(10);

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name.c_str();
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;

    __try
    {
        RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
#  endif
#elif __MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
    pthread_setname_np( name.c_str( ));
#else
    // Not implemented
    EQVERB << "Thread::setDebugName( " << name << " ) not implemented" 
           << std::endl;
#endif
}

std::ostream& operator << ( std::ostream& os, const Thread* thread )
{
    os << "Thread " << thread->_id << " state " 
       << ( thread->_state == Thread::STATE_STOPPED  ? "stopped"  :
            thread->_state == Thread::STATE_STARTING ? "starting" :
            thread->_state == Thread::STATE_RUNNING  ? "running"  :
            thread->_state == Thread::STATE_STOPPING ? "stopping" : "unknown" );

#ifdef PTW32_VERSION
    os << " called from " << pthread_self().p;
#else
    os << " called from " << pthread_self();
#endif

    return os;
}
}
}
