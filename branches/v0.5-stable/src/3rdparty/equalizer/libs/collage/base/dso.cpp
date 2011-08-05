
/* Copyright (c) 2009, Cedric Stalder <cedric.stalder@gmail.com> 
 *               2009-2010, Stefan Eilemann <eile@equalizergraphics.com> 
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

#include "dso.h"

#include "debug.h"
#include "log.h"

#ifdef _WIN32 //_MSC_VER
#  define EQ_DL_ERROR sysError
#else
#  include <dlfcn.h>
#  define EQ_DL_ERROR dlerror()
#endif

namespace co
{
namespace base
{

bool DSO::open( const std::string& fileName )
{
    if( _dso )
    {
        EQWARN << "DSO already open, close it first" << std::endl;
        return false;
    }

    if( fileName.empty( ))
    {
#ifdef _WIN32 //_MSC_VER
#  ifndef NDEBUG
        _dso = GetModuleHandle( "Equalizer_d.dll" );
#  endif
        if( !_dso )
            _dso = GetModuleHandle( "Equalizer.dll" );
        EQASSERT( _dso );
#else
        _dso = RTLD_DEFAULT;
#endif
    }
    else
    {
#ifdef _WIN32 //_MSC_VER
        _dso = LoadLibrary( fileName.c_str() );
#elif defined( RTLD_LOCAL )
        _dso = dlopen( fileName.c_str(), RTLD_LAZY | RTLD_LOCAL );
#else
        _dso = dlopen( fileName.c_str(), RTLD_LAZY );
#endif
        if( !_dso )
        {
            EQWARN << fileName << "Can't open library: " << EQ_DL_ERROR << std::endl;
            return false;
        }
    }

    return true;
}

void DSO::close()
{
    if( !_dso )
        return;

#ifdef _WIN32 //_MSC_VER
    if( _dso != GetModuleHandle( 0 ))
        FreeLibrary( _dso ) ;
#else
    if( _dso != RTLD_DEFAULT )
        dlclose ( _dso );
#endif

    _dso = 0;
}

void* DSO::getFunctionPointer( const std::string& name )
{
#ifdef _WIN32 //_MSC_VER
    return (void*)GetProcAddress( _dso, name.c_str() );
#else
    return dlsym( _dso, name.c_str() );
#endif
}

}
}
