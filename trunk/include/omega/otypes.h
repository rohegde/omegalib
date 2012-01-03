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
#ifndef __OMEGA_TYPES_H__
#define __OMEGA_TYPES_H__

#include "osysdefs.h"

#include <omicron.h>

// Forward declarations of some OpenGL and OpenCL types.
// Using these, we can avoid including gl, cl (and windows) headers for every compiled source file.
// Source files that make explicit use of OpenGL/or OpenCL functionality have to include glheaders.h
struct _cl_program;
struct _cl_kernel;
struct _cl_context;
struct _cl_mem;
struct _cl_command_queue;
struct _cl_device_id;
typedef _cl_device_id* cl_device_id;
typedef _cl_program* cl_program;
typedef _cl_kernel* cl_kernel;
typedef _cl_mem* cl_mem;
typedef _cl_context* cl_context;
typedef _cl_command_queue* cl_command_queue;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

namespace omega
{
	using namespace omicron;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class Application;
}; // namespace omega
#endif
