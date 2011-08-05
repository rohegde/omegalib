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
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class Application;
	class Config;
	class DisplaySystem;
	class ServiceManager;
	class Service;
	class DataManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// The SystemManager class is the root object of the omegalib architecture.
	class OMEGA_API SystemManager
	{
	public:
		// Get the singleton instance of the system manager.
		static SystemManager* instance();

		//! Initializes the system manager
		void setup(Config* cfg);

		void initialize();

		//! Starts running the current application.
		//! This method does not return until the application is done running.
		void	run();

		//! Sends an exit request to the system manager.
		void postExitRequest(const String& reason = "Undefined reason");

		//! Cleans up runtime resources, performs final debug checks etc.
		void cleanup();

		//! Gets the application configuration.
		Config* getAppConfig();

		//! Gets the system configuration.
		Config* getSystemConfig();

		DataManager* getDataManager();

		//! Gets the ServiceManager object
		ServiceManager* getServiceManager();

		//! Gets the DisplaySystem object
		DisplaySystem* getDisplaySystem();
		//! Sets the DisplaySystem object
		void setDisplaySystem(DisplaySystem* value);

		//! Gets the Application object
		Application* getApplication();

		//! Sets the Application object
		void setApplication(Application* value);

		bool isExitRequested();

		const String& getExitReason();

		bool isInitialized();

	private:
		SystemManager();
		~SystemManager();

		void setupServiceManager();
		void setupDisplaySystem();

	private:
		// Singleton instance.
		static SystemManager* mysInstance;

		bool myIsInitialized;

		Config*			myAppConfig;
		Config*			mySystemConfig;
		DataManager*    myDataManager;
		DisplaySystem*	myDisplaySystem;
		ServiceManager*	myServiceManager;
		Application*	myApplication;
		bool			myExitRequested;
		String			myExitReason;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline DataManager* SystemManager::getDataManager()
	{ return myDataManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ServiceManager* SystemManager::getServiceManager() 
	{ return myServiceManager; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline DisplaySystem* SystemManager::getDisplaySystem() 
	{ return myDisplaySystem; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SystemManager::setDisplaySystem(DisplaySystem* value) 
	{ myDisplaySystem = value; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Application* SystemManager::getApplication() 
	{ return myApplication; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SystemManager::setApplication(Application* value) 
	{ myApplication = value; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SystemManager::isExitRequested() 
	{ return myExitRequested; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const String& SystemManager::getExitReason() 
	{ return myExitReason; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SystemManager::isInitialized() 
	{ return myIsInitialized; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Config* SystemManager::getAppConfig() 
	{ return myAppConfig; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Config* SystemManager::getSystemConfig() 
	{ return mySystemConfig; }
}; // namespace omega

#endif