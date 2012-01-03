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
#include "oengine/PortholeTabletService.h"
#include "oengine/Camera.h"
#include "oengine/EngineApplication.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
void oengine::registerPortholeTabletService()
{
	SystemManager::instance()->getServiceManager()->registerService("PortholeTabletService", (ServiceAllocator)PortholeTabletService::New);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletGuiElement* TabletGuiElement::createButton(int id, const String& label, const String& description, const String& text)
{
	TabletGuiElement* e = new TabletGuiElement();
	e->setId(id);
	e->setType(TabletGuiElement::ElementTypeButton);
	e->setLabel(label);
	e->setDescription(description);
	e->setText(text);
	return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletGuiElement* TabletGuiElement::createSlider(int id, const String& label, const String& description, int min, int max, int value)
{
	TabletGuiElement* e = new TabletGuiElement();
	e->setId(id);
	e->setType(TabletGuiElement::ElementTypeSlider);
	e->setLabel(label);
	e->setMinimum(min);
	e->setMaximum(max);
	e->setValue(value);
	return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletGuiElement* TabletGuiElement::createSwitch(int id, const String& label, const String& description, bool value)
{
	TabletGuiElement* e = new TabletGuiElement();
	e->setId(id);
	e->setType(TabletGuiElement::ElementTypeSwitch);
	e->setLabel(label);
	e->setValue(value ? 1 : 0);
	return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const char* TabletGuiElement::getDef()
{
	switch(myType)
	{
	case ElementTypeButton:
		sprintf(myElemDef, "0:%d:%s:%s:%s:|", myId, myLabel.c_str(), myDescription.c_str(), myText.c_str());
		break;
	case ElementTypeSlider:
		sprintf(myElemDef, "1:%d:%s:%s:%d:%d:%d:|", myId, myLabel.c_str(), myDescription.c_str(), myMin, myMax, myValue);
		break;
	case ElementTypeSwitch:
		sprintf(myElemDef, "2:%d:%s:%s:%d:|", myId, myLabel.c_str(), myDescription.c_str(), myValue);
		break;
	}
	return myElemDef;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PortholeTabletService::PortholeTabletService():
myAutoUpdateInterval(0.5f), myEngine(NULL), myEnabled(true), myTabletService(NULL), myEventFlags(Event::Left)
{
	setPollPriority(Service::PollLast);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::setup(Setting& settings)
{
	myHires = true;
	myOffscreen = true;

	// Create and setup the internal tablet service.
	myTabletService	= (BasicPortholeService*)getManager()->addService("BasicPortholeService");
	if(myTabletService != NULL)
	{
		myTabletService->doSetup(getManager(), settings);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::initialize()
{
	EngineModuleServices::addModule(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::initialize(EngineServer* engine)
{
	myEngine = engine;

	if(myHires)
	{
		myTabletPixels = new PixelData(PixelData::FormatRgb, 840, 480);
	}
	else
	{
		myTabletPixels = new PixelData(PixelData::FormatRgb, 420, 240);
	}

	uint flags = Camera::ForceMono | Camera::DrawScene;
	if(myOffscreen) flags |= Camera::Offscreen;

	myTabletCamera = myEngine->createCamera(flags);
	myTabletCamera->setProjection(60, 1, 0.1f, 100);
	myTabletCamera->setAutoAspect(true);

	// Initialize the tablet camera position to be the same as the main camera.
	Camera* defaultCamera = myEngine->getDefaultCamera();
	myTabletCamera->setPosition(defaultCamera->getPosition());

	//Quaternion o = AngleAxis(-Math::HalfPi, Vector3f::UnitX());
	//myTabletCamera->setOrientation(o);
	myTabletCamera->getOutput(0)->setReadbackTarget(myTabletPixels);
	myTabletCamera->getOutput(0)->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::update(const UpdateContext& context)
{
	// Enable / disable tablet camera rendering
	myTabletCamera->getOutput(0)->setEnabled(myEnabled);

	if(myEnabled)
	{
		if(context.time - myLastUpdateTime > myAutoUpdateInterval)
		{
			myLastUpdateTime = context.time;
			ByteArray* png = ImageUtils::encode(myTabletPixels, ImageUtils::FormatPng);
			foreach(TabletConnection* tablet, myTablets)
			{
				sendMessage(tablet, "ipng", png->getData(), png->getSize());
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::poll()
{
	lockEvents();
	int numEvts = getManager()->getAvailableEvents();
	for(int i = 0; i < numEvts; i++)
	{
		Event* evt = getEvent(i);
		processEvent(evt);
	}
	unlockEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::processEvent(Event* evt)
{
	if(myTabletService != NULL)
	{
		if(evt->getServiceType() == Service::Pointer && evt->getServiceId() == myTabletService->getServiceId())
		{
			Vector2f normalizedPoint(evt->getPosition(0), 1 - evt->getPosition(1));
			//ofmsg("pt %1%", %normalizedPoint);
			Ray ray = myTabletCamera->getViewRay(normalizedPoint);
			evt->clearFlags();
			evt->setFlags(myEventFlags);
			evt->setExtraDataType(Event::ExtraDataVector3Array);
			evt->setExtraDataVector3(0, ray.getOrigin());
			evt->setExtraDataVector3(1, ray.getDirection());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::handleEvent(const Event& evt)
{
	if( evt.getServiceType() == Service::Generic )
	{
		if(evt.getType() == Event::Connect)
		{
			TabletConnection* connection = myTabletService->getConnection(evt.getServiceId());
			myTablets.push_back(connection);

			if(myGuiElements.size() != 0)
			{
				sendMessage(connection, "mgui", myGuiDef, strlen(myGuiDef));
			}

			evt.setProcessed();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::beginGui()
{
	foreach(TabletGuiElement* e, myGuiElements)
	{
		delete e;
	}
	myGuiElements.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::addGuiElement(TabletGuiElement* elem)
{
	myGuiElements.push_back(elem);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::finishGui()
{
	refreshGuiDef();
	foreach(TabletConnection* tablet, myTablets)
	{
		sendMessage(tablet, "mgui", myGuiDef, strlen(myGuiDef));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::refreshGuiDef()
{
	memset(myGuiDef, 0, MaxGuiDefSize);
	foreach(TabletGuiElement* e, myGuiElements)
	{
		strcat(myGuiDef, e->getDef());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PortholeTabletService::sendMessage(TabletConnection* conn, const char* header, void* data, int size)
{
	conn->write((void*)header, 4);
	conn->write(&size, sizeof(int));
	conn->write(data, size);
}

