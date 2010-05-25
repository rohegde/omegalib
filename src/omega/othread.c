/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * othread functions implementation. See othread.h for more details.
 *********************************************************************************************************************/
#include "othread.h"

#define DEFAULT_STACKSIZE 4096

/**********************************************************************************************************************
*/
OMThread othreadCreate(unsigned (OM_THREADPROC* thread_func)(void*), void* userData)
{
	OMThread result;
#ifdef WIN32
    // we are using _beginthreadex() instead of CreateThread(),
    // because we want to use c runtime functions from within the thread
    unsigned int thrdaddr;
    result = (HANDLE)_beginthreadex(
                   NULL,    // security
                   DEFAULT_STACKSIZE,
                   thread_func,
                   userData,    // arglist
                   0,       // init_flags
                   &thrdaddr);
#else
    // FIXME: ignore stack size under Linux
    int pok = pthread_create(&(result),
                             NULL,
                             (void *(*)(void *))thread_func,
                             userData);
    //n_assert(!((pok == EAGAIN) || (pok == EINVAL)))
#endif
	return result;
}

/**********************************************************************************************************************
*/
void othreadKill(OMThread thread)
{
#ifdef WIN32
    WaitForSingleObject(thread, 500);
    CloseHandle(thread);
    thread = 0;
#else
    pthread_join(thread, NULL);
    thread = 0;
#endif
}

/**********************************************************************************************************************
 */
void othreadMutexInit(OMMutex* m)
{
#ifdef WIN32
    InitializeCriticalSection( m );
#else
    const int error = pthread_mutex_init( m, 0 );
    if( error )
    {
		ologError("Error creating pthread mutex: %s", strerror(error));
        return;
    }
#endif
}

/**********************************************************************************************************************
 */
void othreadMutexDelete(OMMutex* m)
{
#ifdef WIN32
    DeleteCriticalSection( m ); 
#else
    pthread_mutex_destroy( m );
#endif
}

/**********************************************************************************************************************
 */
void othreadMutexLock(OMMutex* m)
{
#ifdef WIN32
    EnterCriticalSection( m );
#else
    pthread_mutex_lock( m );
#endif
}

/**********************************************************************************************************************
 */
void othreadMutexUnlock(OMMutex* m)
{
#ifdef WIN32
    LeaveCriticalSection( m );
#else
    pthread_mutex_unlock( m );
#endif
}

/**********************************************************************************************************************
 */
int othreadMutexTryLock(OMMutex* m)
{
#ifdef WIN32
    return TryEnterCriticalSection( m );
#else
    return ( pthread_mutex_trylock( m ) == 0 );
#endif
}
