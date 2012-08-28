/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omegaToolkit/UiModule.h"
#include "omegaToolkit/ui/Widget.h"
#include "omegaToolkit/ui/Container.h"
//#include "omegaToolkit/ui/DefaultSkin.h"
#include "omega/DrawInterface.h"
#include "omega/EventSharingModule.h"

#include "omega/glheaders.h"

#include <float.h>

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

NameGenerator Widget::mysNameGenerator("Widget_");

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget::Widget(Engine* server):
	myStereo(false),
	myInitialized(false),
	myServer(server),
	myEventHandler(NULL),
	myContainer(NULL),
	myVisible(true),
	myEnabled(true),
	myDebugModeColor(255, 0, 255),
	myDebugModeEnabled(false),
	myAutosize(false),
	myRotation(0),
	myUserMoveEnabled(false),
	myMoving(false),
	myMaximumSize(FLT_MAX, FLT_MAX),
	myMinimumSize(0, 0),
	myActive(false),
	myHorizontalNextWidget(NULL),
	myHorizontalPrevWidget(NULL),
	myVerticalNextWidget(NULL),
	myVerticalPrevWidget(NULL),
	mySize(Vector2f::Zero()),
	myPosition(Vector2f::Zero())
{
	myId = mysNameGenerator.getNext();
	myName = mysNameGenerator.generate();
	UiModule::instance()->myWidgets[myId] = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget::~Widget()
{
	if(myInitialized)
	{
		dispose();
	}
	UiModule::instance()->myWidgets[myId] = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::isGamepadInteractionEnabled()
{
	return UiModule::instance()->getGamepadInteractionEnabled();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::isPointerInteractionEnabled()
{
	return UiModule::instance()->getPointerInteractionEnabled();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::update(const omega::UpdateContext& context) 
{
	if(!myInitialized)
	{
		//ofmsg("Initializing Widget: %1%", %getName());
		initialize(myServer);
		myInitialized = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::handleEvent(const Event& evt) 
{
	UiModule* ui = UiModule::instance();
	if(isGamepadInteractionEnabled())
	{
		if(myActive)
		{
			if(evt.isButtonDown(Event::ButtonDown))
			{
				evt.setProcessed();
				if(myVerticalNextWidget != NULL) 
					ui->activateWidget(myVerticalNextWidget);
			}
			else if(evt.isButtonDown(Event::ButtonUp))
			{
				evt.setProcessed();
				if(myVerticalPrevWidget != NULL) 
					ui->activateWidget(myVerticalPrevWidget);
			}
			else if(evt.isButtonDown(Event::ButtonLeft))
			{
				evt.setProcessed();
				if(myHorizontalPrevWidget != NULL) 
					ui->activateWidget(myHorizontalPrevWidget);
			}
			else if(evt.isButtonDown(Event::ButtonRight))
			{
				evt.setProcessed();
				if(myHorizontalNextWidget != NULL) 
					ui->activateWidget(myHorizontalNextWidget);
			}
		}
	}

	if(isPointerInteractionEnabled())
	{
		Vector2f pos2d = Vector2f(evt.getPosition().x(), evt.getPosition().y());
		if(myActive)
		{
			//if(evt.getType() == Event::Zoom)
			//{
			//	float val = evt.getExtraDataInt(0);
			//	if(val != 0)
			//	{
			//		float sc = 0.0f;
			//		if(val < 0) sc = 0.9f;
			//		else sc = 1.1f;

			//		float width = mySize[0] * sc;
			//		float height = mySize[1] * sc;

			//		setSize(Vector2f(width, height));
			//	}
			//}
		}
		if(hitTest(transformPoint(pos2d)))
		{
			if(!evt.isProcessed())
			{
				// Some kind of pointer event happened over this widget: make it active.
				if(!isActive())
				{
					ui->activateWidget(this);
				}

				if(myUserMoveEnabled)
				{
					if(evt.getType() == Event::Down)
					{
						myUserMovePosition = pos2d;
						evt.setProcessed();
						myMoving = true;
						myActive = true;
					}
					else if(evt.getType() == Event::Move && myMoving)
					{
						Vector2f delta = pos2d - myUserMovePosition;
						myUserMovePosition = pos2d;

						myPosition += delta;
						evt.setProcessed();
					}
					else if(myMoving && evt.getType() == Event::Up)
					{
						myMoving = false;
						myActive = false;
						evt.setProcessed();
					}
				}
			}
		}
		else
		{
			myMoving = false;
			//myActive = false;
		}
	}
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
	if(myEventHandler != NULL) 
	{
		// If the local events option is set, mark ui events as local before dispatching them to
		// the event listener. Local events do not get broadcast to other nodes in clustered systems.
		if(UiModule::instance()->isLocalEventsEnabled())
		{
			EventSharingModule::markLocal(evt);
		}
		myEventHandler->handleEvent(evt);
	}
	else if(myContainer != NULL) 
	{
			myContainer->dispatchUIEvent(evt);
	}
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
void Widget::updateSize(Renderer* r)
{
	if(myAutosize) autosize(r);
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
	myRenderState = state;

	if(myOwner->isStereo())
	{
		if(state->context->eye != DrawContext::EyeCyclop) 
		{
			preDraw();
			drawContent();
			postDraw();
		}
	}
	else
	{
		if(state->context->eye == DrawContext::EyeCyclop) 
		{
			preDraw();
			drawContent();
			postDraw();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WidgetRenderable::drawContent()
{
	if(myOwner->myDebugModeEnabled)
	{
		getRenderer()->drawRectOutline(Vector2f::Zero(), myOwner->mySize, myOwner->myDebugModeColor);
	}
}
