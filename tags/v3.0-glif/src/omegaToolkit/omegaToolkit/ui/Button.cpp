/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omegaToolkit/ui/Button.h"
#include "omegaToolkit/UiModule.h"
//#include "omegaToolkit/ui/DefaultSkin.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Button::Button(Engine* srv):
	AbstractButton(srv),
	myLabel(srv)
{
	//addChild(&myLabel);
	//myLabel.setText(name);
	setMaximumHeight(22);
	myColor = Color(0.2f, 0.2f, 0.2f);
	setAutosize(true);
	//setDebugModeEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Button::~Button()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Button::autosize(Renderer* r)
{
	myLabel.autosize(r);
	// HACK: we add some default margin to the label.
	Vector2f size = myLabel.getSize();
	if(myCheckable)
	{
		size += Vector2f(20, 4);
	}
	else
	{
		size += Vector2f(10, 4);
	}

	setSize(size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Button::update(const omega::UpdateContext& context)
{
	AbstractButton::update(context);
	myLabel.update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Button::handleEvent(const Event& evt)
{
	if(isPointerInteractionEnabled())
	{
		Vector2f point  = Vector2f(evt.getPosition().x(), evt.getPosition().y());
		point = transformPoint(point);
		if(hitTest(point))
		{
			if(evt.getType() == Event::Down)		
			{
				myPressed = true;
				myPressedStateChanged = true;
				evt.setProcessed();
			}
			else if(evt.getType() == Event::Up)
			{
				myPressed = false;
				myPressedStateChanged = true;
				evt.setProcessed();
			}
		}
	}
	if(!evt.isProcessed() && isGamepadInteractionEnabled())
	{
		if(isActive())
		{
			if(evt.isButtonDown(UiModule::getConfirmButton()))
			{
				myPressed = false;
				myPressedStateChanged = true;
			}
		}
	}
	AbstractButton::handleEvent(evt);
}

