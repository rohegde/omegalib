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
#include "oengine/TabletManagerModule.h"
#include "oengine/Camera.h"
#include "omega/TabletService.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletManagerModule::TabletManagerModule():
	myAutoUpdateInterval(0.1f), myEngine(NULL), myEnabled(true)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::beginGui()
{
	foreach(TabletGuiElement* e, myGuiElements)
	{
		delete e;
	}
	myGuiElements.empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::addGuiElement(TabletGuiElement* elem)
{
	myGuiElements.push_back(elem);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::finishGui()
{
	foreach(TabletInterface* tablet, myTablets)
	{
		tablet->beginGui();
		foreach(TabletGuiElement* e, myGuiElements)
		{
			tablet->addGuiElement(e);
		}
		tablet->finishGui();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::initialize(EngineServer* engine, bool hires, bool offscreen)
{
	myEngine = engine;

	if(hires)
	{
		myTabletPixels = new PixelData(PixelData::FormatRgb, 840, 480);
	}
	else
	{
		myTabletPixels = new PixelData(PixelData::FormatRgb, 420, 240);
	}

	uint flags = Camera::ForceMono | Camera::DrawScene;
	if(offscreen) flags |= Camera::Offscreen;

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
void TabletManagerModule::update(const UpdateContext& context)
{
	// Enable / disable tablet camera rendering
	myTabletCamera->getOutput(0)->setEnabled(myEnabled);

	if(myEnabled)
	{
		if(context.time - myLastUpdateTime > myAutoUpdateInterval)
		{
			myLastUpdateTime = context.time;
			foreach(TabletInterface* tablet, myTablets)
			{
				tablet->sendImage(myTabletPixels);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::handleEvent(const Event& evt)
{
	if( evt.getServiceType() == Service::Generic )
    {
		if(evt.getType() == Event::Connect)
		{
			TabletService* tsvc = myEngine->getServiceManager()->getService<TabletService>(evt.getServiceId());
			TabletInterface* tablet = new TabletInterface(tsvc, evt.getSourceId());
			myTablets.push_back(tablet);

			if(myGuiElements.size() != 0)
			{
				// Send the current gui definition to the tablet.
				tablet->beginGui();
				foreach(TabletGuiElement* e, myGuiElements)
				{
					tablet->addGuiElement(e);
				}
				tablet->finishGui();
			}

			evt.setProcessed();
		}
	}
}
