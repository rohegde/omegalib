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
#include "omega/Application.h"
#include "omega/ServiceManager.h"
#include "omega/StringUtils.h"

using namespace omega;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ServiceManager::ServiceManager(SystemManager* sys):
	mySys(sys),
	myEventBuffer(NULL),
	myEventBufferHead(0),
	myEventBufferTail(0),
	myAvailableEvents(0),
	myDroppedEvents(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ServiceManager::~ServiceManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::initialize()
{
	omsg("ServiceManager::initialize");

	myEventBuffer = new Event[MaxEvents];
	ofmsg("Event buffer allocated. Max events: %1%", %MaxEvents);

	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->initialize();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::dispose()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->dispose();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::start()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->start();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::stop()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->stop();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::poll()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->poll();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::addService(Service* svc)
{
	oassert(svc != NULL);

	myServices.push_back(svc);
	svc->setManager(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::removeService(Service* svc)
{
	oassert(false | !"Not Implemented");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ServiceManager::incrementBufferIndex(int index)
{
	index++;
	if(index == ServiceManager::MaxEvents) index = 0;
	return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ServiceManager::decrementBufferIndex(int index)
{
	if(index == 0) index = ServiceManager::MaxEvents;
	index--;
	return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ServiceManager::getEvents(Event* ptr, int maxEvents)
{
	int returnedEvents = 0;
	Event* evt;
	
	lockEvents();
	do
	{
		evt = readTail();
		if(evt)	
		{
			memcpy(&ptr[returnedEvents], evt, sizeof(Event));
			returnedEvents++;
		}
	} while(evt && returnedEvents < maxEvents);
	unlockEvents();

	return returnedEvents;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::lockEvents()
{
#ifdef OMEGA_USE_DISPLAY
	myEventBufferLock.set();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ServiceManager::unlockEvents()
{
#ifdef OMEGA_USE_DISPLAY
	myEventBufferLock.unset();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Event* ServiceManager::writeHead()
{
	Event* evt = &myEventBuffer[myEventBufferHead];
	myEventBufferHead = incrementBufferIndex(myEventBufferHead);

	// This is not totally exact, we would need an event more to actually start dropping..
	if(myAvailableEvents == MaxEvents)
	{
		myDroppedEvents++;
	}
	else
	{
		myAvailableEvents++;
	}

	return evt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Event* ServiceManager::readHead()
{
	if(myAvailableEvents > 0)
	{
		Event* evt = &myEventBuffer[myEventBufferHead];
		myEventBufferHead = decrementBufferIndex(myEventBufferHead);
		myAvailableEvents--;
		return evt;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Event* ServiceManager::readTail()
{
	if(myAvailableEvents > 0)
	{
		Event* evt = &myEventBuffer[myEventBufferTail];
		myEventBufferTail = incrementBufferIndex(myEventBufferTail);
		myAvailableEvents--;
		return evt;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void ServiceManager::processEvents(ApplicationServer* app)
//{
//	oassert(app);
//
//	int av = getAvailableEvents();
//	if(av != 0)
//	{
//		// @todo: Instead of copying the event list, we can lock the main one.
//		Event evts[ServiceManager::MaxEvents];
//		getEvents(evts, ServiceManager::MaxEvents);
//		for( int evtNum = 0; evtNum < av; evtNum++)
//		{
//			app->handleEvent(evts[evtNum]);
//		}
//	}
//}
