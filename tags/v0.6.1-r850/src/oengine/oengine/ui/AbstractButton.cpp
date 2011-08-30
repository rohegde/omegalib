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
#include "oengine/ui/AbstractButton.h"

using namespace omega;
using namespace oengine;
using namespace oengine::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
AbstractButton::AbstractButton(UiManager* mng):
	Widget(mng),
	myCheckable(false),
	myChecked(false),
	myPressed(false)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
AbstractButton::~AbstractButton()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AbstractButton::update(const omega::UpdateContext& context) 
{
	if(myPressedStateChanged)
	{
		if(myPressed)
		{
			Event evt;
			evt.reset(Event::Click, Service::UI, getId());

			dispatchUIEvent(evt);

			// If button is checkable, toggle check state.
			if(myCheckable)
			{
				myChecked = !myChecked;
				Event evt;
				evt.reset(Event::Toggle, Service::UI, getId());
				dispatchUIEvent(evt);
			}
		}
		myPressedStateChanged = false;
	}
};
