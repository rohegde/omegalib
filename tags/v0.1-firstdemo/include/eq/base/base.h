 
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

/**
 * @file base/base.h
 *
 * Includes key system header files and defines essential base macros.
 */
#ifndef EQBASE_BASE_H
#define EQBASE_BASE_H

#include <eq/base/defines.h>

#ifdef WIN32
#  define _USE_MATH_DEFINES
#  define _WIN32_WINNT 0x500
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Winsock2.h>
#  include <Windows.h>
#  include <windef.h>
#  define EQ_DLLEXPORT __declspec(dllexport) 
#  define EQ_DLLIMPORT __declspec(dllimport)
#  ifdef EQUALIZER_EXPORTS
#    define EQ_EXPORT EQ_DLLEXPORT
#    define EQ_STLEXTERN 
#    define GLEW_BUILD
#  else
#    define EQ_EXPORT EQ_DLLIMPORT
#    define EQ_STLEXTERN extern
#  endif
   // Need to predefine server library exports for forward declaration of 
   // eqsStartLocalServer
#  ifdef EQUALIZERSERVERLIBRARY_EXPORTS
#    define EQSERVER_EXPORT EQ_DLLEXPORT
#    define EQSERVER_STLEXTERN 
#  else
#    define EQSERVER_EXPORT EQ_DLLIMPORT
#    define EQSERVER_STLEXTERN extern
#  endif
#else // WIN32
#  define EQ_DLLEXPORT //!< @internal
#  define EQ_DLLIMPORT //!< @internal
/** Mark the following function as exported in the Equalizer DSO. @internal */
#  define EQ_EXPORT
#  define EQSERVER_DLLEXPORT //!< @internal
#  define EQSERVER_DLLIMPORT //!< @internal
/** Mark the following function as exported in the server DSO. @internal */
#  define EQSERVER_EXPORT
#endif

// Defining our own min/max macros seems to be the only sane way to get this
// functionality across platforms thanks to some screwup in the MS header files.
#define EQ_MAX(a,b) ((a)>(b)?(a):(b)) //!< returns the maximum of two values
#define EQ_MIN(a,b) ((a)<(b)?(a):(b)) //!< returns the minimum of two values

#include <cmath>
#include <cstdio>
#include <cstdlib>
#ifndef WIN32_VC
#  include <stdint.h>
#  include <sys/param.h>  // for MIN/MAX
#endif

#include <eq/base/types.h>

// defines
/** A 'NULL' value for an uint32, typically used for identifiers an versions. */
#define EQ_UNDEFINED_UINT32   (0xffffffffu)
//#define EQ_UNDEFINED_FLOAT    (std::numeric_limits<float>::quiet_NaN( ))
//#define EQ_UNDEFINED_INT32    (0x7fffffffu)

/** Constant defining 'wait forever' in methods with wait parameters. */
#define EQ_TIMEOUT_INDEFINITE 0

#ifdef WIN32_VC
/** Declare and align a variable to a 8-byte boundary. */
#  define EQ_ALIGN8( var )  __declspec (align (8)) var;
/** Declare and align a variable to a 16-byte boundary. */
#  define EQ_ALIGN16( var ) __declspec (align (16)) var;
#else
/** Declare and align a variable to a 8-byte boundary. */
#  define EQ_ALIGN8( var )  var __attribute__ ((aligned (8)));
/** Declare and align a variable to a 16-byte boundary. */
#  define EQ_ALIGN16( var ) var __attribute__ ((aligned (16)));
#endif



#endif //EQBASE_BASE_H