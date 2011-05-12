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
#include "omega/ui/Slider.h"
#include "omega/StringUtils.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Slider::Slider(omega::String name):
	Widget(name),
	myTicks(100),
	myValue(0),
	myDeferUpdate(false),
	myValueChanged(false)
{
	setMaximumHeight(22);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Slider::~Slider()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Slider::processInputEvent(const Event& evt)
{
	Vector2f point = Vector2f(evt.position[0], evt.position[1]);
	
	point = transformPoint(point);

	Vector2f sliderPos = getSliderPosition();
	Vector2f sliderSize = getSliderSize();

	if(evt.type == Event::Up)
	{
		myPressed = false;
		if(myValueChanged)
		{
			UIEvent evt = UIEvent(this, UIEvent::ValueChange);
			dispatchUIEvent(evt);
		}
	}

	if(hitTest(point, sliderPos, sliderSize))
	{
		if(evt.type == Event::Down)
		{
			myPressed = true;
			myPressPos = evt.position[0];
		}
		return true;
	}
	if(hitTest(point))
	{
		if(myPressed && evt.type == Event::Move)
		{
			int newValue = (point[0] + sliderSize[0] / 2) * myTicks / mySize[0]; 
			if(newValue < 0) newValue = 0;
			if(newValue > (myTicks - 1)) newValue = myTicks - 1;

			if(newValue != myValue)
			{
				myValue = newValue;
				if(!myDeferUpdate)
				{
					dispatchUIEvent(UIEvent(this, UIEvent::ValueChange));
				}
				else
				{
					myValueChanged = true;
				}
			}
		}
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::update(const omega::UpdateContext& context) 
{
	//float slSize = mySize[0] / (float(myMaxValue - myMinValue));
	//mySliderSize = slSize > 20 ? slSize : 20;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f Slider::getSliderSize()
{
	//return Vector2f(mySize[0] / myTicks, mySize[1] + 10);
	return Vector2f(20, mySize[1] - 4);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f Slider::getSliderPosition()
{
	Vector2f position;
	Vector2f size = getSliderSize();

	position[0] = (myValue * mySize[0] / (myTicks - 1)) - size[0] / 2;
	position[1] = -(size[1] - mySize[1]) / 2;
	return position;
}
