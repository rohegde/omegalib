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
#include "omega/ui/DefaultSkin.h"
#include "omega/ui/UIManager.h"
#include "omega/ui/Container.h"
#include "omega/ui/Painter.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::UIManager():
	myRootContainer(new Container("root")),
	myWidgetFactory(new DefaultWidgetFactory()),
	myDefaultFont(NULL),
	myEventHandler(NULL),
	myDefaultPainter(NULL)
{
	myRootContainer->setUIManager(this);
	myDefaultPainter = new Painter();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::~UIManager() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::update(const UpdateContext& context)
{
	myRootContainer->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::draw(const Recti& viewport)
{
	// Update the root container size if necessary.
	if(myRootContainer->getPosition() != viewport[0] ||
		myRootContainer->getSize() != viewport[1])
	{
		myRootContainer->setPosition(viewport[0]);
		myRootContainer->setSize(viewport[1]);
	}

	// Make sure all widget sizes are up to date (and perform autosize where necessary).
	myRootContainer->updateSize();

	// Layout ui.
	myRootContainer->layout();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, viewport[1][1] - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewport[1][0], 0, viewport[1][1], -1, 1);

    glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw ui.
	myRootContainer->draw();

	glDisable (GL_BLEND);
	glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool UIManager::processInputEvent(const Event& evt)
{
	// UI widgets only manage touch and pointer events.
	if(evt.serviceType == Service::Pointer)
	{
		myRootContainer->processInputEvent(evt);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::dispatchUIEvent(const UIEvent& evt)
{
	if(myEventHandler)
	{
		myEventHandler->handleUIEvent(evt);
	}
}
