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
#include "omega/SystemManager.h"
#include "omega/StringUtils.h"
#include "omega/DataManager.h"

#include "omega/script/PythonInterpreter.h"
#include "omega/script/PythonModule.h"

#include "PythonInterpreterWrapper.h"

using namespace omega;
using namespace omega::script;

OMEGA_DEFINE_TYPE(PythonInterpreter, ScriptInterpreter);

//struct vtkPythonMessage
//{
//  vtkStdString Message;
//  bool IsError;
//};

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::PythonInterpreter():
	myExecutablePath(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::~PythonInterpreter()
{
  myExecutablePath = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addPath(const char* dir)
{
  // Append the path to the python sys.path object.
  PyObject* opath = PySys_GetObject(const_cast<char*>("path"));
  PyObject* newpath = PyString_FromString(dir);
  PyList_Insert(opath, 0, newpath);
  Py_DECREF(newpath);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::initialize(const char* programName)
{
	// Set the program name, so that we can ask python to provide us
	// full path.
	Py_SetProgramName((char*)programName);

	// initialize the statically linked modules
	//CMakeLoadAllPythonModules();

	// Initialize interpreter.
	Py_Initialize();

	// HACK: Calling PyRun_SimpleString for the first time for some reason results in
	// a "\n" message being generated which is causing the error dialog to
	// popup. So we flush that message out of the system before setting up the
	// callbacks.
	// The cast is necessary because PyRun_SimpleString() hasn't always been
	// const-correct.
	PyRun_SimpleString(const_cast<char*>(""));
	PythonInterpreterWrapper* wrapperOut = wrapInterpretor(this);
	wrapperOut->DumpToError = false;

	PythonInterpreterWrapper* wrapperErr = wrapInterpretor(this);
	wrapperErr->DumpToError = true;

	// Redirect Python's stdout and stderr and stdin
	PySys_SetObject(const_cast<char*>("stdout"), reinterpret_cast<PyObject*>(wrapperOut));
	PySys_SetObject(const_cast<char*>("stderr"), reinterpret_cast<PyObject*>(wrapperErr));
	PySys_SetObject(const_cast<char*>("stdin"), reinterpret_cast<PyObject*>(wrapperErr));

	Py_DECREF(wrapperOut);
	Py_DECREF(wrapperErr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addModule(const char* name, ScriptModule* module)
{
	PythonModule* pm = (PythonModule*)module;
	Py_InitModule(name, pm->getDefs());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::eval(const String& script, const char* format, ...)
{
	char* str = const_cast<char*>(script.c_str());
	if(format == NULL)
	{
		PyRun_SimpleString(str);
	}
	else
	{
		PyObject * module = PyImport_AddModule("__main__");
		PyObject* dict = PyModule_GetDict(module);
		PyObject* result = PyRun_String(str, Py_eval_input, dict, dict);
	
		if(format != NULL && result != NULL)
		{
			va_list args;
			va_start(args, format);

			char* fmt = const_cast<char*>(format);
			if(!PyArg_Parse(result, format, va_arg(args, void*)))
			{
				ofwarn("PythonInterpreter: result of statement '%1%' cannot be parsed by format string '%2%'", %str %fmt);
			}

			va_end(args);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::runFile(const String& filename)
{
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo cfgInfo = dm->getInfo(filename);

	if(!cfgInfo.isNull())
	{
		PyObject* PyFileObject = PyFile_FromString((char*)cfgInfo.path.c_str(), "r");
		PyRun_SimpleFile(PyFile_AsFile(PyFileObject), filename.c_str());
	}
	else
	{
		ofwarn("PythonInterpreter: script not found: %1%", %filename);
	}
}
