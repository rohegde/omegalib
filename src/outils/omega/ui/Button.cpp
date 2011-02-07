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
#include "omega/ui/Button.h"
#include "omega/ui/DefaultSkin.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Button::Button(omega::String name):
	AbstractButton(name),
	myLabel(name + ".label")
{
	//addChild(&myLabel);
	myLabel.setText(name);
	setMaximumHeight(22);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Button::~Button()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Button::autosize()
{
	myLabel.autosize();
	setSize(myLabel.getSize());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Button::update(const omega::UpdateContext& context)
{
	// Make sure label ui manager is set.
	myLabel.setUIManager(getUIManager());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Button::processInputEvent(const InputEvent& evt)
{
	Vector2f point  = evt.position;
	
	transformPoint(point);

	if(hitTest(point))
	{
		if(evt.type == InputEvent::Down)
		{
			myPressed = true;
			myPressedStateChanged = true;
		}
		else if(evt.type == InputEvent::Up)
		{
			myPressed = false;
			myPressedStateChanged = true;
		}
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::renderContent()
{
	AbstractButton::renderContent();
	DefaultWidgetFactory::fillRect(Vector2i::ZERO, getSize(), Color(1.0f, 1.0f, 1.0f, 0.2f));
	DefaultWidgetFactory::outlineRect(Vector2i::ZERO, getSize(), Color(1.0f, 1.0f, 1.0f, 1.0f));

	myLabel.setPosition(Vector2i::ZERO);
	myLabel.setSize(getSize());
	myLabel.draw();
}
