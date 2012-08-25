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
#ifndef __PYTHON_INTERPRETER_H__
#define __PYTHON_INTERPRETER_H__

#include "omega/osystem.h"
#include "omega/ApplicationBase.h"
#include "omega/IRendererCommand.h"
#include "omega/SharedDataServices.h"

struct PyMethodDef;
class PythonInteractiveThread;

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//struct CommandHelpEntry
	//{
	//	String syntax;
	//	String info;
	//};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API PythonInterpreter: public SharedObject
	{
		friend struct PythonInterpreterWrapper;
	public:
		enum CallbackType
		{
			CallbackUpdate, CallbackEvent
		};

		//! @internal returns the last event received by the interpreter. Used for script interoperability
		static const Event* getLastEvent() { return mysLastEvent; }

	public:
		PythonInterpreter();
		virtual ~PythonInterpreter();

		void setup(const Setting& setting);
		void initialize(const char* programName);
		void addModule(const char* name, PyMethodDef* methods);
		void addModule(const char* name, PyMethodDef* methods, const Dictionary<String, int> intConstants, const Dictionary<String, String> stringConstants);
		void eval(const String& script, const char* format = NULL, ...);
		void runFile(const String& filename);
		void queueInteractiveCommand(const String& command);

		void registerCallback(void* callback, CallbackType type);
		void unregisterAllCallbacks();

		void addPythonPath(const char*);

		bool isEnabled();
		bool isShellEnabled() { return myShellEnabled; }

		// invoke python callbacks.
		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		// Shared data
		virtual void commitSharedData(SharedOStream& out);
		virtual void updateSharedData(SharedIStream& in);

		//String getHelpString(const String& filter);

	protected:
		bool myEnabled;
		bool myShellEnabled;
		bool myDebugShell;
		PythonInteractiveThread* myInteractiveThread;

		Lock myInteractiveCommandLock;
		String myInteractiveCommand;
		bool myInteractiveCommandNeedsExecute;
		bool myInteractiveCommandNeedsSend;

		List<void*> myUpdateCallbacks;
		List<void*> myEventCallbacks;
		//char* myExecutablePath;

		//List<CommandHelpEntry*> myHelpData;

	private:
		static const Event* mysLastEvent;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements a renderer command that runs a python statement when executed.
	class ScriptRendererCommand: public IRendererCommand
	{
	public:
		ScriptRendererCommand()
		{
			myInterp = SystemManager::instance()->getScriptInterpreter();
			myStatement = "";
		}

		void setStatement(const String& value) { myStatement = value; }

		void execute(Renderer* r)
		{
			myInterp->eval(myStatement);
		}

	private:
		String myStatement;
		PythonInterpreter* myInterp;
	};
};
#endif

