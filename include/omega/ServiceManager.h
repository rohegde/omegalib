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
//#include "Application.h"
#include "Event.h"
#include "Lock.h"

// Preprocessor macro, bleah.. Forced to use this instead of static constant as a quick workaround 
// to a gcc 4.2 build error. Think of a better solution in the future.
#define OMEGA_MAX_EVENTS 1024

namespace omega
{
	// Forward declarations.
	class SystemManager;

	typedef Service* (*ServiceAllocator)();
	typedef Dictionary<String, ServiceAllocator> ServiceAllocatorDictionary;
	typedef Dictionary<String, Service*> ServiceDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ServiceManager
	{
	friend class Service;

	public:
		// Class constructor.
		ServiceManager(SystemManager* sys);

		// Class destructor
		~ServiceManager();

		void registerService(String svcName, ServiceAllocator creationFunc);

		//! Find an input service allocator given the input service name.
		//! See registerInputService for additional information.
		ServiceAllocator findServiceAllocator(String svcName);

		// Add a new input service to the manager.
		void addService(Service* svc);
		void removeService(Service* svc);
		Service* findService(String svcName);
		template<typename T> T* findService(String svcName);

		// initialize
		void setup(Setting& settings);
		void initialize();
		void start();
		void stop();
		void dispose();

		void poll();

		int getAvailableEvents() { return myAvailableEvents; }
		int getDroppedEvents() { return myDroppedEvents; }
		void resetDroppedEvents() { myDroppedEvents = 0; }
		int getEvents(Event* ptr, int maxEvents);
		Event* getEvent(int index);
		void clearEvents();
		void lockEvents();
		void unlockEvents();
		Event* writeHead();

	public:
		// The maximum number of events stored in the event buffer.
		static const int MaxEvents;
	
	private:
		int incrementBufferIndex(int index);
		int decrementBufferIndex(int index);
		Event* readHead();
		Event* readTail();

	private:
		SystemManager*	mySys;

		// The service registry.
		ServiceAllocatorDictionary myServiceRegistry;

		// Dictionary of active services
		ServiceDictionary myServices;

		// Event buffer stuff.
		Lock*  myEventBufferLock;
		Event*		myEventBuffer;
		int				myEventBufferHead;
		int				myEventBufferTail;

		int myAvailableEvents;
		int myDroppedEvents;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Service* ServiceManager::findService(String svcName)
	{
		ServiceDictionary::const_iterator elem = myServices.find(svcName);
		if(elem == myServices.end()) return NULL;
		return elem->second;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> inline T* ServiceManager::findService(String svcName)
	{
		return (T*)findService(svcName);
	}
}; // namespace omega

#endif
