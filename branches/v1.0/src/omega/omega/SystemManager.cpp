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

// Display system
#include "omega/DisplaySystem.h"
#include "omega/ObserverUpdateService.h"
#include "omega/SagePointerService.h"

#ifdef OMEGA_USE_DISPLAY_EQUALIZER
	#include "omega/EqualizerDisplaySystem.h"
#endif
#ifdef OMEGA_USE_DISPLAY_GLUT
	#include "omega/GlutDisplaySystem.h"
#endif

// Input services
#include "omega/KeyboardService.h"
#include "omega/MouseService.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager* SystemManager::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager* SystemManager::instance()
{
	if(!mysInstance) mysInstance = new SystemManager();
	return mysInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager::SystemManager():
	mySystemConfig(NULL),
	myDisplaySystem(NULL),
	myApplication(NULL),
	myExitRequested(false),
	myIsInitialized(false)
{
	myDataManager = DataManager::getInstance();
#ifdef OMEGA_USE_PYTHON
	myInterpreter = new PythonInterpreter();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager::~SystemManager()
{
	delete myInterpreter;
	myInterpreter = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setup(Config* appcfg)
{
	omsg("SystemManager::setup");

	if(!appcfg->isLoaded()) appcfg->load();
	oassert(appcfg->isLoaded());

	// If app config has a systemConfig entry, load a separate system configuration file, otherwise use
	// the app configuration file as system configuration.
	myAppConfig = appcfg;
	if(appcfg->exists("config/systemConfig"))
	{
		String systemCfgName = appcfg->lookup("config/systemConfig");
		mySystemConfig = new Config(systemCfgName);
	}
	else
	{
		mySystemConfig = appcfg;
	}

	// Make sure the configuration is loaded.
	if(!mySystemConfig->isLoaded()) mySystemConfig->load();

	oassert(mySystemConfig->isLoaded());

	try
	{
		setupServiceManager();
		setupDisplaySystem();
	}
	catch(libconfig::SettingTypeException ste)
	{
		oferror("Wrong setting type at %1% (HINT: make floats have a decimal part in your configuration)", %ste.getPath());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupServiceManager()
{
	myServiceManager = new ServiceManager();

	myServiceManager->registerService("MouseService", (ServiceAllocator)MouseService::New);
	myServiceManager->registerService("KeyboardService", (ServiceAllocator)KeyboardService::New);
	myServiceManager->registerService("ObserverUpdateService", (ServiceAllocator)ObserverUpdateService::New);
	myServiceManager->registerService("SagePointerService", (ServiceAllocator)SagePointerService::New);

	// Kinda hack: run application initialize here because for now it is used to register services from
	// external libraries, so it needs to run before setting up services from the config file.
	// Initialize the application object (if present)
	if(myApplication) myApplication->initialize();

	// Instantiate services (for compatibility reasons, look under'input' and 'services' sections
	Setting& stRoot = mySystemConfig->getRootSetting()["config"];
	if(stRoot.exists("input"))
	{
		myServiceManager->setup(stRoot["input"]);
	}
	else if(stRoot.exists("services"))
	{
		myServiceManager->setup(stRoot["services"]);
	}
	else
	{
		owarn("Config/InputServices section missing from config file: No services created.");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupDisplaySystem()
{
	// Instantiate input services
	Setting& stRoot = mySystemConfig->getRootSetting();
	if(stRoot.exists("config/display"))
	{
		Setting& stDS = stRoot["config/display"][0];
		DisplaySystem* ds = NULL;

		String displaySystemType;
		stDS.lookupValue("type", displaySystemType);
		if(displaySystemType == "Equalizer")
		{
#ifdef OMEGA_USE_DISPLAY_EQUALIZER
			ds = new EqualizerDisplaySystem();
#else
			oerror("Equalizer display system support disabled for this build!");
#endif
		}
		else if(displaySystemType == "Glut")
		{
#ifdef OMEGA_USE_DISPLAY_GLUT
			ds = new GlutDisplaySystem();
#else
			oerror("Glut display system support disabled for this build!");
#endif
		}
		else
		{
			oferror("invalid display system type: %s", %displaySystemType);
		}
		if(ds != NULL)
		{
			ds->setup(stDS);
			setDisplaySystem(ds);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::initialize()
{
	if(myDisplaySystem) myDisplaySystem->initialize(this);
	myServiceManager->initialize();

	myIsInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::run()
{
	// If the system manager has not been initialized yet, do it now.
	if(!myIsInitialized) initialize();

	myServiceManager->start();
	if(myDisplaySystem)
	{
		myDisplaySystem->run();
	}
	else
	{
		owarn("SystemManager::run - no display system specified, returning immediately");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::cleanup()
{
	if(myDisplaySystem) myDisplaySystem->cleanup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::postExitRequest(const String& reason)
{
	myExitRequested = true;
	myExitReason = reason;
}
