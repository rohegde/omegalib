/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/ObserverUpdateServiceExt.h"
#include "omega/ViewRayService.h"
#include "omega/WandEmulationService.h"
#include "omega/PythonInterpreter.h"
#include "omega/MissionControl.h"

#ifdef OMEGA_USE_DISPLAY_EQUALIZER
	#include "omega/EqualizerDisplaySystem.h"
#endif
#ifdef OMEGA_USE_DISPLAY_GLUT
	#include "omega/GlutDisplaySystem.h"
#endif

// Input services
#include "omega/KeyboardService.h"
#include "omega/MouseService.h"
#include "omega/SageManager.h"
#include "omega/ModuleServices.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager* SystemManager::mysInstance = NULL;

Setting sNullSetting(NULL);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SystemManager::settingExists(const String& name)
{
	if(mysInstance != NULL)
	{
		Config* appCfg = mysInstance->getAppConfig();
		Config* sysCfg = mysInstance->getSystemConfig();
		if(appCfg->exists(name)) return true;
		if(sysCfg->exists(name)) return true;
		return false;
	}
	owarn("SystemManager::settingExists: cannot search for settings before System Manager initialization");
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Setting& SystemManager::settingLookup(const String& name)
{
	if(mysInstance != NULL)
	{
		Config* appCfg = mysInstance->getAppConfig();
		Config* sysCfg = mysInstance->getSystemConfig();
		if(appCfg->exists(name)) return appCfg->lookup(name);
		if(sysCfg->exists(name)) return sysCfg->lookup(name);;
		oferror("FATAL SystemManager::settingLookup: could not find setting", %name);
	}
	oerror("FATAL SystemManager::settingLookup: cannot search for settings before System Manager initialization");
	return sNullSetting;
}

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
	myStatsManager(NULL),
	myExitRequested(false),
	myIsInitialized(false),
	myIsMaster(true),
	mySageManager(NULL)
{
	myDataManager = DataManager::getInstance();
	myInterpreter = new PythonInterpreter();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager::~SystemManager()
{
	myAppConfig = NULL;
	mySystemConfig = NULL;

	if(myServiceManager != NULL) delete myServiceManager;
	if(myDisplaySystem != NULL) delete myDisplaySystem;
	if(myStatsManager != NULL) delete myStatsManager;
	
	myDisplaySystem = NULL;
	myInterpreter = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupRemote(Config* cfg, const String& hostname)
{
	myIsMaster = false;
	myHostname = hostname;
	setup(cfg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setup(Config* appcfg)
{
	omsg("SystemManager::setup");

	setupConfig(appcfg);
	
#ifdef OMEGA_USE_SAGE
	// DEMO HACK (SageManager should start on all nodes, but launch createSAIL only on nodes that do have
	// output tiles to SAGE)
	if(isMaster())
	{
		mySageManager = new SageManager();
		ModuleServices::addModule(mySageManager);
	}
#endif

	try
	{
		setupServiceManager();
		setupDisplaySystem();
		if(myInterpreter->isEnabled())
		{
			if(mySystemConfig->exists("config"))
			{
				const Setting& sConfig = mySystemConfig->lookup("config");
				myInterpreter->setup(sConfig);
			}
		}
	}
	catch(libconfig::SettingTypeException ste)
	{
		oferror("Wrong setting type at %1% (HINT: make floats have a decimal part in your configuration)", %ste.getPath());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupConfig(Config* appcfg)
{
	if(!appcfg->isLoaded()) appcfg->load();
	oassert(appcfg->isLoaded());

	// If app config has a systemConfig entry, load the system configuration file specified there.
	myAppConfig = appcfg;
	if(appcfg->exists("config/systemConfig"))
	{
		String systemCfgName = appcfg->lookup("config/systemConfig");
		mySystemConfig = new Config(systemCfgName);
	}
	else
	{
		// LOGIC CHANGE: 24Jul2012
		// App config has no systemConfig entry. 
		// Open default.cfg and lookup systemConfig there.
		Config* defaultCfg = new Config("default.cfg");
		if(defaultCfg->load())
		{
			String systemCfgName = defaultCfg->lookup("config/systemConfig");
			mySystemConfig = new Config(systemCfgName);
		}
		else
		{
			oerror("SystemManager::setup: FATAL - coult not load default.cfg");
		}
	}

	// Make sure the configuration is loaded.
	if(!mySystemConfig->isLoaded()) mySystemConfig->load();

	oassert(mySystemConfig->isLoaded());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupServiceManager()
{
	myServiceManager = new ServiceManager();

	myServiceManager->registerService("MouseService", (ServiceAllocator)MouseService::New);
	myServiceManager->registerService("KeyboardService", (ServiceAllocator)KeyboardService::New);
	myServiceManager->registerService("ObserverUpdateServiceExt", (ServiceAllocator)ObserverUpdateServiceExt::New);
	myServiceManager->registerService("ViewRayService", (ServiceAllocator)ViewRayService::New);
	myServiceManager->registerService("WandEmulationService", (ServiceAllocator)WandEmulationService::New);

	// Kinda hack: run application initialize here because for now it is used to register services from
	// external libraries, so it needs to run before setting up services from the config file.
	// Initialize the application object (if present)
	if(myApplication) myApplication->initialize();

	// NOTE setup services only on master node.
	if(isMaster())
	{
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
		
		ofmsg("SystemManager::setupDisplaySystem: type = %1%", %displaySystemType);
		
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

	myInterpreter->initialize("omegalib");

	myStatsManager = new StatsManager();
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
	if(myServiceManager != NULL)
	{
		myServiceManager->stop();
		myServiceManager->dispose();
	}

	if(myInterpreter != NULL) 
	{
		delete myInterpreter;
		myInterpreter = NULL;
	}
	

	if(myDisplaySystem) 
	{
		myDisplaySystem->cleanup();
		delete myDisplaySystem;
		myDisplaySystem = NULL;
	}

	delete mysInstance;
	mysInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::postExitRequest(const String& reason)
{
	myExitRequested = true;
	myExitReason = reason;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::loadAppConfig(const String& filename)
{
	String cfgPath;
	if(DataManager::findFile(filename, cfgPath))
	{
		// Load and set the new app config.
		Config* cfg = new Config(filename);
		cfg->load();
		myAppConfig = cfg;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String SystemManager::getHostname() 
{ 
	Vector<String> args = StringUtils::split(myHostname, ":");
	return args[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const String& SystemManager::getHostnameAndPort() 
{ 
	return myHostname;
}
