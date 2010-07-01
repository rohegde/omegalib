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
	myExitRequested(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager::~SystemManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::setup(Config* cfg)
{
	omsg("SystemManager::initialize");

	myConfig = cfg;
	myInputManager = new InputManager(this);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::initialize()
{
	if(myDisplaySystem) myDisplaySystem->initialize(this);
	myInputManager->initialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::run()
{
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
void SystemManager::postExitRequest()
{
	myExitRequested = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::registerInputService(const char* svcName, InputManager* (creationFunc)())
{
}
