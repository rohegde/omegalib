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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InputService
{
friend class InputManager;
public:
	// Class constructor
	//	@param manager: The input manager owning this service
	InputService(): myManager(NULL) {}

   // Class destructor
	~InputService() {}

	InputManager* GetManager() { return myManager; }

	virtual void Initialize() {}
	virtual void Start() {}
	virtual void Stop() {}
	virtual void Dispose() {}

protected:
	void LockEvents() { myManager->LockEvents(); }
	void UnlockEvents() { myManager->UnlockEvents(); }
	InputEvent* WriteHead() { return myManager->WriteHead(); }
	InputEvent* ReadHead() { return myManager->ReadHead(); }
	InputEvent* ReadTail() { return myManager->ReadTail(); }

private:
	void SetManager(InputManager* mng) { myManager = mng; }

private:
	InputManager* myManager;
};
}; // namespace omega

#endif