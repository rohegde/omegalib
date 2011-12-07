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
	myAutoUpdateInterval(5.0f), myEngine(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::initialize(EngineServer* engine)
{
	myEngine = engine;

	myTabletPixels = new PixelData(PixelData::FormatRgba, 420, 240);

	Camera* cam = myEngine->createCamera(Camera::ForceMono | Camera::Offscreen | Camera::DrawScene);
	cam->setProjection(30, 1, 0.1f, 100);
	cam->setAutoAspect(true);
	cam->setPosition(Vector3f(1, 3, 0));
	Quaternion o = AngleAxis(-Math::HalfPi, Vector3f::UnitX());
	cam->setOrientation(o);
	cam->getOutput(0)->setReadbackTarget(myTabletPixels);
	cam->getOutput(0)->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::update(const UpdateContext& context)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletManagerModule::handleEvent(const Event& evt)
{
	if( evt.getServiceType() == Service::Generic )
    {
		if(evt.getType() == Event::Connect)
		{
			TabletService* tsvc = myEngine->getServiceManager()->getService<TabletService>(evt.getServiceId());
			TabletInterface* tablet = new TabletInterface(tsvc, evt.getSourceId());
			tablet->beginGui();
			tablet->addButton(0, "Button1", "Hello I'm a Button", "Click me!");
			tablet->addButton(1, "Button2", "Hello I'm another Button", "Click me!");
			tablet->addSlider(2, "Slider1", "Hello I'm a Slider! (0, 100, 50)", 0, 100, 50);
			tablet->addSlider(3, "Slider2", "Hello I'm another Slider! (0, 10, 10)", 0, 10, 10);
			tablet->addSwitch(4, "Switch1", "Hello I'm a Switch! (on)", true);
			tablet->addSwitch(5, "Switch2", "Hello I'm another Switch! (off)", false);
			tablet->finishGui();

			myTablets.push_back(tablet);

			evt.setProcessed();
		}
	}
}
