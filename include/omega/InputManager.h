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
	OMEGA_API void AddService(InputService* svc);
	OMEGA_API void RemoveService(InputService* svc);

	// Initialize
	OMEGA_API void Initialize();
	OMEGA_API void Start();
	OMEGA_API void Stop();
	OMEGA_API void Dispose();

	// TEMPORARY: this will run in a thread in the future.
	OMEGA_API void Poll();

	OMEGA_API void ProcessEvents(Application* app);

	OMEGA_API int GetAvailableEvents() { return myAvailableEvents; }
	OMEGA_API int GetDroppedEvents() { return myDroppedEvents; }
	OMEGA_API void ResetDroppedEvents() { myDroppedEvents = 0; }
	OMEGA_API int GetEvents(InputEvent* ptr, int maxEvents);

public:
	// The maximum number of events stored in the event buffer.
	static const int MaxEvents = 1024;

private:
	int IncrementBufferIndex(int index);
	int DecrementBufferIndex(int index);
	void LockEvents();
	void UnlockEvents();
	InputEvent* WriteHead();
	InputEvent* ReadHead();
	InputEvent* ReadTail();

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