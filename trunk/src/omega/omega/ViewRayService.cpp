/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/ViewRayService.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
ViewRayService::ViewRayService():
	myDebug(false),
	myInputService(NULL),
	myDisplay(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewRayService::setup(Setting& settings)
{
	myDebug = Config::getBoolValue("debug", settings);
	myHasNormalizedInput = Config::getBoolValue("hasNormalizedInput", settings, true);

	String inputSvc = Config::getStringValue("inputService", settings);
	myInputService = getManager()->findService(inputSvc);
	if(myInputService == NULL)
	{
		ofwarn("ViewRayService::setup: could not find input service %1%", %myInputService);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewRayService::initialize()
{
	setPollPriority(Service::PollLast);
	myDisplay = SystemManager::instance()->getDisplaySystem();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewRayService::poll()
{
	if(myInputService == NULL) return;

	// Can this be moved to initialize?
	myCanvasSize = myDisplay->getCanvasSize();

	lockEvents();
	int numEvts = getManager()->getAvailableEvents();
	for(int i = 0; i < numEvts; i++)
	{
		Event* evt = getEvent(i);
		// NOTE: We do not process pointer zoom events, because the extra data is already used to
		// store the zoom factor. Replacing it with ray data would break compatibility with code using
		// pointer zoom events (usually generated by mouse wheel)
		if(evt->getServiceType() == Service::Pointer && 
			evt->getType() != Event::Zoom &&
			evt->getServiceId() == myInputService->getServiceId() )
		{
			if(myHasNormalizedInput)
			{
				const Vector3f& pos = evt->getPosition();
				evt->setPosition(
					pos[0] * myCanvasSize[0], 
					pos[1] * myCanvasSize[1]);
			}

			Vector2i pos = Vector2i(
				evt->getPosition().x(),
				evt->getPosition().y());

			Ray r = myDisplay->getViewRay(pos);
			evt->setExtraDataType(Event::ExtraDataVector3Array);
			evt->setExtraDataVector3(0, r.getOrigin());
			evt->setExtraDataVector3(1, r.getDirection());
		}
	}

	unlockEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ViewRayService::dispose()
{
}

