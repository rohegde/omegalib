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
#include "omega/ServiceManager.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"
#include "omega/Config.h"
#include "omega/StringUtils.h"

#include "omega/HeartbeatService.h"

// Display system
#ifdef OMEGA_USE_DISPLAY
	#include "omega/DisplaySystem.h"
	#include "omega/ObserverUpdateService.h"
	#include "omega/SagePointerService.h"
#endif

#ifdef OMEGA_USE_DISPLAY_EQUALIZER
	#include "omega/EqualizerDisplaySystem.h"
#endif
#ifdef OMEGA_USE_DISPLAY_GLUT
	#include "omega/GlutDisplaySystem.h"
#endif

// Input services
#ifdef OMEGA_USE_DIRECTINPUT
	#include "omega/DirectXInputService.h"
#endif
#ifdef OMEGA_USE_NATURAL_POINT
	#include "omega/NaturalPointService.h"
#endif
#ifdef OMEGA_USE_KEYBOARD
	#include "omega/KeyboardService.h"
#endif
#ifdef OMEGA_USE_MOUSE
	#include "omega/MouseService.h"
#endif
#ifdef OMEGA_USE_NETSERVICE
	#include "omega/NetService.h"
#endif
#ifdef OMEGA_USE_PQLABS
	#include "omega/PQService.h"
#endif
#ifdef OMEGA_USE_OPTITRACK
	#include "omega/OptiTrackService.h"
#endif
#ifdef OMEGA_USE_OPENNI
	#include "omega/OpenNIService.h"
#endif
#ifdef OMEGA_USE_VRPN
	#include "omega/VRPNService.h"
#endif
#ifdef OMEGA_USE_THINKGEAR
	#include "omega/ThinkGearService.h"
#endif

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
#ifdef OMEGA_USE_DISPLAY
	myDisplaySystem(NULL),
#endif
	myApplication(NULL),
	myExitRequested(false),
	myIsInitialized(false)
{
	myDataManager = new DataManager();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager::~SystemManager()
{
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
#ifdef OMEGA_USE_DISPLAY
		setupDisplaySystem();
#endif
	}
	catch(libconfig::SettingTypeException ste)
	{
		oferror("Wrong setting type at %1% (HINT: make floats have a decimal part in your configuration)", %ste.getPath());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupServiceManager()
{
	myServiceManager = new ServiceManager(this);

	// register standard input services.
	myServiceManager->registerService("HeartbeatService", (ServiceAllocator)HeartbeatService::New);

#ifdef OMEGA_USE_DIRECTINPUT
	myServiceManager->registerService("DirectXInputService", (ServiceAllocator)DirectXInputService::New);
#endif
#ifdef OMEGA_USE_MOUSE
	myServiceManager->registerService("MouseService", (ServiceAllocator)MouseService::New);
#endif
#ifdef OMEGA_USE_KEYBOARD
	myServiceManager->registerService("KeyboardService", (ServiceAllocator)KeyboardService::New);
#endif
#ifdef OMEGA_USE_NATURAL_POINT
	myServiceManager->registerService("NaturalPointService", (ServiceAllocator)NaturalPointService::New);
#endif
#ifdef OMEGA_USE_NETSERVICE
	myServiceManager->registerService("NetService", (ServiceAllocator)NetService::New);
#endif
#ifdef OMEGA_USE_PQLABS
	myServiceManager->registerService("PQService", (ServiceAllocator)PQService::New);
#endif
#ifdef OMEGA_USE_VRPN
	myServiceManager->registerService("VRPNService", (ServiceAllocator)VRPNService::New);
#endif
#ifdef OMEGA_USE_VRPN
	myServiceManager->registerService("ThinkGearService", (ServiceAllocator)ThinkGearService::New);
#endif
#ifdef OMEGA_USE_OPTITRACK
	myServiceManager->registerService("OptiTrackService", (ServiceAllocator)OptiTrackService::New);
#endif
#ifdef OMEGA_USE_OPENNI
	myServiceManager->registerService("OpenNIService", (ServiceAllocator)OpenNIService::New);
#endif

#ifdef OMEGA_USE_DISPLAY
	myServiceManager->registerService("ObserverUpdateService", (ServiceAllocator)ObserverUpdateService::New);
	myServiceManager->registerService("SagePointerService", (ServiceAllocator)SagePointerService::New);
#endif

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
#ifdef OMEGA_USE_DISPLAY
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
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::initialize()
{
#ifdef OMEGA_USE_DISPLAY
	if(myDisplaySystem) myDisplaySystem->initialize(this);
#endif
	myServiceManager->initialize();

#ifdef OMEGA_USE_DISPLAY
	// Initialize the application object (if present)
	if(myApplication) myApplication->initialize();
#endif

	myIsInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::run()
{
	// If the system manager has not been initialized yet, do it now.
	if(!myIsInitialized) initialize();

	myServiceManager->start();
#ifdef OMEGA_USE_DISPLAY
	if(myDisplaySystem)
	{
		myDisplaySystem->run();
	}
	else
	{
		owarn("SystemManager::run - no display system specified, returning immediately");
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::cleanup()
{
#ifdef OMEGA_USE_DISPLAY
	if(myDisplaySystem) myDisplaySystem->cleanup();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::postExitRequest(const String& reason)
{
	myExitRequested = true;
	myExitReason = reason;
}

