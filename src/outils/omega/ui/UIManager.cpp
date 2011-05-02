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

#include "omega/StringUtils.h"

using namespace omega;
using namespace omega::ui;

// Uncomment to enable pointer debugging
//#define DEBUG_POINTER

#ifdef DEBUG_POINTER
int gx = 0;
int gy = 0;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::UIManager():
	myWidgetFactory(new DefaultWidgetFactory()),
	myDefaultFont(NULL),
	myEventHandler(NULL),
	myDefaultPainter(NULL)
{
	for(int i =0; i < Application::MaxLayers; i++)
	{
		myRootContainer[i] = new Container(ostr("root%1%", %i));
		myRootContainer[i]->setUIManager(this);
	}
	myDefaultPainter = new Painter();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::~UIManager() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::update(const UpdateContext& context)
{
	for(int i =0; i < Application::MaxLayers; i++)
	{
		myRootContainer[i]->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::draw(const DrawContext& context)
{
	// Get the UI layer id (higher two bytes of 4 byte layer in DrawContext)
	int layer = (context.layer >> 2) - 1;
	Container* root = myRootContainer[layer];

	const Rect& viewport = context.viewport;
	// Update the root container size if necessary.
	if((root->getPosition().cwiseNotEqual(viewport.min.cast<float>())).all() ||
		root->getSize().cwiseNotEqual(viewport.max.cast<float>()).all())
	{
		root->setPosition(viewport.min.cast<float>());
		root->setSize(Vector2f(viewport.width(), viewport.height()));
	}

	// Make sure all widget sizes are up to date (and perform autosize where necessary).
	root->updateSize();

	// Layout ui.
	root->layout();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, viewport.height() - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewport.width(), 0, viewport.height(), -1, 1);

    glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw ui.
	root->draw();

#ifdef DEBUG_POINTER
	glPointSize(32);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(gx, gy);
	glEnd();
#endif

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
#ifdef DEBUG_POINTER
		gx = (int)evt.position[0];
		gy = (int)evt.position[1];
		ofmsg("Pos: %d %d", %gx %gy);
#endif
		bool handled = false;
		for(int i =0; i < Application::MaxLayers; i++)
		{
			handled |= myRootContainer[i]->processInputEvent(evt);
		}
		return handled;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::dispatchUIEvent(const UIEvent& evt)
{
	if(myEventHandler)
	{
		myEventHandler->handleUIEvent(evt);
	}
}
