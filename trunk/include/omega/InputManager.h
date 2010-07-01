/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Input management
 *********************************************************************************************************************/
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "osystem.h"
#include "Application.h"
#include "InputEvent.h"

namespace omega
{
// Forward declarations.
class SystemManager;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InputManager
{
friend class InputService;

public:
	// Class constructor.
	OMEGA_API InputManager(SystemManager* sys);

	// Class destructor
	OMEGA_API ~InputManager();

	// Add a new input service to the manager.
	OMEGA_API void addService(InputService* svc);
	OMEGA_API void removeService(InputService* svc);

	// initialize
	OMEGA_API void initialize();
	OMEGA_API void start();
	OMEGA_API void stop();
	OMEGA_API void dispose();

	// TEMPORARY: this will run in a thread in the future.
	OMEGA_API void poll();

	OMEGA_API void processEvents(Application* app);

	OMEGA_API int getAvailableEvents() { return myAvailableEvents; }
	OMEGA_API int getDroppedEvents() { return myDroppedEvents; }
	OMEGA_API void resetDroppedEvents() { myDroppedEvents = 0; }
	OMEGA_API int getEvents(InputEvent* ptr, int maxEvents);

public:
	// The maximum number of events stored in the event buffer.
	static const int MaxEvents = 1024;

private:
	int incrementBufferIndex(int index);
	int decrementBufferIndex(int index);
	void lockEvents();
	void unlockEvents();
	InputEvent* writeHead();
	InputEvent* readHead();
	InputEvent* readTail();

private:
	SystemManager*	mySys;
	eq::base::Lock  myEventBufferLock;
	InputEvent*		myEventBuffer;
	int				myEventBufferHead;
	int				myEventBufferTail;
	std::vector<InputService*> myServices;

	int myAvailableEvents;
	int myDroppedEvents;
};
}; // namespace omega

#endif