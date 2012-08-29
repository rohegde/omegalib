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
#ifndef __ASYNC_TASK__
#define __ASYNC_TASK__

#include "osystem.h"
#include "omega/SystemManager.h"
#include "omega/PythonInterpreter.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class AsyncTask: public ReferenceType
	{
	public:
		typedef T Data;

	public:
		AsyncTask(): myProgress(0), myComplete(false) {}

		T& getData() { return myData; }
		void setData(const T& data) { myData = data; }

		bool isComplete() { return myComplete; }
		int getProgress() { return myProgress; }
		void setProgress(int value) { myProgress = value; }

		void  notifyComplete()
		{
			myProgress = 100;
			myComplete = true;

			if(myCompletionCommand.size() > 0)
			{
				PythonInterpreter* pi = SystemManager::instance()->getScriptInterpreter();
				// Queue a script command for execution on the main thread.
				pi->queueCommand(myCompletionCommand, true);
			}
		}

		void setCompletionCommand(const String& cmd) { myCompletionCommand = cmd; }
		const String& getCompletionCommand() { return myCompletionCommand; }

	private:
		T myData;
		bool myComplete;
		int myProgress;
		String myCompletionCommand;
	};
}; // namespace omega

#endif
