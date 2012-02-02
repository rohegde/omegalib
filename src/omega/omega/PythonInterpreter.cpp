/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/PythonInterpreter.h"
#include "omega/SystemManager.h"

using namespace omega;

#ifdef OMEGA_USE_PYTHON

#include <signal.h>  // for signal
#include "omega/PythonInterpreterWrapper.h"

//#if defined(OMEGA_TOOL_VS10) || defined(OMEGA_TOOL_VS9)
//#define VTK_LIBRARY_DIR_POSTFIX "/Release"
//#else
//#define VTK_LIBRARY_DIR_POSTFIX 
//#endif

//struct vtkPythonMessage
//{
//  vtkStdString Message;
//  bool IsError;
//};

///////////////////////////////////////////////////////////////////////////////////////////////////
class PythonInteractiveThread: public Thread
{
public:
	virtual void threadProc()
	{
		while(true)
		{
			char buf[65535];
			char* line = gets(buf);
			strcat(line, "\n");
			PyRun_SimpleString(line);
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
bool PythonInterpreter::isEnabled()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::PythonInterpreter()
{
	myInteractiveThread = new PythonInteractiveThread();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::~PythonInterpreter()
{
	delete myInteractiveThread;
	myInteractiveThread = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addPythonPath(const char* dir)
{
  // Convert slashes for this platform.
  String out_dir = dir ? dir : "";
#ifdef OMEGA_OS_WIN
  out_dir = StringUtils::replaceAll(out_dir, "/", "\\");
#endif

  // Append the path to the python sys.path object.
  PyObject* opath = PySys_GetObject(const_cast<char*>("path"));
  PyObject* newpath = PyString_FromString(out_dir.c_str());
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

	// The following code will hack in the path for running VTK/Python
	// from the build tree. Do not try this at home. We are
	// professionals.

	// Compute the directory containing this executable.  The python
	// sys.executable variable contains the full path to the interpreter
	// executable.
	//if (!myExecutablePath)
	//{
	//	PyObject* executable = PySys_GetObject(const_cast<char*>("executable"));
	//	char* exe_str = PyString_AsString(executable);
	//	if (exe_str)
	//	{
	//		// Use the executable location to try to set sys.path to include
	//		// the VTK python modules.
	//		vtkstd::string self_dir;
	//		vtkstd::string self_name;
	//		omega::StringUtils::splitFilename(exe_str, self_name, self_dir);
	//		addPythonPath(self_dir.c_str());
	//		addPythonPath(VTK_LIBRARY_DIR VTK_LIBRARY_DIR_POSTFIX);
	//		addPythonPath(VTK_PYTHON_DIR);
	//	}
	//}

	// HACK: Calling PyRun_SimpleString for the first time for some reason results in
	// a "\n" message being generated which is causing the error dialog to
	// popup. So we flush that message out of the system before setting up the
	// callbacks.
	// The cast is necessary because PyRun_SimpleString() hasn't always been
	// const-correct.
	PyRun_SimpleString(const_cast<char*>(""));
	PythonInterpreterWrapper* wrapperOut = vtkWrapInterpretor(this);
	wrapperOut->DumpToError = false;

	PythonInterpreterWrapper* wrapperErr = vtkWrapInterpretor(this);
	wrapperErr->DumpToError = false;

	// Redirect Python's stdout and stderr and stdin
	PySys_SetObject(const_cast<char*>("stdout"), reinterpret_cast<PyObject*>(wrapperOut));
	//PySys_SetObject(const_cast<char*>("stderr"), reinterpret_cast<PyObject*>(wrapperErr));
	//PySys_SetObject(const_cast<char*>("stdin"), reinterpret_cast<PyObject*>(wrapperErr));

	Py_DECREF(wrapperOut);
	Py_DECREF(wrapperErr);

	myInteractiveThread->start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addModule(const char* name, PyMethodDef* methods)
{
	Py_InitModule(name, methods);
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

#else

///////////////////////////////////////////////////////////////////////////////////////////////////
bool PythonInterpreter::isEnabled() { return false; }

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::PythonInterpreter() { }

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::~PythonInterpreter() { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addPythonPath(const char* dir) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::initialize(const char* programName) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addModule(const char* name, PyMethodDef* methods) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::eval(const String& script, const char* format, ...) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::runFile(const String& filename) { }

#endif