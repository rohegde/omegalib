/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __OSYSTEM_H__
#define __OSYSTEM_H__

#include "osysdefs.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// The current omegalib version string.
#define OMEGA_VERSION "0.6.30"

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WIN32 Platform-specific includes & macros.
#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	// Omega DLL import / export macros
	#ifndef OMEGA_STATIC
		#ifdef OMEGA_EXPORTING
		   #define OMEGA_API    __declspec(dllexport)
		#else
		   #define OMEGA_API    __declspec(dllimport)
		#endif
	#else
		#define OMEGA_API
	#endif
#else
	#define OMEGA_API
#endif

#include "otypes.h"

// Include the omega type class, used for RTTI
#include <omega/TypeInfo.h>

// Creates an integer identifier out of a 4 character string.
#define OID(s) (unsigned int)(s[0] | s[1] << 8 | s[2] << 16 | s[3] << 24)

struct GLEWContextStruct;
typedef struct GLEWContextStruct GLEWContext;

// Forward declaration of DataSource, used for omain
namespace omega 
{ 
	class DataSource;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Function definitions.
	OMEGA_API GLEWContext* glewGetContext();
	OMEGA_API void glewSetContext(const GLEWContext* context);

	OMEGA_API void ologopen(const char* filename);
	OMEGA_API void ologclose();
	OMEGA_API void omsg(const String& str);
	OMEGA_API void owarn(const String& str);
	OMEGA_API void oerror(const String& str);
	OMEGA_API void oexit(int code);
	OMEGA_API void oabort(const char* file, int line, const char* reason);
	OMEGA_API void omain(omega::Application& app, const char* configFile, const char* logFile, omega::DataSource* dataSource = NULL);
};

#define odbg(str) omsg(str);
#define oassert(c) if(!(c)) { oabort(__FILE__, __LINE__, #c); }

#define onew(type) new type
#define odelete(var) if(var != NULL) { delete var; var = NULL; }

#endif