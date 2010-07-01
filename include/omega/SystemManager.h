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
	OMEGA_API static SystemManager* instance();

	// initializes the system manager
	OMEGA_API void setup(Config* cfg);

	OMEGA_API void registerInputService(const char* svcName, InputManager* (creationFunc)());

	OMEGA_API void initialize();

	// Starts running the current application.
	// This method does not return until the application is done running.
	OMEGA_API void	run();

	// Sends an exit request to the system manager.
	OMEGA_API void postExitRequest();

	// Cleans up runtime resources, performs final debug checks etc.
	OMEGA_API void cleanup();

	// Gets the system configuration.
	OMEGA_API Config* getConfig() { return myConfig; }

	// Gets the InputManager object
	OMEGA_API InputManager* getInputManager() { return myInputManager; }

	// Gets the DisplaySystem object
	OMEGA_API DisplaySystem* getDisplaySystem() { return myDisplaySystem; }

	// Sets the DisplaySystem object
	OMEGA_API void setDisplaySystem(DisplaySystem* value) { myDisplaySystem = value; }

	// Gets the Application object
	OMEGA_API Application* getApplication() { return myApplication; }

	// Sets the Application object
	OMEGA_API void setApplication(Application* value) { myApplication = value; }

	OMEGA_API bool isExitRequested() { return myExitRequested; }

private:
	SystemManager();
	~SystemManager();

private:
	// Singleton instance.
	static SystemManager* mysInstance;

	Config*			myConfig;
	DisplaySystem*	myDisplaySystem;
	InputManager*	myInputManager;
	Application*	myApplication;
	bool			myExitRequested;
};

}; // namespace omega

#endif