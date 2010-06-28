/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * This file contains system / platform defines and is included in every other file.
 * osystem also implements some low-level functionality used by the rest of the lib, like logging.
 *********************************************************************************************************************/
#ifndef __OSYSTEM_H__
#define __OSYSTEM_H__

#ifdef WIN32
// disable warnings generated by libraries
#pragma warning( disable : 4224 4244 4251 4018 )
#endif

// Standard C includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Stdlib includes
#include <string>

// Equalizer includes
#include <eq/eq.h>

// Platform-specific includes.
#ifdef WIN32
#include <windows.h>
#include <objbase.h>
#include <atlbase.h>

#ifdef OMEGA_EXPORTING
   #define OMEGA_API    __declspec(dllexport)
#else
   #define OMEGA_API    __declspec(dllimport)
#endif

#endif

// OpenGL includes
#include <GL/gl.h>
#include <GL/glut.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The current omegalib version string.
#define OM_VERSION "0.1"

#define OM_STRLEN 256 /* Standard string length */

// Creates an integer identifier out of a 4 character string.
#define OID(s) (unsigned int)(s[0] | s[1] << 8 | s[2] << 16 | s[3] << 24)

namespace omega
{
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
typedef vmml::vector< 2, float > Vector2f; //!< A four-component float vector
typedef vmml::vector< 3, float > Vector3f; //!< A four-component float vector
typedef vmml::vector< 4, float > Vector4f; //!< A four-component float vector
}; // namespace omega

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function definitions.
OMEGA_API void ologopen(const char* filename);
OMEGA_API void ologclose();
OMEGA_API void omsg(const char* fmt, ...);
OMEGA_API void owarn(const char* fmt, ...);
OMEGA_API void oerror(const char* fmt, ...);
OMEGA_API void oexit(int code);

#define OMEGA_LOG_INIT_FILE(file) { ologopen(file); eq::base::Log::setOutput(std::ostream(new EqualizerLogStreamBuf())); }
#define OMEGA_LOG_CLOSE() { ologclose(); }

#define odbg(str) omsg(str);
#define oassert(c) if(!(c)) { oerror("Assertion failed at %s:%d - %s", __FILE__, __LINE__, ##c); exit(1); }

#endif