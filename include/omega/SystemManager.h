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
	class ApplicationBase;
	class DisplaySystem;
	class PythonInterpreter;
	class MissionControlServer;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// The SystemManager class is the root object of the omegalib architecture.
	class OMEGA_API SystemManager
	{
	public:
		//! Utility method: searches for setting in application and system configuration file,
		// returns true if it exists
		static bool settingExists(const String& name);
		//! Utility method: searches for setting in application and system configuration file,
		// returns a reference to it if it exists.
		static Setting& settingLookup(const String& name);

	public:
		// Get the singleton instance of the system manager.
		static SystemManager* instance();

		//! Initializes the system manager
		void setup(Config* cfg);
		void setupRemote(Config* cfg, const String& masterHostname);

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

		//! Gets the ApplicationBase object
		ApplicationBase* getApplication();

		//! Sets the ApplicationBase object
		void setApplication(ApplicationBase* value);

		bool isExitRequested();

		const String& getExitReason();

		bool isInitialized();
		
		bool isMaster() { return myIsMaster; }
		const String& getHostname() { return myHostname; }

		PythonInterpreter* getScriptInterpreter() { return myInterpreter; }

		MissionControlServer* getMissionControlServer() { return myMissionControlServer; }

	private:
		SystemManager();
		~SystemManager();

		void setupServiceManager();
		void setupDisplaySystem();

	private:
		// Singleton instance.
		static SystemManager* mysInstance;

		bool myIsInitialized;

		// NOTE; We can't use Ref<> for a few types because they are forward-declared.
		Config*			myAppConfig;
		Config*			mySystemConfig;
		Ref<DataManager>    myDataManager;
		DisplaySystem*	myDisplaySystem;
		ServiceManager*	myServiceManager;
		ApplicationBase*	myApplication;
		bool			myExitRequested;
		String			myExitReason;
		PythonInterpreter* myInterpreter;
		
		bool myIsMaster;
		String myHostname;
		String myProgramName;

		// Mission control server
		bool myMissionControlEnabled;
		int myMissionControlPort;
		MissionControlServer* myMissionControlServer;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline DataManager* SystemManager::getDataManager()
	{ return myDataManager.get(); }

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
	inline ApplicationBase* SystemManager::getApplication() 
	{ return myApplication; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SystemManager::setApplication(ApplicationBase* value) 
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