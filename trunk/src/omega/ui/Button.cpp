/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/GfxUtils.h"
#include "omega/ui/Button.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Button::Button(omega::String name):
	AbstractButton(name),
	myLabel(name + ".label")
{
	addChild(&myLabel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Button::~Button()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
void Button::draw()
{
	myLabel.setPosition(myPosition);
	myLabel.setSize(mySize);

	AbstractButton::draw();
}
