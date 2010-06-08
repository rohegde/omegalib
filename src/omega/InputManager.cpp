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
#include "InputManager.h"

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
void InputManager::Initialize()
{
	ODBG("InputManager::Initialize");

	myEventBuffer = new InputEvent[MaxEvents];
	Log::Message("Event buffer allocated. Max events: %d", MaxEvents);

	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->Initialize();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::Dispose()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->Dispose();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::Start()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->Start();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::Stop()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->Stop();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::Poll()
{
	for(int i = 0; i < myServices.size(); i++)
	{
		myServices[i]->Poll();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::AddService(InputService* svc)
{
	OASSERT(svc != NULL);

	myServices.push_back(svc);
	svc->SetManager(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::RemoveService(InputService* svc)
{
	OASSERT(false | !"Not Implemented");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InputManager::IncrementBufferIndex(int index)
{
	index++;
	if(index == InputManager::MaxEvents) index = 0;
	return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InputManager::DecrementBufferIndex(int index)
{
	if(index == 0) index = InputManager::MaxEvents;
	index--;
	return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InputManager::GetEvents(InputEvent* ptr, int maxEvents)
{
	int returnedEvents = 0;
	InputEvent* evt;
	
	LockEvents();
	do
	{
		evt = ReadTail();
		if(evt)	
		{
			memcpy(&ptr[returnedEvents], evt, sizeof(InputEvent));
			returnedEvents++;
		}
	} while(evt && returnedEvents < maxEvents);
	UnlockEvents();

	return returnedEvents;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::LockEvents()
{
	myEventBufferLock.set();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputManager::UnlockEvents()
{
	myEventBufferLock.unset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputManager::WriteHead()
{
	InputEvent* evt = &myEventBuffer[myEventBufferHead];
	myEventBufferHead = IncrementBufferIndex(myEventBufferHead);

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
InputEvent* InputManager::ReadHead()
{
	if(myAvailableEvents > 0)
	{
		InputEvent* evt = &myEventBuffer[myEventBufferHead];
		myEventBufferHead = DecrementBufferIndex(myEventBufferHead);
		myAvailableEvents--;
		return evt;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InputEvent* InputManager::ReadTail()
{
	if(myAvailableEvents > 0)
	{
		InputEvent* evt = &myEventBuffer[myEventBufferTail];
		myEventBufferTail = IncrementBufferIndex(myEventBufferTail);
		myAvailableEvents--;
		return evt;
	}
	return NULL;
}
