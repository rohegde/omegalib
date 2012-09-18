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
#include "omega/Camera.h"
#include "omega/KeyboardMouseCameraController.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
KeyboardMouseCameraController::KeyboardMouseCameraController():
	mySpeed(2.0f),
	myStrafeMultiplier(1.0f),
	myYawMultiplier(0.002f),
	myPitchMultiplier(0.002f),
	myYaw(0),
	myPitch(0),
	myMoveFlags(0),
	myRotating(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#define NAV_KEY(k, f) if(evt.isKeyDown(k)) myMoveFlags |= f; if(evt.isKeyUp(k)) myMoveFlags &= ~f; 
void KeyboardMouseCameraController::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Keyboard)
	{
		NAV_KEY('d', MoveRight);
		NAV_KEY('a', MoveLeft);
		NAV_KEY('w', MoveForward);
		NAV_KEY('s', MoveBackward);
		NAV_KEY('r', MoveUp);
		NAV_KEY('f', MoveDown);
	}
	else if(evt.getServiceType() == Service::Pointer)
	{
		if(evt.isFlagSet(Event::Left)) myRotating = true;
		else myRotating = false;
			
		if(myRotating)
		{
			Vector3f dpos = evt.getPosition() - myLastPointerPosition;
			myYaw -= dpos.x() * myYawMultiplier;
			myPitch -= dpos.y() * myPitchMultiplier;
		}
		myLastPointerPosition = evt.getPosition();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void KeyboardMouseCameraController::update(const UpdateContext& context)
{
	Vector3f speed = computeSpeedVector(myMoveFlags, mySpeed, myStrafeMultiplier);
	updateCamera(speed, myYaw, myPitch, 0, context.dt);
	reset();
	myYaw = 0;
	myPitch = 0;
}

