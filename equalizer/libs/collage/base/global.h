
/* Copyright (c) 2010, Cedric Stalder <cedric.stalder@gmail.com> 
 *               2010, Stefan Eilemann <eile@eyescale.ch>
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

#ifndef COBASE_GLOBAL_H
#define COBASE_GLOBAL_H

#include <co/base/os.h>
#include <co/base/lock.h> // member
#include <co/base/types.h>
#include <string>

namespace co
{
namespace base
{
    /** Global parameter handling for the Equalizer base library. */
    class Global
    {
    public:
        /** @return the plugin registry. @version 1.0 */
        COBASE_API static PluginRegistry& getPluginRegistry();

        /** @return the error registry. @version 1.0 */
        COBASE_API static ErrorRegistry& getErrorRegistry();

    private:
        friend COBASE_API bool init( const int argc, char** argv );
        friend COBASE_API bool exit();
    };
}
}

#endif // EQ_GLOBAL_H

