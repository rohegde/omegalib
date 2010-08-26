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
#include "omega/ui/AbstractButton.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AbstractButton::AbstractButton(omega::String name):
	Widget(name),
	myCheckable(false),
	myChecked(false),
	myPressed(false)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AbstractButton::~AbstractButton()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AbstractButton::update(const omega::UpdateContext& context) 
{
	if(myPressedStateChanged)
	{
		if(myPressed)
		{
			// Create and dispatch an ui event.
			UIEvent evt(this, UIEvent::Click, &myLastEvent);
			dispatchUIEvent(evt);
		}
		myPressedStateChanged = false;
	}
};
