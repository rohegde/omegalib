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
#include "omegaToolkit/UiModule.h"
#include "omegaToolkit/ui/DefaultSkin.h"
#include "omegaToolkit/ui/Image.h"
#include "omegaToolkit/UiRenderPass.h"

using namespace omegaToolkit;
using namespace omega;

UiModule* UiModule::mysInstance = NULL;
	
///////////////////////////////////////////////////////////////////////////////////////////////////
UiModule::UiModule():
	ServerModule("UiModule"),
	myWidgetFactory(NULL)
{
	mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
UiModule::~UiModule()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::initialize()
{
	omsg("UiModule initializing...");

	myWidgetFactory = new ui::DefaultWidgetFactory(getServer());
	myUi = new ui::Container(getServer());
	myUi->setLayout(ui::Container::LayoutFree);
	myUi->setUIEventHandler(getServer());

	getServer()->registerRenderPassClass("UiRenderPass", (ServerEngine::RenderPassFactory)UiRenderPass::createInstance);
	getServer()->addRenderPass("UiRenderPass");

    Config* cfg = getServer()->getSystemManager()->getAppConfig();
	myLocalEventsEnabled = cfg->getBoolValue("config/ui/enableLocalEvents", true);
	if(cfg->exists("config/ui/images"))
	{
		const Setting& stImages = cfg->lookup("config/ui/images");
		initImages(stImages);
	}

	omsg("UiModule initialization OK");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::initImages(const Setting& images)
{
	for(int i = 0; i < images.getLength(); i++)
	{
		Setting& imageSetting = images[i];

		String fileName = Config::getStringValue("source", imageSetting, "");
		if(fileName != "")
		{
			ui::Image* img = myWidgetFactory->createImage("img", myUi);

			bool stereo = Config::getBoolValue("stereo", imageSetting, false);

			img->setStereo(stereo);
			ImageData* imgData = ImageUtils::loadImage(fileName);
			img->setData(imgData->getPixels());

			Vector2f position = Config::getVector2fValue("position", imageSetting, Vector2f(0, 0));
			Vector2f size = Config::getVector2fValue("size", imageSetting, 
				Vector2f(imgData->getWidth() / (stereo ? 2 : 1), imgData->getHeight()));
			float scale = Config::getFloatValue("scale", imageSetting, 1);

			img->setPosition(position);
			img->setSize(size * scale);
			img->setUserMoveEnabled(true);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::update(const UpdateContext& context)
{
	myUi->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::handleEvent(const Event& evt)
{
	myUi->handleEvent(evt);
}

///////////////////////////////////////////////////////////////////////////////////////////////
ui::Container* UiModule::getUi()
{
	return myUi;
}
