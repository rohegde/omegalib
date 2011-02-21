
/* Copyright (c) 2010, Stefan Eilemann <eile@eyescale.ch>
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

#ifndef COBASE_ERRORREGISTRY_H
#define COBASE_ERRORREGISTRY_H

#include <co/base/os.h>
#include <co/base/stdExt.h> // hash_map

namespace co 
{
namespace base
{
    /**
     * A registry translating error codes to strings.
     *
     * Applications can register custom error strings starting at
     * eq::ERROR_CUSTOM. Error registration and erasure is not
     * thread-safe. Equalizer registers errors only during eq::init(). It is
     * strongly advised to register application-specific errors before
     * eq::init() and erase them after eq::exit().
     *
     * @sa co::base::Error, eq::Error
     */
    class ErrorRegistry
    {
    public:
        /** @return the error string for the given error code. @version 1.0 */
        COBASE_API const std::string& getString( const uint32_t error ) const;

        /** Set an error string for the given error code. @version 1.0 */
        COBASE_API void setString(const uint32_t error, const std::string& text);

        /** Clear a given error code string. @version 1.0 */
        COBASE_API void eraseString( const uint32_t error );

        bool isEmpty() const { return _errors.empty(); } //!< @internal

    private:
        typedef stde::hash_map< uint32_t, std::string > ErrorHash;
        ErrorHash _errors;
    };
}
}
#endif // COBASE_ERRORREGISTRY_H
