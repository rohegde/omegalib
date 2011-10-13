/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "oengine/NavigationInteractor.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
NavigationInteractor::NavigationInteractor():
	mySpeed(1.0f),
	myMoveLeft(false),
	myMoveRight(false),
	myMoveUp(false),
	myMoveDown(false)
{
	//myPosition = Vector3f::Zero();
	myVelocity = Vector3f::Zero();
	myOrientation = Quaternion::Identity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NavigationInteractor::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Keyboard)
	{
		if(evt.isKeyDown('d')) myMoveRight = true;
		if(evt.isKeyUp('d')) myMoveRight = false;
		if(evt.isKeyDown('a')) myMoveLeft = true;
		if(evt.isKeyUp('a')) myMoveLeft = false;
		if(evt.isKeyDown('w')) myMoveUp = true;
		if(evt.isKeyUp('w')) myMoveUp = false;
		if(evt.isKeyDown('s')) myMoveDown = true;
		if(evt.isKeyUp('s')) myMoveDown = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NavigationInteractor::update(const UpdateContext& context)
{
	// Update the observer position offset using current speed, orientation and dt.
	if(myMoveRight) myVelocity = Vector3f(mySpeed, 0, 0);
	else if(myMoveLeft) myVelocity = Vector3f(-mySpeed, 0, 0);
	else if(myMoveUp) myVelocity = Vector3f(0, 0, mySpeed);
	else if(myMoveDown) myVelocity = Vector3f(0, 0, -mySpeed);
	else myVelocity = myVelocity * (0.1f * context.dt);

	if(myNode != NULL)
	{
		Vector3f pos = myNode->getPosition();
		Quaternion orientation = myNode->getOrientation();

		pos += myVelocity * context.dt;

		myNode->setPosition(pos);
		myNode->setOrientation(orientation);
	}
}
