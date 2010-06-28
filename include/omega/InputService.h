/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Input service definition
 *********************************************************************************************************************/
#ifndef __INPUT_SERVICE_H__
#define __INPUT_SERVICE_H__

#include "osystem.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
struct InputEvent;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InputService
{
friend class InputManager;
public:
	enum ServiceType { Pointer, Mocap, Touch, Keyboard }; 

public:
	// Class constructor
	//	@param manager: The input manager owning this service
	InputService(): myManager(NULL) {}

   // Class destructor
	~InputService() {}

	InputManager* GetManager() { return myManager; }

	virtual void Initialize() {}
	virtual void Start() {}
	virtual void Poll() {}
	virtual void Stop() {}
	virtual void Dispose() {}

protected:
	void LockEvents();
	void UnlockEvents();
	InputEvent* WriteHead();
	InputEvent* ReadHead();
	InputEvent* ReadTail();

private:
	void SetManager(InputManager* mng) { myManager = mng; }

private:
	InputManager* myManager;
};
}; // namespace omega

#endif