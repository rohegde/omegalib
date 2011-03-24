/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omega/Observer.h"
#include "omega/ObserverUpdateService.h"
#include "omega/StringUtils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
ObserverUpdateService::ObserverUpdateService():
	myObserver(NULL),
	mySourceId(0),
	myEnableLookAt(false)
{
	myLookAt = Vector3f::Zero();
	setPollPriority(Service::PollLast);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::setup(Setting& settings)
{
	if(settings.exists("sourceId"))
	{
		mySourceId = settings["sourceId"];
	}
	if(settings.exists("lookAt"))
	{
		myEnableLookAt = true;
		Setting& stLa = settings["lookAt"];
		myLookAt(0) = stLa[0];
		myLookAt(1) = stLa[1];
		myLookAt(2) = stLa[2];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::initialize()
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	myObserver = ds->getObserver(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::poll()
{
	lockEvents();
	int numEvts = getManager()->getAvailableEvents();
	for(int i = 0; i < numEvts; i++)
	{
		Event* evt = getEvent(i);
		if(evt->serviceType == Service::Mocap)
		{
			if(evt->sourceId == mySourceId)
			{
				//ofmsg("Observer pos: %1%", %evt->position);
				//myObserver->update(evt->position, evt->orientation);

				Quaternion q = Quaternion::Identity();

				// If look at is enabled, compute orientation using target lookat point.
				if(myEnableLookAt)
				{
					Vector3f dir = myLookAt - evt->position;
					dir.normalize();
					Vector3f localZ = Vector3f(0, 0, -1);
					q = Math::buildRotation(localZ, dir, Vector3f::Zero());
				}

				myObserver->update(evt->position, q);
			}
		}
	}
	unlockEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::dispose()
{
}

