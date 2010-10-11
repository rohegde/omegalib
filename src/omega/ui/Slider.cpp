/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/GfxUtils.h"
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
	addChild(&myLabel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Slider::~Slider()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Slider::processInputEvent(const InputEvent& evt)
{
	Vector2f point  = evt.position;
	
	transformPoint(point);

	Vector2f sliderPos, sliderSize;
	getSliderCoords(&sliderPos, &sliderSize);

	if(!myProgressBarMode && hitTest(point, sliderPos, sliderSize))
	{
		if(evt.type == InputEvent::Down)
		{
			myPressed = true;
			myPressPos = evt.position[0];
			mySmoothValue = myValue;
		}
		else if(evt.type == InputEvent::Up)
		{
			myPressed = false;
		}
		return true;
	}
	if(hitTest(point))
	{
		if(evt.type == InputEvent::Down && myProgressBarMode)
		{
			myPressed = true;
		}
		if(evt.type == InputEvent::Up && myProgressBarMode)
		{
			myPressed = false;
		}
		if(myPressed && (evt.type == InputEvent::Move || myProgressBarMode))
		{
			if(!myProgressBarMode)
			{
				float dp = (float)(evt.position[0] - myPressPos) / (mySize[0] - mySliderSize) * (myMaxValue - myMinValue);
				mySmoothValue += dp;
				myPressPos = evt.position[0];
			}
			else
			{
				mySmoothValue = (float)(evt.position[0]) / (mySize[0]) * (myMaxValue - myMinValue);;
			}
			myValue = (int)mySmoothValue;

			if(myValue < myMinValue) myValue = myMinValue;
			if(myValue > myMaxValue) myValue = myMaxValue;

			char tst[32];
			sprintf(tst, "%d", myValue);
			String s = String(tst);
			setText(s);
		}
		return true;
	}
	else
	{
		myPressed = false;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::update(const omega::UpdateContext& context) 
{
	float slSize = mySize[0] / (float(myMaxValue - myMinValue));
	mySliderSize = slSize > 20 ? slSize : 20;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::draw()
{
	Widget::draw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Slider::getSliderCoords(Vector2f* pos, Vector2f* size)
{
	float dv = myMaxValue - myMinValue;

	float startW = (float)(myValue - myMinValue) / dv;

	*pos = myPosition;
	*size = mySize;
	if(myProgressBarMode)
	{
		size->x() = startW * mySize[0];
	}
	else
	{
		pos->x() += startW * (mySize[0] - mySliderSize);
		size->x() = mySliderSize;
	}
}
