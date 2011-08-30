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
 *-------------------------------------------------------------------------------------------------
 * Original code Copyright (c) Kitware, Inc.
 * All rights reserved.
 * See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
 *************************************************************************************************/
#ifndef __PythonInterpreterWrapper_h
#define __PythonInterpreterWrapper_h

#include "omega/script/PythonInterpreter.h"

/* Undefine macros that Python.h defines to avoid redefinition warning.  */
#undef _POSIX_C_SOURCE
#undef _POSIX_THREADS

/* 
   Use the real python debugging library if it is provided.  
   Otherwise use the "documented" trick involving checking for _DEBUG
   and undefined that symbol while we include Python headers.
   Update: this method does not fool Microsoft Visual C++ 8 anymore; two
   of its header files (crtdefs.h and use_ansi.h) check if _DEBUG was set
   or not, and set flags accordingly (_CRT_MANIFEST_RETAIL, 
   _CRT_MANIFEST_DEBUG, _CRT_MANIFEST_INCONSISTENT). The next time the
   check is performed in the same compilation unit, and the flags are found,
   and error is triggered. Let's prevent that by setting _CRT_NOFORCE_MANIFEST.
*/
# ifdef _DEBUG
// Include these low level headers before undefing _DEBUG. Otherwise when doing
// a debug build against a release build of python the compiler will end up 
// including these low level headers without DEBUG enabled, causing it to try 
// and link release versions of this low level C api.
# include <basetsd.h>
# include <assert.h>
# include <ctype.h>
# include <errno.h>
# include <io.h>
# include <math.h>
# include <sal.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <time.h>
# include <wchar.h>
#  undef _DEBUG
#  if defined(_MSC_VER) && _MSC_VER >= 1400
#    define _CRT_NOFORCE_MANIFEST 1
#  endif
#  include <Python.h>
#  define _DEBUG
# else
#  include <Python.h>
# endif

#include "structmember.h"

namespace omega { namespace script {
	///////////////////////////////////////////////////////////////////////////////////////////////
	struct PythonInterpreterWrapper
	{
		PyObject_HEAD
		int softspace;  // Used by print to keep track of its state.
		PythonInterpreter* Interpretor;
		bool DumpToError;

		void Write(const char* string)
		{
			omsg(string);
		}

		String Read()
		{
			return "";
		}
	};

	static PyObject* wrapperWrite(PyObject* self, PyObject* args);
	static PyObject* wrapperRead(PyObject* self, PyObject* args);

	// const_cast since older versions of python are not const correct.
	static PyMethodDef PythonInterpreterWrapperMethods[] = {
		{const_cast<char*>("write"), wrapperWrite, METH_VARARGS, const_cast<char*>("Dump message")},
		{const_cast<char*>("readline"), wrapperRead, METH_VARARGS, const_cast<char*>("Read input line")},
		{0, 0, 0, 0}
	};

	static PyObject* PythonInterpreterWrapperNew(
	  PyTypeObject* type, PyObject* /*args*/, PyObject* /*kwds*/)
	{
	  return type->tp_alloc(type, 0);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	static PyMemberDef PythonInterpreterWrapperMembers[] = {
	  { const_cast<char*>("softspace"),
		T_INT, offsetof(PythonInterpreterWrapper, softspace), 0,
		const_cast<char *>("Placeholder so print can keep state.") },
	  { 0, 0, 0, 0, 0 }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	static PyTypeObject PythonInterpreterWrapperType = {
		PyObject_HEAD_INIT(NULL)
		0,                         // ob_size
		const_cast<char*>("PythonInterpreterWrapper"),   // tp_name
		sizeof(PythonInterpreterWrapper), // tp_basicsize
		0,                         // tp_itemsize
		0,                         // tp_dealloc
		0,                         // tp_print
		0,                         // tp_getattr
		0,                         // tp_setattr
		0,                         // tp_compare
		0,                         // tp_repr
		0,                         // tp_as_number
		0,                         // tp_as_sequence
		0,                         // tp_as_mapping
		0,                         // tp_hash 
		0,                         // tp_call
		0,                         // tp_str
		PyObject_GenericGetAttr,   // tp_getattro
		PyObject_GenericSetAttr,   // tp_setattro
		0,                         // tp_as_buffer
		Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
		const_cast<char*>("PythonInterpreterWrapper"),   //  tp_doc 
		0,                         //  tp_traverse 
		0,                         //  tp_clear 
		0,                         //  tp_richcompare 
		0,                         //  tp_weaklistoffset 
		0,                         //  tp_iter 
		0,                         //  tp_iternext 
		PythonInterpreterWrapperMethods, //  tp_methods 
		PythonInterpreterWrapperMembers, //  tp_members 
		0,                         //  tp_getset 
		0,                         //  tp_base 
		0,                         //  tp_dict 
		0,                         //  tp_descr_get 
		0,                         //  tp_descr_set 
		0,                         //  tp_dictoffset 
		0,                         //  tp_init 
		0,                         //  tp_alloc 
		PythonInterpreterWrapperNew,  //  tp_new
		0, // freefunc tp_free; /* Low-level free-memory routine */
		0, // inquiry tp_is_gc; /* For PyObject_IS_GC */
		0, // PyObject *tp_bases;
		0, // PyObject *tp_mro; /* method resolution order */
		0, // PyObject *tp_cache;
		0, // PyObject *tp_subclasses;
		0, // PyObject *tp_weaklist;
	#if PYTHON_API_VERSION >= 1012
		0  // tp_del
	#endif
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	static PyObject* wrapperWrite(PyObject* self, PyObject* args)
	{
	  if(!self || !PyObject_TypeCheck(self, &PythonInterpreterWrapperType))
		{
		return 0;
		}

	  PythonInterpreterWrapper* wrapper = 
		reinterpret_cast<PythonInterpreterWrapper*>(self);
  
	  char *string;
	  // const_cast since older versions of python are not const correct.
	  if (wrapper && PyArg_ParseTuple(args, const_cast<char*>("s"), &string))
	  {
		wrapper->Write(string);
      }

	  return Py_BuildValue(const_cast<char*>(""));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	static PyObject* wrapperRead(PyObject* self, PyObject* args)
	{
	  (void)args;
	  if(!self || !PyObject_TypeCheck(self, &PythonInterpreterWrapperType))
		{
		return 0;
		}

	  PythonInterpreterWrapper* wrapper = 
		reinterpret_cast<PythonInterpreterWrapper*>(self);

	  String ret;
	  if (wrapper)
		{
		ret = wrapper->Read();
		}
	  return Py_BuildValue(const_cast<char*>("s"), const_cast<char*>(ret.c_str()));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	static PythonInterpreterWrapper* wrapInterpretor(PythonInterpreter* interpretor)
	{
		if(PyType_Ready(&PythonInterpreterWrapperType) < 0)
		{
			return 0;
		}

		PythonInterpreterWrapper* wrapper = 
		PyObject_New(PythonInterpreterWrapper, &PythonInterpreterWrapperType);
		if (wrapper)
		{
			wrapper->Interpretor = interpretor;
		}

		return wrapper;
		}
}; };
#endif

