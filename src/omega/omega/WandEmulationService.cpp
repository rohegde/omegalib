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
#include "omega/WandEmulationService.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
WandEmulationService::WandEmulationService():
	myEventFlags(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool WandEmulationService::processKey(const Event* evt, const char key, Event::Flags flag)
{
	Event* newEvent;
	if(evt->isKeyDown(key))
	{
		myEventFlags |= flag;
		newEvent = writeHead();
		newEvent->reset(Event::Down, Service::Wand, 0);
		newEvent->setFlags(myEventFlags);
		return true;
	}
	if(evt->isKeyUp(key))
	{
		myEventFlags &= ~flag;
		newEvent = writeHead();
		newEvent->reset(Event::Up, Service::Wand, 0);
		newEvent->setFlags(myEventFlags);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool WandEmulationService::processMouseButton(const Event* evt, Event::Flags flag)
{
	Event* newEvent;
	if(evt->getType() == Event::Down && evt->isFlagSet(flag))
	{
		myEventFlags |= flag;
		newEvent = writeHead();
		newEvent->reset(Event::Down, Service::Wand, 0);
		newEvent->setFlags(myEventFlags);
		return true;
	}
	if(evt->getType() == Event::Up && evt->isFlagSet(flag))
	{
		myEventFlags &= ~flag;
		newEvent = writeHead();
		newEvent->reset(Event::Down, Service::Wand, 0);
		newEvent->setFlags(myEventFlags);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WandEmulationService::poll()
{
	lockEvents();
	int numEvts = getManager()->getAvailableEvents();
	bool eventWasKeyUpDown = false;
	for(int i = 0; i < numEvts; i++)
	{
		Event* evt = getEvent(i);
		eventWasKeyUpDown |= processKey(evt, 'a', Event::ButtonLeft);
		eventWasKeyUpDown |= processKey(evt, 'd', Event::ButtonRight);
		eventWasKeyUpDown |= processKey(evt, 'w', Event::ButtonUp);
		eventWasKeyUpDown |= processKey(evt, 's', Event::ButtonDown);

		eventWasKeyUpDown |= processKey(evt, '1', Event::Button3);
		eventWasKeyUpDown |= processKey(evt, '2', Event::Button4);
		eventWasKeyUpDown |= processKey(evt, 'r', Event::Button5);
		eventWasKeyUpDown |= processKey(evt, 'f', Event::Button6);

		eventWasKeyUpDown |= processMouseButton(evt, Event::Button1);
		eventWasKeyUpDown |= processMouseButton(evt, Event::Button2);
	}

	if(!eventWasKeyUpDown)
	{
		Event* newEvent = writeHead();
		newEvent->reset(Event::Update, Service::Wand, 0);
		newEvent->setFlags(myEventFlags);
	}

	unlockEvents();
}

