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
#include "oengine/ui/Widget.h"
#include "oengine/ui/Container.h"
//#include "oengine/ui/DefaultSkin.h"
#include "oengine/Renderer.h"
#include "omega/StringUtils.h"

#include "omega/glheaders.h"

#include <float.h>

using namespace omega;
using namespace oengine;
using namespace oengine::ui;

NameGenerator Widget::mysNameGenerator("Widget_");

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget::Widget(EngineServer* server):
	myInitialized(false),
	myServer(server),
	myEventHandler(NULL),
	myContainer(NULL),
	myVisible(true),
	myDebugModeColor(255, 0, 255),
	myDebugModeEnabled(false),
	myAutosize(false),
	myRotation(0),
	myUserMoveEnabled(false),
	myMoving(false),
	myMaximumSize(FLT_MAX, FLT_MAX),
	myMinimumSize(0, 0)
{
	myId = mysNameGenerator.getNext();
	myName = mysNameGenerator.generate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget::~Widget()
{
	if(myInitialized)
	{
		dispose();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::update(const omega::UpdateContext& context) 
{
	if(!myInitialized)
	{
		ofmsg("Initializing Widget: %1%", %getName());
		initialize(myServer);
		myInitialized = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::handleEvent(const Event& evt) 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::clearSizeConstaints()
{
	myMaximumSize = Vector2f(FLT_MAX, FLT_MAX);
	myMinimumSize = Vector2f(0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::setContainer(Container* value) 
{ 
	//oassert(value && value->getManager() == myManager);
	myContainer = value; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::hitTest(const Vector2f& point)
{
	float x1 = 0; //myPosition[0];
	float y1 = 0; //myPosition[1];
	float x2 = mySize[0]; // myPosition[0] + mySize[0];
	float y2 = mySize[1]; //myPosition[1] + mySize[1];

	if(point[0] >= x1 && point[1] >= y1 && point[0] < x2 && point[1] < y2) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::hitTest(const Vector2f& point, const Vector2f& pos, const Vector2f& size)
{
	float x1 = pos[0];
	float y1 = pos[1];
	float x2 = pos[0] + size[0];
	float y2 = pos[1] + size[1];

	if(point[0] >= x1 && point[1] >= y1 && point[0] < x2 && point[1] < y2) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f Widget::transformPoint(const Vector2f& point)
{
	Vector2f res = point;
	if(myContainer)
	{
		res = myContainer->transformPoint(point);
	}
	res -= myPosition;

	if(myRotation != 0)
	{
		Vector2f center = myPosition + (mySize / 2);
		res = point - center;
		float s = sin(-myRotation * omega::Math::DegToRad);
		float c = cos(-myRotation * omega::Math::DegToRad);
		float x = point[0];
		float y = point[1];
		res[0] = x * c - y * s;;
		res[1] = y * c + x * s;
		res += center;
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::dispatchUIEvent(const Event& evt)
{
	if(myEventHandler != NULL) myEventHandler->handleEvent(evt);
	else if(myContainer != NULL) myContainer->dispatchUIEvent(evt);
	//else myManager->dispatchUIEvent(evt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::requestLayoutRefresh() 
{ 
	myNeedLayoutRefresh = true; 
	if(myContainer != NULL) 
		myContainer->requestLayoutRefresh(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::needLayoutRefresh() 
{ 
	return myNeedLayoutRefresh; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::layout()
{ 
	myNeedLayoutRefresh = false; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::updateSize()
{
	if(myAutosize) autosize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::setActualSize(int value, Orientation orientation, bool force) 
{
	requestLayoutRefresh();
	if(!force)
	{
		if(value < myMinimumSize[orientation]) value = myMinimumSize[orientation];
		if(value > myMaximumSize[orientation]) value = myMaximumSize[orientation];
	}
	mySize[orientation] = value; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Widget::getId()
{
	return myId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Widget::createRenderable()
{
	return new WidgetRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WidgetRenderable::preDraw()
{
	glPushMatrix();

	// Setup transformation.
	Vector2f center = myOwner->myPosition + (myOwner->mySize / 2);
	Vector2f mcenter = -center;

	glTranslatef(center[0], center[1], 0.0f);
	glRotatef(myOwner->myRotation, 0, 0, 1);
	glTranslatef(mcenter[0], mcenter[1], 0.0f);

	glTranslatef((float)myOwner->myPosition[0], (float)myOwner->myPosition[1], 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WidgetRenderable::postDraw()
{
	// reset transform.
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WidgetRenderable::draw(RenderState* state)
{
	preDraw();
	drawContent();
	postDraw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WidgetRenderable::drawContent()
{
	if(myOwner->myDebugModeEnabled)
	{
		getRenderer()->drawRectOutline(Vector2f::Zero(), myOwner->mySize, myOwner->myDebugModeColor);
	}
}