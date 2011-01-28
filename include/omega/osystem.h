/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
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
	// 4396 the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
	// 4996 _strdup warning (still shows even after the #define strdup _strdup for some reason)
	#pragma warning( disable : 4224 4244 4251 4018 4800 4396 4996)
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#define strdup _strdup
#endif

// Standard C includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

// Equalizer includes
#include <eq/eq.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WIN32 Platform-specific includes & macros.
#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <objbase.h>
	#include <atlbase.h>

	// Omega DLL import / export macros
	#ifndef OMEGA_STATIC
		#ifdef OMEGA_EXPORTING
		   #define OMEGA_API    __declspec(dllexport)
		#else
		   #define OMEGA_API    __declspec(dllimport)
		#endif
		// Omega DLL import / export macros
		#ifdef OUTILS_EXPORTING
		   #define OUTILS_API    __declspec(dllexport)
		#else
		   #define OUTILS_API    __declspec(dllimport)
		#endif
	#else
		#define OMEGA_API
		#define OUTILS_API
	#endif
#else
	#define OMEGA_API
	#define OUTILS_API
#endif

#include "otypes.h"

// Enable GLEW multiple render context support.
#define GLEW_MX 

// OpenGL includes
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

// Boost boost_foreach support
#include "boost/foreach.hpp"
// Let's use a nicer lowercase sytax for this macro.
#define boost_foreach BOOST_FOREACH

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The current omegalib version string.
#define OMEGA_VERSION "0.3"

// Creates an integer identifier out of a 4 character string.
#define OID(s) (unsigned int)(s[0] | s[1] << 8 | s[2] << 16 | s[3] << 24)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function definitions.
OMEGA_API GLEWContext* glewGetContext();
OMEGA_API void glewSetContext(const GLEWContext* context);

OMEGA_API void ologopen(const char* filename);
OMEGA_API void ologclose();
OMEGA_API void omsg(const char* fmt, ...);
OMEGA_API void owarn(const char* fmt, ...);
OMEGA_API void oerror(const char* fmt, ...);
OMEGA_API void oexit(int code);
OMEGA_API void omain(omega::Application& app, const char* configFile, const char* logFile);

// @todo: stupid macros bleah go away.
#define OMEGA_LOG_INIT_FILE(file) { ologopen(file); eq::base::Log::setOutput(std::ostream(new EqualizerLogStreamBuf())); }
#define OMEGA_LOG_CLOSE() { ologclose(); }

#define odbg(str) omsg(str);
#define oassert(c) if(!(c)) { oerror("Assertion failed at %s:%d - %s", __FILE__, __LINE__, #c); exit(1); }

#endif