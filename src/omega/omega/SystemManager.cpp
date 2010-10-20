/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * omega functions implementation. See omega.h for more details.
 *********************************************************************************************************************/
#include "omega/DisplaySystem.h"
#include "omega/InputManager.h"
#include "omega/SystemManager.h"
#include "omega/Config.h"
#include "omega/DisplaySystem.h"

// Input services
#ifdef OMEGA_USE_MOCAP
	#include "omega/input/MoCapService.h"
#endif
#ifdef OMEGA_USE_MOUSE
	#include "omega/input/MouseService.h"
#endif
#ifdef OMEGA_USE_NETSERVICE
#include "omega/input/NetService.h"
#endif
#ifdef OMEGA_USE_PQLABS
#include "omega/input/PQService.h"
#endif
#ifdef OMEGA_USE_OPTITRACK
#include "omega/input/OptiTrackService.h"
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
	myConfig(NULL),
	myDisplaySystem(NULL),
	myExitRequested(false),
	myIsInitialized(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager::~SystemManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setup(Config* cfg)
{
	omsg("SystemManager::setup");

	myConfig = cfg;

	// Make sure the configuration is loaded.
	if(!myConfig->isLoaded()) myConfig->load();

	oassert(myConfig->isLoaded());

	setupInputManager();
	setupDisplaySystem();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupInputManager()
{
	myInputManager = new InputManager(this);

	// register standard input services.
	// @todo: I don't understand why a static_cast does not work here.
#ifdef OMEGA_USE_MOUSE
	registerInputService("MouseService", (InputServiceAllocator)MouseService::New);
#endif
#ifdef OMEGA_USE_MOCAP
	registerInputService("MoCapService", (InputServiceAllocator)MoCapService::New);
#endif
#ifdef OMEGA_USE_NETSERVICE
	registerInputService("NetService", (InputServiceAllocator)NetService::New);
#endif
#ifdef OMEGA_USE_PQLABS
	registerInputService("PQService", (InputServiceAllocator)PQService::New);
#endif
#ifdef OMEGA_USE_OPTITRACK
	registerInputService("OptiTrackService", (InputServiceAllocator)OptiTrackService::New);
#endif

	// Instantiate input services
	Setting& stRoot = myConfig->getRootSetting()["Config"];
	if(stRoot.exists("InputServices"))
	{
		Setting& stServices = stRoot["InputServices"];
		for(int i = 0; i < stServices.getLength(); i++)
		{
			Setting& stSvc = stServices[i];
			InputServiceAllocator svcAllocator = findInputService(stSvc.getName());
			if(svcAllocator != NULL)
			{
				// Input service found: create and setup it.
				InputService* svc = svcAllocator();
				svc->setup(stSvc);
				myInputManager->addService(svc);

				omsg("Input service added: %s", stSvc.getName());
			}
			else
			{
				owarn("Input service not found: %s", stSvc.getName());
			}
		}
	}
	else
	{
		owarn("Config/InputServices section missing from config file: No input services created.");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setupDisplaySystem()
{
	// Instantiate input services
	Setting& stRoot = myConfig->getRootSetting();
	if(stRoot.exists("Config/DisplaySystem"))
	{
		Setting& stDS = stRoot["Config/DisplaySystem"][0];
		DisplaySystem* ds = new DisplaySystem();
		ds->setup(stDS);
		setDisplaySystem(ds);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::initialize()
{
	if(myDisplaySystem) myDisplaySystem->initialize(this);
	myInputManager->initialize();

	// Initialize the application object (if present)
	if(myApplication) myApplication->initialize();

	myIsInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::run()
{
	// If the system manager has not been initialized yet, do it now.
	if(!myIsInitialized) initialize();

	myInputManager->start();
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::registerInputService(String svcName, InputServiceAllocator creationFunc)
{
	myInputServiceRegistry.insert(InputServiceDictionary::value_type(std::string(svcName), creationFunc));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputServiceAllocator SystemManager::findInputService(String svcName)
{
	InputServiceDictionary::const_iterator elem = myInputServiceRegistry.find(svcName);
	if(elem == myInputServiceRegistry.end()) return NULL;
	return elem->second;
}
