/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/PythonInterpreter.h"
#include "omega/SystemManager.h"
#include "omega/ModuleServices.h"

using namespace omega;

const Event* PythonInterpreter::mysLastEvent = NULL;

#ifdef OMEGA_USE_PYTHON

#ifdef OMEGA_READLINE_FOUND
	#include<readline/readline.h>
	#include<readline/history.h>
#endif
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
		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();

		while(!SystemManager::instance()->isExitRequested())	
		{
			osleep(100);
			String line;
#ifdef OMEGA_READLINE_FOUND
			String prompt = ostr("%1%>>", %SystemManager::instance()->getApplication()->getName());
			char *inp_c = readline(prompt.c_str()); //Instead of getline()
			
			// THE COMMAND OF DEATH
			if(inp_c[0] == 'D' &&
				inp_c[1] == 'I' &&
				inp_c[2] == 'E') exit(-1);
			
			line = (const char *)(inp_c); //Because C strings stink
			if(strlen(inp_c) != 0)
			{
				add_history(inp_c);
			}
			free(inp_c);
#else
			getline(std::cin, line);
#endif
			
			//ofmsg("line read: %1%", %line);

			interp->queueCommand(line);
		}
		omsg("Ending console interactive thread");
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
	omsg("~PythonInterpreter");
	//myInteractiveThread->stop();
	delete myInteractiveThread;
	myInteractiveThread = NULL;

	Py_Finalize();
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
	// Register self as shared object
	SharedDataServices::registerObject(this, "interp");

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

	addPythonPath("./");

	if(myShellEnabled && SystemManager::instance()->isMaster())
	{
		omsg("PythonInterpreter: starting interactive shell thread.");
		myInteractiveThread->start();
	}

	// Initialize internal Apis
	omegaPythonApiInit();

	PyRun_SimpleString("from omega import *");
	
	omsg("Python Interpreter initialized.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addModule(const char* name, PyMethodDef* methods)
{
	Dictionary<String, int> intConstants;
	Dictionary<String, String> stringConstants;
	addModule(name, methods, intConstants, stringConstants);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::addModule(const char* name, PyMethodDef* methods, const Dictionary<String, int> intConstants, const Dictionary<String, String> stringConstants)
{
	PyObject* module = Py_InitModule(name, methods);

	typedef Dictionary<String, int>::Item IntConstantItem;
	foreach(IntConstantItem item, intConstants)
	{
		PyModule_AddIntConstant(module, item.getKey().c_str(), item.getValue());
	}

	typedef Dictionary<String, String>::Item StringConstantItem;
	foreach(StringConstantItem item, stringConstants)
	{
		PyModule_AddStringConstant(module, item.getKey().c_str(), item.getValue().c_str());
	}

	PyMethodDef* cur = methods;
	//while(cur->ml_name != NULL)
	//{
	//	CommandHelpEntry* help = new CommandHelpEntry();
	//	String syntax = StringUtils::split(cur->ml_doc, "\n")[0];
	//	help->syntax = syntax;
	//	help->info = cur->ml_doc;

	//	cur++;
	//	myHelpData.push_back(help);
	//}

#ifdef OMEGA_READLINE_FOUND
	cur = methods;
	while(cur->ml_name != NULL)
	{
		add_history(cur->ml_name);
		cur++;
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::eval(const String& script, const char* format, ...)
{
	char* str = const_cast<char*>(script.c_str());
	if(format == NULL)
	{
		// Handle special 'shortcut' commands. Commands starting with a ':' won't be interpreted
		// using the python interpreter. Instead, we will dispatch them to EngineModule::handleCommand 
		// methods.
		if(script.length() > 0 && script[0] == ':')
		{
			ModuleServices::handleCommand(script.substr(1, script.length() - 1));
		}
		else
		{
			PyRun_SimpleString(str);
		}
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::registerCallback(void* callback, CallbackType type)
{
	// BLAGH cast
	PyObject* pyCallback =(PyObject*)callback;
	if(callback != NULL)
	{
		Py_INCREF(callback);
		switch(type)
		{
		case CallbackUpdate:
			myUpdateCallbacks.push_back(callback);
			return;
		case CallbackEvent:
			myEventCallbacks.push_back(callback);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::unregisterAllCallbacks()
{
	myUpdateCallbacks.clear();
	myEventCallbacks.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::update(const UpdateContext& context) 
{
	// Execute queued interactive commands first
	if(myCommandQueue.size() != 0)
	{
		// List of commands to be removed from queue
		List<QueuedCommand*> cmdsToRemove;
		foreach(QueuedCommand* qc, myCommandQueue)
		{
			if(qc->needsExecute) 
			{
				if(myDebugShell)
				{
					ofmsg("running %1%", %qc->command);
				}
				eval(qc->command);
				qc->needsExecute = false;
			}
			// Purge commands from list
			if(!qc->needsExecute && !qc->needsSend) cmdsToRemove.push_back(qc);
		}
		foreach(QueuedCommand* qc, cmdsToRemove)
		{
			myCommandQueue.remove(qc);
			delete qc;
		}
	}
	
	PyObject *arglist;
	arglist = Py_BuildValue("(lff)", (long int)context.frameNum, context.time, context.dt);

	foreach(void* cb, myUpdateCallbacks)
	{
		// BLAGH cast
		PyObject* pyCallback =(PyObject*)cb;
		PyObject_CallObject(pyCallback, arglist);

	}

	Py_DECREF(arglist);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::queueCommand(const String& command, bool local)
{
	//oassert(!myInteractiveCommandNeedsExecute && 
	//	!myInteractiveCommandNeedsSend);
	
	myInteractiveCommandLock.lock();
	myCommandQueue.push_back(new QueuedCommand(command, true, !local));
	myInteractiveCommandLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::commitSharedData(SharedOStream& out)
{
	// Count number of commands that need sending
	int i = 0;
	foreach(const QueuedCommand* qc, myCommandQueue) if(qc->needsSend) i++;

	// Send commands
	out << i;
	foreach(QueuedCommand* qc, myCommandQueue) 
	{
		if(qc->needsSend)
		{
			out << qc->command;
			qc->needsSend = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::updateSharedData(SharedIStream& in)
{
	int cmdCount;
	
	in >> cmdCount;
	for(int i = 0; i < cmdCount; i++)
	{
		String cmd;
		in >> cmd;
		// Add command to the local command queue.
		queueCommand(cmd, true);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::handleEvent(const Event& evt) 
{
	// Save the received event to the static variable mysLastEvent.
	// Script code will be able to retrieve it using getEvent()
	mysLastEvent = &evt;

	foreach(void* cb, myEventCallbacks)
	{
		// BLAGH cast
		PyObject* pyCallback =(PyObject*)cb;
		PyObject_CallObject(pyCallback, NULL);
	}

	// We can't guarantee the event will live outside of this call tree, so clean up the static
	// variable. getEvent() will return None when called outside the event callback.
	mysLastEvent = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//String PythonInterpreter::getHelpString(const String& filter)
//{
//	String result = "";
//	foreach(CommandHelpEntry* item, myHelpData)
//	{
//		if(filter == "" || StringUtils::startsWith(item->syntax, filter))
//		{
//			result.append(item->syntax);
//			result.append("|");
//			result.append(item->info);
//			result.append("|");
//		}
//	}
//	return result;
//}


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

void PythonInterpreter::addModule(const char* name, PyMethodDef* methods, const Dictionary<String, int> intConstants, const Dictionary<String, String> stringConstants) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::eval(const String& script, const char* format, ...) 
{ 
	ofwarn("PythonInterpreter::eval: Python interpreter not available on this system. (%1%)", %script);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::runFile(const String& filename) 
{ 
	ofwarn("PythonInterpreter::runFile: Python interpreter not available on this system. (%1%)", %filename);
}

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
void PythonInterpreter::queueCommand(const String& command, bool local) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PythonInterpreter::unregisterAllCallbacks() {}

#endif
