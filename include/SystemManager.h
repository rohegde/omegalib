/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SystemManager
{
public:
	// Get the singleton instance of the system manager.
	static SystemManager* GetInstance();

	// Initializes the system manager
	void Setup(Config* cfg);

	void Initialize();

	// Starts running the current application.
	// This method does not return until the application is done running.
	void Run();

	// Sends an exit request to the system manager.
	void PostExitRequest();

	// Cleans up runtime resources, performs final debug checks etc.
	void Cleanup();

	// Get the system configuration.
	Config* GetConfig() { return myConfig; }

	InputManager* GetInputManager() { return myInputManager; }

	DisplaySystem* GetDisplaySystem() { return myDisplaySystem; }
	void SetDisplaySystem(DisplaySystem* value) { myDisplaySystem = value; }

	Application* GetApplication() { return myApplication; }
	void SetApplication(Application* value) { myApplication = value; }

	bool IsExitRequested() { return myExitRequested; }

private:
	SystemManager();
	~SystemManager();

private:
	// Singleton instance.
	static SystemManager* myInstance;

	Config* myConfig;
	DisplaySystem* myDisplaySystem;
	InputManager* myInputManager;
	Application* myApplication;
	bool myExitRequested;
};

}; // namespace omega

#endif