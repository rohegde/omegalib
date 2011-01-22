/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __OTYPES_H__
#define __OTYPES_H__

typedef unsigned char byte;
typedef unsigned int uint;

// Stdlib includes
#include <string>

// vmmlib includes
#define VMMLIB_DONT_FORCE_ALIGNMENT
#include <vmmlib/vmmlib.hpp>

// Libconfig
#include "libconfig/libconfig.hh"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class Application;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! wrapper to enable array use where arrays would not be allowed otherwise
template< class T, size_t d >
struct ArrayWrapper
{
    T& operator[]( const size_t i )
    {
        assert( i < d );
        return data[i];
    }
        
    const T& operator[]( const size_t i ) const
    {
        assert( i < d );
        return data[i];
    }
        
private:
    T data[d];
};
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type definitions.
typedef vmml::matrix< 3, 3, double > Matrix3d; //!< A 3x3 double matrix
typedef vmml::matrix< 4, 4, double > Matrix4d; //!< A 4x4 double matrix
typedef vmml::matrix< 3, 3, float >  Matrix3f; //!< A 3x3 float matrix
typedef vmml::matrix< 4, 4, float >  Matrix4f; //!< A 4x4 float matrix
typedef vmml::vector< 2, int > Vector2i; //!< A two-component integer vector
typedef vmml::vector< 3, int > Vector3i; //!< A three-component integer vector
typedef vmml::vector< 4, int > Vector4i; //!< A four-component integer vector
typedef vmml::vector< 3, double >Vector3d; //!< A three-component double vector
typedef vmml::vector< 4, double >Vector4d; //!< A four-component double vector
typedef vmml::vector< 2, float > Vector2f; //!< A two-component float vector
typedef vmml::vector< 3, float > Vector3f; //!< A three-component float vector
typedef vmml::vector< 4, float > Vector4f; //!< A four-component float vector

typedef vmml::quaternion<float> Quaternion;

typedef vmml::vector< 4, float >    BoundingSphere;
typedef vmml::vector< 3, uint >      Triangle;

//! enumeration for the axes
enum Axis
{
    AxisX,
    AxisY,
    AxisZ
};

// String
typedef std::string String;

// Setting
typedef libconfig::Setting Setting;
typedef co::base::uint128_t uint128_t;

}; // namespace omega

#endif