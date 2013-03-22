/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omegaToolkit/ui/Slider.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Slider::Slider(Engine* srv):
	Widget(srv),
	myTicks(100),
	myValue(0),
	myDeferUpdate(false),
	myValueChanged(false)
{
	setMaximumHeight(22);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Slider::~Slider()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::handleEvent(const Event& evt)
{
	if(isPointerInteractionEnabled())
	{
		Vector2f point = Vector2f(evt.getPosition().x(), evt.getPosition().y());
	
		point = transformPoint(point);

		Vector2f sliderPos = getSliderPosition();
		Vector2f sliderSize = getSliderSize();

		if(evt.getType() == Event::Up)
		{
			myPressed = false;
			if(myValueChanged)
			{
				Event e;
				e.reset(Event::ChangeValue, Service::Ui, getId());
				dispatchUIEvent(e);
			}
		}

		if(simpleHitTest(point, sliderPos, sliderSize))
		{
			if(evt.getType() == Event::Down)
			{
				myPressed = true;
				myPressPos = evt.getPosition().x();
			}
			evt.setProcessed();
		}
		if(simpleHitTest(point))
		{
			if(myPressed && evt.getType() == Event::Move)
			{
				int newValue = (point[0] + sliderSize[0] / 2) * myTicks / mySize[0]; 
				if(newValue < 0) newValue = 0;
				if(newValue > (myTicks - 1)) newValue = myTicks - 1;

				if(newValue != myValue)
				{
					myValue = newValue;
					if(!myDeferUpdate)
					{
						Event e;
						e.reset(Event::ChangeValue, Service::Ui, getId());
						dispatchUIEvent(evt);
					}
					else
					{
						myValueChanged = true;
					}
				}
			}
			evt.setProcessed();
		}
	}
	if(isGamepadInteractionEnabled())
	{
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::update(const omega::UpdateContext& context) 
{
	Widget::update(context);
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
