/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/ui/DefaultSkin.h"
#include "omega/ui/UIManager.h"
#include "omega/ui/Container.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::UIManager():
	myRootContainer(new Container("root")),
	myWidgetFactory(new DefaultWidgetFactory()),
	myDefaultFont(NULL),
	myEventHandler(NULL)
{
	myRootContainer->setUIManager(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::~UIManager() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::update(const UpdateContext& context)
{
	myRootContainer->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::draw(const DrawContext& context, const Recti& viewport)
{
	// Update the layout.
	myRootContainer->setPosition(viewport[0]);
	myRootContainer->setSize(viewport[1]);
	myRootContainer->layout();

	// Draw ui.
	myRootContainer->draw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::processInputEvent(const InputEvent& evt)
{
	// UI widgets only manage touch and pointer events.
	if(evt.serviceType == InputService::Touch || evt.serviceType == InputService::Pointer)
	{
		//if(evt.type == InputEvent::Move) 
		//	printf("move! at %f %f\n", evt.position[0], evt.position[1]);
		//if(evt.type == InputEvent::Rotate) 
		//	printf("rotate! at %f %f, deg:%f\n", evt.position[0], evt.position[1], evt.rotation[0]);
		//if(evt.type == InputEvent::Split) printf("split!\n");
		myRootContainer->processInputEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::dispatchUIEvent(const UIEvent& evt)
{
	if(myEventHandler)
	{
		myEventHandler->handleUIEvent(evt);
	}
}
