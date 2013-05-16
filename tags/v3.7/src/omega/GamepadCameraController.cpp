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
#include "omega/Camera.h"
#include "omega/GamepadCameraController.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
GamepadCameraController::GamepadCameraController():
	mySpeed(2.0f),
	myStrafeMultiplier(1.0f),
	myYawMultiplier(0.002f),
	myPitchMultiplier(0.002f)
{
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////
void GamepadCameraController::handleEvent(const Event& evt)
{
	if(!isEnabled() || evt.isProcessed()) return;
	if(evt.getServiceType() == Service::Controller)
	{
		float n = 1000;
		float x = evt.getExtraDataFloat(1) / n;
		float y = evt.getExtraDataFloat(2) / n;
		float z = evt.getExtraDataFloat(5) / n;
		float yaw = evt.getExtraDataFloat(3) / n;
		float pitch = evt.getExtraDataFloat(4) / n;
		float tresh = 0.2f;

		if(x < tresh && x > -tresh) x = 0;
		if(y < tresh && y > -tresh) y = 0;
		if(z < tresh && z > -tresh) z = 0;
			
		if(myYaw < tresh && myYaw > -tresh) myYaw = 0;
		if(myPitch < tresh && myPitch > -tresh) myPitch = 0;

		mySpeedVector = Vector3f(x, y, z) * mySpeed;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void GamepadCameraController::update(const UpdateContext& context)
{
	if(!isEnabled()) return;
	//updateCamera(mySpeedVector, myYaw, myPitch, 0, context.dt);
}

