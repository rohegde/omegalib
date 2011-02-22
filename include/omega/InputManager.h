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
class OMEGA_API InputManager
{
friend class InputService;

public:
	// Class constructor.
	InputManager(SystemManager* sys);

	// Class destructor
	~InputManager();

	// Add a new input service to the manager.
	void addService(InputService* svc);
	void removeService(InputService* svc);

	// initialize
	void initialize();
	void start();
	void stop();
	void dispose();

	// TEMPORARY: this will run in a thread in the future.
	void poll();

	void processEvents(ApplicationServer* app);

	int getAvailableEvents() { return myAvailableEvents; }
	int getDroppedEvents() { return myDroppedEvents; }
	void resetDroppedEvents() { myDroppedEvents = 0; }
	int getEvents(InputEvent* ptr, int maxEvents);

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
	// XXX if you do not select OMEGA_USE_DISPLAY_EQUALIZER
	// then it cannot find co - Vic
#ifdef OMEGA_USE_DISPLAY
	co::base::Lock  myEventBufferLock;
#endif
	InputEvent*		myEventBuffer;
	int				myEventBufferHead;
	int				myEventBufferTail;
	std::vector<InputService*> myServices;

	int myAvailableEvents;
	int myDroppedEvents;
};
}; // namespace omega

#endif