/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * SystemManager class declaration.
 *********************************************************************************************************************/
#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__

#include "osystem.h"

#include "boost/unordered_map.hpp"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class Application;
class Config;
class DisplaySystem;
class InputManager;
class InputService;
class InputFilter;

typedef InputService* (*InputServiceAllocator)();
typedef boost::unordered_map<String, InputServiceAllocator> InputServiceDictionary;

typedef InputFilter* (*InputFilterAllocator)();
typedef boost::unordered_map<String, InputFilterAllocator> InputFilterDictionary;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The SystemManager class is the root object of the omegalib architecture.
class SystemManager
{
public:
	// Get the singleton instance of the system manager.
	OMEGA_API static SystemManager* instance();

	//! Initializes the system manager
	OMEGA_API void setup(Config* cfg);

	OMEGA_API void registerInputService(String svcName, InputServiceAllocator creationFunc);

	//! Find an input service allocator given the input service name.
	//! See registerInputService for additional information.
	OMEGA_API InputServiceAllocator findInputService(String svcName);

	OMEGA_API void initialize();

	//! Starts running the current application.
	//! This method does not return until the application is done running.
	OMEGA_API void	run();

	//! Sends an exit request to the system manager.
	OMEGA_API void postExitRequest();

	//! Cleans up runtime resources, performs final debug checks etc.
	OMEGA_API void cleanup();

	//! Gets the system configuration.
	OMEGA_API Config* getConfig() { return myConfig; }

	//! Gets the InputManager object
	OMEGA_API InputManager* getInputManager() { return myInputManager; }

	//! Gets the DisplaySystem object
	OMEGA_API DisplaySystem* getDisplaySystem() { return myDisplaySystem; }

	//! Sets the DisplaySystem object
	OMEGA_API void setDisplaySystem(DisplaySystem* value) { myDisplaySystem = value; }

	//! Gets the Application object
	OMEGA_API Application* getApplication() { return myApplication; }

	//! Sets the Application object
	OMEGA_API void setApplication(Application* value) { myApplication = value; }

	OMEGA_API bool isExitRequested() { return myExitRequested; }

	OMEGA_API bool isInitialized() { return myIsInitialized; }

private:
	SystemManager();
	~SystemManager();

	void setupInputManager();
	void setupDisplaySystem();

private:
	// Singleton instance.
	static SystemManager* mysInstance;

	bool myIsInitialized;

	// The input manager registry.
	InputServiceDictionary myInputServiceRegistry;

	// The input service registry.
	InputFilterDictionary myInputFilterRegistry;

	Config*			myConfig;
	DisplaySystem*	myDisplaySystem;
	InputManager*	myInputManager;
	Application*	myApplication;
	bool			myExitRequested;
};

}; // namespace omega

#endif