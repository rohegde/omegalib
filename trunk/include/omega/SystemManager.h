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
	OMEGA_API static SystemManager* GetInstance();

	// Initializes the system manager
	OMEGA_API void Setup(Config* cfg);

	OMEGA_API void Initialize();

	// Starts running the current application.
	// This method does not return until the application is done running.
	OMEGA_API void Run();

	// Sends an exit request to the system manager.
	OMEGA_API void PostExitRequest();

	// Cleans up runtime resources, performs final debug checks etc.
	OMEGA_API void Cleanup();

	// Get the system configuration.
	OMEGA_API Config* GetConfig() { return myConfig; }

	OMEGA_API InputManager* GetInputManager() { return myInputManager; }

	OMEGA_API DisplaySystem* GetDisplaySystem() { return myDisplaySystem; }
	OMEGA_API void SetDisplaySystem(DisplaySystem* value) { myDisplaySystem = value; }

	OMEGA_API Application* GetApplication() { return myApplication; }
	OMEGA_API void SetApplication(Application* value) { myApplication = value; }

	OMEGA_API bool IsExitRequested() { return myExitRequested; }

private:
	SystemManager();
	~SystemManager();

private:
	// Singleton instance.
	static SystemManager* myInstance;

	Config*			myConfig;
	DisplaySystem*	myDisplaySystem;
	InputManager*	myInputManager;
	Application*	myApplication;
	bool			myExitRequested;
};

}; // namespace omega

#endif