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

#include<iostream>

//struct vtkPythonMessage
//{
//  vtkStdString Message;
//  bool IsError;
//};

void omegaPythonApiInit();

//PyThreadState* sMainThreadState;

///////////////////////////////////////////////////////////////////////////////////////////////////
class PythonInteractiveThread: public Thread
{
public:
	virtual void threadProc()
	{
		//PyEval_AcquireLock();
		//PyInterpreterState* interpState = sMainThreadState->interp;
		//PyThreadState* threadState = PyThreadState_New(interpState);
		//PyEval_ReleaseLock();

		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();

		// Not sure this is helping with safe init but let's try.
		while(true)	
		{
			osleep(100);
			String line;
			getline(std::cin, line);
			
			//ofmsg("line read: %1%", %line);

			interp->queueInteractiveCommand(line);

			//PyEval_AcquireLock();
			//PyThreadState_Swap(threadState);

			//PyRun_SimpleString(buf);

			//PyThreadState_Swap(NULL);
			//PyEval_ReleaseLock();
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
	myShellEnabled = false;
	myInteractiveThread = new PythonInteractiveThread();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::~PythonInterpreter()
{
	//myInteractiveThread->stop();
	delete myInteractiveThread;
	myInteractiveThread = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addPythonPath(const char* dir)
{
	ofmsg("PythonInterpreter::addPythonPath: %1%", %dir);
	
	//PyEval_AcquireLock();
	//PyThreadState_Swap(sMainThreadState);

	// Convert slashes for this platform.
	String out_dir = dir ? dir : "";
#ifdef OMEGA_OS_WIN
	out_dir = StringUtils::replaceAll(out_dir, "/", "\\");
#endif

	// Append the path to the python sys.path object.
	PyObject* opath = PySys_GetObject("path");
	PyObject* newpath = PyString_FromString(out_dir.c_str());
	PyList_Insert(opath, 0, newpath);
	Py_DECREF(newpath);

	//PyThreadState_Swap(NULL);
	//PyEval_ReleaseLock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::setup(const Setting& setting)
{
	myShellEnabled = Config::getBoolValue("pythonShellEnabled", setting, false);
	myDebugShell = Config::getBoolValue("pythonShellDebug", setting, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::initialize(const char* programName)
{
	// Reset interactive command flags
	myInteractiveCommandNeedsExecute = false;
	myInteractiveCommandNeedsSend = false;

	// Register self as shared object
	SharedDataServices::registerObject(this, "interp");

	// Set the program name, so that we can ask python to provide us
	// full path.
	Py_SetProgramName((char*)programName);

	// initialize the statically linked modules
	//CMakeLoadAllPythonModules();

	// Initialize interpreter.
	Py_Initialize();

	//PyEval_InitThreads();
	//sMainThreadState = PyThreadState_Get();
	//PyEval_ReleaseLock();

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
	PySys_SetObject(const_cast<char*>("stderr"), reinterpret_cast<PyObject*>(wrapperErr));
	//PySys_SetObject(const_cast<char*>("stdin"), reinterpret_cast<PyObject*>(wrapperErr));

	Py_DECREF(wrapperOut);
	Py_DECREF(wrapperErr);

	if(myShellEnabled && SystemManager::instance()->isMaster())
	{
		omsg("PythonInterpreter: starting interactive shell thread.");
		myInteractiveThread->start();
	}

	// Initialize internal Apis
	omegaPythonApiInit();

	//PyEval_AcquireLock();
	//PyThreadState_Swap(sMainThreadState);

	// Not sure this is helping with safe init but let's try.
	//osleep(1000);
	PyRun_SimpleString("from omega import *");
	//PyThreadState_Swap(NULL);
	//PyEval_ReleaseLock();
	
	omsg("Python Interpreter initialized.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addModule(const char* name, PyMethodDef* methods)
{
	//PyEval_AcquireLock();
	//PyThreadState_Swap(sMainThreadState);

	Py_InitModule(name, methods);

	//PyThreadState_Swap(NULL);
	//PyEval_ReleaseLock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::eval(const String& script, const char* format, ...)
{
	//PyEval_AcquireLock();
	//PyThreadState_Swap(sMainThreadState);

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

	//PyThreadState_Swap(NULL);
	//PyEval_ReleaseLock();
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::registerCallback(void* callback, CallbackType type)
{
	// BLAGH
	PyObject* pyCallback =(PyObject*)callback;
	if(callback != NULL)
	{
		Py_INCREF(callback);
		switch(type)
		{
		case CallbackUpdate:
			myUpdateCallbacks.push_back(callback);
			return;
		case CallbackPointerEvent:
			myPointerEventCallbacks.push_back(callback);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::update(const UpdateContext& context) 
{
	//PyEval_AcquireLock();
	//PyThreadState_Swap(sMainThreadState);

	// Execute queued interactive commands first
	if(myInteractiveCommandNeedsExecute)
	{
		if(myDebugShell)
		{
			ofmsg("running %1%", %myInteractiveCommand);
		}
		PyRun_SimpleString(myInteractiveCommand.c_str());
		myInteractiveCommandNeedsExecute = false;
	}
	
	PyObject *arglist;
	arglist = Py_BuildValue("(iff)", context.frameNum, context.time, context.dt);


	foreach(void* cb, myUpdateCallbacks)
	{
		// BLAGH
		PyObject* pyCallback =(PyObject*)cb;
		PyObject_CallObject(pyCallback, arglist);

	}

	//PyThreadState_Swap(NULL);
	//PyEval_ReleaseLock();

	Py_DECREF(arglist);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::queueInteractiveCommand(const String& command)
{
	//oassert(!myInteractiveCommandNeedsExecute && 
	//	!myInteractiveCommandNeedsSend);
	
	//ofmsg("Queuing command %1%", %command);

	myInteractiveCommandLock.lock();
	myInteractiveCommandNeedsExecute = true;
	myInteractiveCommandNeedsSend = true;
	myInteractiveCommand = command;
	myInteractiveCommandLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::commitSharedData(SharedOStream& out)
{
	out << myInteractiveCommandNeedsSend;
	if(myInteractiveCommandNeedsSend) out << myInteractiveCommand;
	myInteractiveCommandNeedsSend = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::updateSharedData(SharedIStream& in)
{
	in >> myInteractiveCommandNeedsExecute;
	if(myInteractiveCommandNeedsExecute)
	{
		in >> myInteractiveCommand;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::handleEvent(const Event& evt) 
{
}

#else

///////////////////////////////////////////////////////////////////////////////////////////////////
bool PythonInterpreter::isEnabled() { return false; }

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::PythonInterpreter() 
{ 	
	myShellEnabled = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PythonInterpreter::~PythonInterpreter() { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::setup(const Setting& setting) { }

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

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::registerCallback(void* callback, CallbackType type) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::update(const UpdateContext& context) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::handleEvent(const Event& evt) { }

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::commitSharedData(SharedOStream& out) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::updateSharedData(SharedIStream& in) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::queueInteractiveCommand(const String& command) {}
#endif