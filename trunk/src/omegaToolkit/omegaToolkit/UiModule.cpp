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
#include "omegaToolkit/UiRenderPass.h"

using namespace omegaToolkit;
using namespace omega;

UiModule* UiModule::mysInstance = NULL;
	
///////////////////////////////////////////////////////////////////////////////////////////////////
UiModule::UiModule():
	myWidgetFactory(NULL),
	myEngine(NULL)
{
	mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
UiModule::~UiModule()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::initialize(EngineServer* server)
{
	ofmsg("UiModule initializing...");

	myEngine = server;
	myWidgetFactory = new ui::DefaultWidgetFactory(myEngine);
	for(int i = 0; i < MaxUis; i++)
	{
		myUi[i] = new ui::Container(myEngine);
		myUi[i]->setLayout(ui::Container::LayoutFree);
		myUi[i]->setUIEventHandler(myEngine);
	}
	myEngine->registerRenderPassClass("UiRenderPass", (EngineServer::RenderPassFactory)UiRenderPass::createInstance);
	myEngine->addRenderPass("UiRenderPass");

	ofmsg("UiModule initialization OK");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::update(const UpdateContext& context)
{
	// Update ui.
	for(int i = 0; i < MaxUis; i++)
	{
		myUi[i]->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UiModule::handleEvent(const Event& evt)
{
	for(int i = 0; i < MaxUis; i++)
	{
		myUi[i]->handleEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
ui::Container* UiModule::getUi(int id)
{
	oassert(id >= 0 && id < MaxUis);
	return myUi[id];
}

