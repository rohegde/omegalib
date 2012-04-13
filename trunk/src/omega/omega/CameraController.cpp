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
#include "omega/CameraController.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraController::updateCamera(const Vector3f& speed, float yaw, float pitch, float roll, float dt)
{
	if(myCamera != NULL)
	{
		//Quaternion navOrientation = AngleAxis(roll, Vector3f::UnitZ()) * AngleAxis(-yaw, Vector3f::UnitY()) * AngleAxis(-pitch, Vector3f::UnitX());
		Quaternion orientation =   AngleAxis(yaw, Vector3f::UnitY()) * AngleAxis(pitch, Vector3f::UnitX()) * AngleAxis(roll, Vector3f::UnitZ());
		orientation = myOriginalOrientation * orientation;
		//navOrientation = navOrientation * myOriginalOrientation.inverse();
		Vector3f ns = orientation * speed;
		Vector3f position = myCamera->getPosition() + (ns * dt);
		myCamera->setPosition(position);
		myCamera->setOrientation(orientation , false);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f CameraController::computeSpeedVector(uint moveFlags, float speed, float strafeMultiplier)
{
	Vector3f vSpeed = Vector3f::Zero();
	// Update the observer position offset using current speed, orientation and dt.
	if(moveFlags & MoveRight)    vSpeed += Vector3f(speed * strafeMultiplier, 0, 0);
	if(moveFlags & MoveLeft)     vSpeed += Vector3f(-speed * strafeMultiplier, 0, 0);
	if(moveFlags & MoveUp)       vSpeed += Vector3f(0, speed, 0);
	if(moveFlags & MoveDown)     vSpeed += Vector3f(0, -speed, 0);
	if(moveFlags & MoveForward)  vSpeed += Vector3f(0, 0, -speed);
	if(moveFlags & MoveBackward) vSpeed += Vector3f(0, 0, speed);
	return vSpeed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
KeyboardMouseCameraController::KeyboardMouseCameraController():
	mySpeed(2.0f),
	myStrafeMultiplier(1.0f),
	myYawMultiplier(0.002f),
	myPitchMultiplier(0.002f),
	myYaw(0),
	myPitch(0),
	myMoveFlags(0)
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
}

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
	updateCamera(mySpeedVector, myYaw, myPitch, 0, context.dt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
WandCameraController::WandCameraController():
	mySpeed(2.0f),
	myStrafeMultiplier(1.0f),
	myYawMultiplier(1.00f),
	myPitchMultiplier(-1.0f),
	myYaw(0),
	myPitch(0),
	myMoveFlags(0)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraController::reset()
{
	if( getCamera() != NULL )
	{
		myOriginalOrientation = getCamera()->getOrientation();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void WandCameraController::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Wand)
	{
		myMoveFlags = evt.getFlags();
		if(evt.isFlagSet(Event::Left)) myRotating = true;
		else myRotating = false;
			
	
		if(myRotating)
		{
			Vector3f dpos = evt.getPosition() - myLastPointerPosition;
			ofmsg("pos %1%, dpos: %2%", %evt.getPosition() %dpos);
			myYaw += dpos.x() * myYawMultiplier;
			myPitch += dpos.y() * myPitchMultiplier;
		}
		myLastPointerPosition = evt.getPosition();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WandCameraController::update(const UpdateContext& context)
{
	Vector3f speed = computeSpeedVector(myMoveFlags, mySpeed, myStrafeMultiplier);
	updateCamera(speed, myYaw, myPitch, 0, context.dt);
}

