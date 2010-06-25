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
	InputManager(SystemManager* sys);

	// Class destructor
	~InputManager();

	// Add a new input service to the manager.
	void AddService(InputService* svc);
	void RemoveService(InputService* svc);

	// Initialize
	void Initialize();
	void Start();
	void Stop();
	void Dispose();

	// TEMPORARY: this will run in a thread in the future.
	void Poll();

	void ProcessEvents(Application* app);

	int GetAvailableEvents() { return myAvailableEvents; }
	int GetDroppedEvents() { return myDroppedEvents; }
	void ResetDroppedEvents() { myDroppedEvents = 0; }
	int GetEvents(InputEvent* ptr, int maxEvents);

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