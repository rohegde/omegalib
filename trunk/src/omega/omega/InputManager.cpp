/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * InputManager members implementation. See InputManager.h for more details.
 *********************************************************************************************************************/
#include "omega/Application.h"
#include "omega/InputManager.h"

using namespace omega;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputManager::InputManager(SystemManager* sys):
	mySys(sys),
	myEventBuffer(NULL),
	myEventBufferHead(0),
	myEventBufferTail(0),
	myAvailableEvents(0),
	myDroppedEvents(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputManager::~InputManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::initialize()
{
	omsg("InputManager::initialize");

	myEventBuffer = new InputEvent[MaxEvents];
	omsg("Event buffer allocated. Max events: %d", MaxEvents);

	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->initialize();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::dispose()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->dispose();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::start()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->start();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::stop()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->stop();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::poll()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->poll();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::addService(InputService* svc)
{
	oassert(svc != NULL);

	myServices.push_back(svc);
	svc->setManager(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::removeService(InputService* svc)
{
	oassert(false | !"Not Implemented");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InputManager::incrementBufferIndex(int index)
{
	index++;
	if(index == InputManager::MaxEvents) index = 0;
	return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InputManager::decrementBufferIndex(int index)
{
	if(index == 0) index = InputManager::MaxEvents;
	index--;
	return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InputManager::getEvents(InputEvent* ptr, int maxEvents)
{
	int returnedEvents = 0;
	InputEvent* evt;
	
	lockEvents();
	do
	{
		evt = readTail();
		if(evt)	
		{
			memcpy(&ptr[returnedEvents], evt, sizeof(InputEvent));
			returnedEvents++;
		}
	} while(evt && returnedEvents < maxEvents);
	unlockEvents();

	return returnedEvents;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::lockEvents()
{
	myEventBufferLock.set();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::unlockEvents()
{
	myEventBufferLock.unset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputManager::writeHead()
{
	InputEvent* evt = &myEventBuffer[myEventBufferHead];
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
InputEvent* InputManager::readHead()
{
	if(myAvailableEvents > 0)
	{
		InputEvent* evt = &myEventBuffer[myEventBufferHead];
		myEventBufferHead = decrementBufferIndex(myEventBufferHead);
		myAvailableEvents--;
		return evt;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputManager::readTail()
{
	if(myAvailableEvents > 0)
	{
		InputEvent* evt = &myEventBuffer[myEventBufferTail];
		myEventBufferTail = incrementBufferIndex(myEventBufferTail);
		myAvailableEvents--;
		return evt;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void InputManager::processEvents(ApplicationServer* app)
//{
//	oassert(app);
//
//	int av = getAvailableEvents();
//	if(av != 0)
//	{
//		// @todo: Instead of copying the event list, we can lock the main one.
//		InputEvent evts[InputManager::MaxEvents];
//		getEvents(evts, InputManager::MaxEvents);
//		for( int evtNum = 0; evtNum < av; evtNum++)
//		{
//			app->handleEvent(evts[evtNum]);
//		}
//	}
//}
