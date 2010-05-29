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
#include "DisplaySystem.h"
#include "InputManager.h"
#include "Log.h"
#include "SystemManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager* SystemManager::myInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemManager* SystemManager::GetInstance()
{
	if(!myInstance) myInstance = new SystemManager();
	return myInstance;
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
void SystemManager::Setup(Config* cfg)
{
	ODBG("SystemManager::Initialize");

	myConfig = cfg;
	myInputManager = new InputManager(this);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::Initialize()
{
	if(myDisplaySystem) myDisplaySystem->Initialize(this);
	myInputManager->Initialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::Run()
{
	if(myDisplaySystem)
	{
		myDisplaySystem->Run();
	}
	else
	{
		Log::Warning("SystemManager::Run - no display system specified, returning immediately");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::Cleanup()
{
	if(myDisplaySystem) myDisplaySystem->Cleanup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SystemManager::PostExitRequest()
{
	myExitRequested = true;
}

