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

	InputManager* getManager() { return myManager; }

	virtual void initialize() {}
	virtual void start() {}
	virtual void poll() {}
	virtual void stop() {}
	virtual void dispose() {}

protected:
	void lockEvents();
	void unlockEvents();
	InputEvent* writeHead();
	InputEvent* readHead();
	InputEvent* readTail();

private:
	void setManager(InputManager* mng) { myManager = mng; }

private:
	InputManager* myManager;
};
}; // namespace omega

#endif