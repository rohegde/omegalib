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
#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__

#include "osystem.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class Application;
class Config;
class DisplaySystem;
class InputManager;
class InputService;
class InputFilter;
class DataManager;

typedef InputService* (*InputServiceAllocator)();
typedef Dictionary<String, InputServiceAllocator> InputServiceDictionary;

typedef InputFilter* (*InputFilterAllocator)();
typedef Dictionary<String, InputFilterAllocator> InputFilterDictionary;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The SystemManager class is the root object of the omegalib architecture.
class OMEGA_API SystemManager
{
public:
	// Get the singleton instance of the system manager.
	static SystemManager* instance();

	//! Initializes the system manager
	void setup(Config* cfg);

	void registerInputService(String svcName, InputServiceAllocator creationFunc);

	//! Find an input service allocator given the input service name.
	//! See registerInputService for additional information.
	InputServiceAllocator findInputService(String svcName);

	void initialize();

	//! Starts running the current application.
	//! This method does not return until the application is done running.
	void	run();

	//! Sends an exit request to the system manager.
	void postExitRequest(const String& reason = "Undefined reason");

	//! Cleans up runtime resources, performs final debug checks etc.
	void cleanup();

	//! Gets the system configuration.
	Config* getConfig() { return myConfig; }

	//! Gets the InputManager object
	InputManager* getInputManager() { return myInputManager; }

	//! Gets the DisplaySystem object
	DisplaySystem* getDisplaySystem() { return myDisplaySystem; }
	//! Sets the DisplaySystem object
	void setDisplaySystem(DisplaySystem* value) { myDisplaySystem = value; }

	//! Gets the Application object
	Application* getApplication() { return myApplication; }

	//! Sets the Application object
	void setApplication(Application* value) { myApplication = value; }

	bool isExitRequested() { return myExitRequested; }

	const String& getExitReason() { return myExitReason; }

	bool isInitialized() { return myIsInitialized; }

private:
	SystemManager();
	~SystemManager();

	void setupInputManager();
	void setupDisplaySystem();

private:
	// Singleton instance.
	static SystemManager* mysInstance;

	bool myIsInitialized;

	// The input manager registry.
	InputServiceDictionary myInputServiceRegistry;

	// The input service registry.
	InputFilterDictionary myInputFilterRegistry;

	Config*			myConfig;
	DisplaySystem*	myDisplaySystem;
	InputManager*	myInputManager;
	Application*	myApplication;
	bool			myExitRequested;
	String			myExitReason;
};

}; // namespace omega

#endif