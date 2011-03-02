/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "osystem.h"
#include "Application.h"
#include "Event.h"
#define OMEGA_MAX_EVENTS 1024

namespace omega
{
// Forward declarations.
class SystemManager;

///////////////////////////////////////////////////////////////////////////////////////////////////
class OMEGA_API ServiceManager
{
friend class Service;

public:
	// Class constructor.
	ServiceManager(SystemManager* sys);

	// Class destructor
	~ServiceManager();

	// Add a new input service to the manager.
	void addService(Service* svc);
	void removeService(Service* svc);

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
	int getEvents(Event* ptr, int maxEvents);

public:
	// The maximum number of events stored in the event buffer.
	static const int MaxEvents;
	
private:
	int incrementBufferIndex(int index);
	int decrementBufferIndex(int index);
	void lockEvents();
	void unlockEvents();
	Event* writeHead();
	Event* readHead();
	Event* readTail();
	Event* getEvent(int index);

private:
	SystemManager*	mySys;

	Lock  myEventBufferLock;

	Event*		myEventBuffer;
	int				myEventBufferHead;
	int				myEventBufferTail;
	std::vector<Service*> myServices;

	int myAvailableEvents;
	int myDroppedEvents;
};
}; // namespace omega

#endif