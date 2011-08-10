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
#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "osystem.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class Event;
	class ServiceManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Service: public DynamicObject
	{
	friend class ServiceManager;
	public:
		enum ServiceType { Pointer, Mocap, Keyboard, Controller, UI }; 
		enum ServicePollPriority { PollFirst, PollNormal, PollLast }; 

	public:
		// Class constructor
		Service(): myManager(NULL), myPriority(PollNormal) {}

	   // Class destructor
		virtual ~Service() {}

		ServiceManager* getManager();
		String getName();

		ServicePollPriority getPollPriority();
		void setPollPriority(ServicePollPriority value);

		virtual void setup(Setting& settings) {}
		virtual void initialize() {}
		virtual void start() {}
		virtual void poll() {}
		virtual void stop() {}
		virtual void dispose() {}

	protected:
		void lockEvents();
		void unlockEvents();
		Event* writeHead();
		Event* readHead();
		Event* readTail();
		Event* getEvent(int index);

	private:
		void doSetup(Setting& settings);
		void setManager(ServiceManager* mng) { myManager = mng; }

	private:
		ServiceManager* myManager;
		String myName;
		ServicePollPriority myPriority;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ServiceManager* Service::getManager() 
	{ return myManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline String Service::getName() 
	{ return myName; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Service::ServicePollPriority Service::getPollPriority()
	{ return myPriority; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Service::setPollPriority(Service::ServicePollPriority value)
	{ myPriority = value; }
}; // namespace omega

#endif
