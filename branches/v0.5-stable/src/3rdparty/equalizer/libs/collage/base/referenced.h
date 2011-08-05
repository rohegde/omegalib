
/* Copyright (c) 2005-2009, Stefan Eilemann <eile@equalizergraphics.com> 
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

#ifndef COBASE_REFERENCED_H
#define COBASE_REFERENCED_H

#include <co/base/os.h>     // for COBASE_API
#include <co/base/debug.h>    // for EQERROR
#include <co/base/atomic.h>   // member
#ifdef EQ_REFERENCED_DEBUG
#  include <co/base/hash.h>
#  include <co/base/lock.h>
#  include <co/base/lockable.h>
#  include <co/base/scopedMutex.h>
#endif

#include <typeinfo>

namespace co
{
namespace base
{
    /**
     * Base class for referenced objects.
     * 
     * Implements reference-counted objects which destroy themselves once they
     * are no longer referenced. Uses an Atomic variable to keep the reference
     * count access thread-safe and efficient.
     *
     * @sa RefPtr
     */
    class Referenced 
    {
    public:
        /** Increase the reference count. @version 1.0 .*/
        void ref( EQ_REFERENCED_ARGS ) const
        {
#ifndef NDEBUG
            EQASSERTINFO( !_hasBeenDeleted, className( this ));
#endif
            ++_refCount;

#ifdef EQ_REFERENCED_DEBUG
            std::stringstream cs;
            cs << backtrace;
            ScopedMutex<> referencedMutex( _holders );
            HolderHash::iterator i = _holders->find( holder );
            EQASSERTINFO( i == _holders->end(), i->second );
            _holders.data[ holder ] = cs.str();
#endif
        }

        /** 
         * Decrease the reference count.
         *
         * The object is deleted when the reference count reaches 0.
         * @version 1.0
         */
        void unref( EQ_REFERENCED_ARGS ) const
            { 
#ifndef NDEBUG
                EQASSERT( !_hasBeenDeleted );
#endif
                EQASSERT( _refCount > 0 ); 
                const bool deleteMe = (--_refCount==0);
                if( deleteMe )
                    deleteReferenced( this );
#ifdef EQ_REFERENCED_DEBUG
                else
                {
                    ScopedMutex<> referencedMutex( _holders );
                    HolderHash::iterator i = _holders->find( holder );
                    EQASSERT( i != _holders->end( ));
                    _holders->erase( i );
                }
#endif
            }

        /** @return the current reference count. @version 1.0 */
        int  getRefCount() const { return _refCount; }

#ifdef EQ_REFERENCED_DEBUG
        void printHolders( std::ostream& os ) const
            {
                os << disableFlush << disableHeader;
                ScopedMutex<> referencedMutex( _holders );
                for( HolderHash::const_iterator i = _holders->begin();
                     i != _holders->end(); ++i )
                {
                    os << "Holder " << i->first << ": " << i->second 
                       << std::endl;
                }
                os << enableHeader << enableFlush;
            }
#endif

    protected:
        /** Construct a new reference-counted object. @version 1.0 */
        Referenced()
            : _refCount( 0 )
            , _hasBeenDeleted( false )
            {}

        /** Construct a new copy of a reference-counted object. @version 1.0 */
        Referenced( const Referenced& ) 
            : _refCount( 0 )
            , _hasBeenDeleted( false )
            {}

        /** Destruct a reference-counted object. @version 1.0 */
        virtual ~Referenced() 
            {
#ifndef NDEBUG
                EQASSERT( !_hasBeenDeleted );
                _hasBeenDeleted = true;
#endif
                EQASSERTINFO( _refCount == 0,
                              "Deleting object with ref count " << _refCount );
            }

        COBASE_API void deleteReferenced( const Referenced* object ) const;

    private:
        mutable a_int32_t _refCount;
        bool _hasBeenDeleted;

#ifdef EQ_REFERENCED_DEBUG
        typedef PtrHash< const void*, std::string > HolderHash;
        mutable Lockable< HolderHash, Lock > _holders;
#endif
    };
}

}
#endif //COBASE_REFERENCED_H
