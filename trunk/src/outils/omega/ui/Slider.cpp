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
#include "omega/ui/Slider.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Slider::Slider(omega::String name):
	Widget(name),
	myLabel(name + ".label"),
	myMinValue(0),
	myMaxValue(100),
	myValue(0),
	mySliderSize(20),
	myProgressBarMode(false)
{
	//addChild(&myLabel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Slider::~Slider()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Slider::processInputEvent(const Event& evt)
{
	//Vector2f point  = evt.position;
	//
	//transformPoint(point);

	//Vector2f sliderPos, sliderSize;
	//getSliderCoords(&sliderPos, &sliderSize);

	//if(!myProgressBarMode && hitTest(point, sliderPos, sliderSize))
	//{
	//	if(evt.type == Event::Down)
	//	{
	//		myPressed = true;
	//		myPressPos = evt.position[0];
	//		mySmoothValue = myValue;
	//	}
	//	else if(evt.type == Event::Up)
	//	{
	//		myPressed = false;
	//	}
	//	return true;
	//}
	//if(hitTest(point))
	//{
	//	if(evt.type == Event::Down && myProgressBarMode)
	//	{
	//		myPressed = true;
	//	}
	//	if(evt.type == Event::Up && myProgressBarMode)
	//	{
	//		myPressed = false;
	//	}
	//	if(myPressed && (evt.type == Event::Move || myProgressBarMode))
	//	{
	//		if(!myProgressBarMode)
	//		{
	//			float dp = (float)(evt.position[0] - myPressPos) / (mySize[0] - mySliderSize) * (myMaxValue - myMinValue);
	//			mySmoothValue += dp;
	//			myPressPos = evt.position[0];
	//		}
	//		else
	//		{
	//			mySmoothValue = (float)(evt.position[0]) / (mySize[0]) * (myMaxValue - myMinValue);;
	//		}
	//		myValue = (int)mySmoothValue;

	//		if(myValue < myMinValue) myValue = myMinValue;
	//		if(myValue > myMaxValue) myValue = myMaxValue;

	//		char tst[32];
	//		sprintf(tst, "%d", myValue);
	//		String s = String(tst);
	//		setText(s);
	//	}
	//	return true;
	//}
	//else
	//{
	//	myPressed = false;
	//}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::update(const omega::UpdateContext& context) 
{
	//float slSize = mySize[0] / (float(myMaxValue - myMinValue));
	//mySliderSize = slSize > 20 ? slSize : 20;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::draw()
{
	Widget::draw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::getSliderCoords(Vector2f* pos, Vector2f* size)
{
	//float dv = myMaxValue - myMinValue;

	//float startW = (float)(myValue - myMinValue) / dv;

	//*pos = myPosition;
	//*size = mySize;
	//if(myProgressBarMode)
	//{
	//	size->x() = startW * mySize[0];
	//}
	//else
	//{
	//	pos->x() += startW * (mySize[0] - mySliderSize);
	//	size->x() = mySliderSize;
	//}
}
