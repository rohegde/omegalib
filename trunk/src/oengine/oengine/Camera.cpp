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
#include "omega/RenderTarget.h"
#include "oengine/Camera.h"
#include "omega/StringUtils.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera::Camera():
	myAutoAspect(false),
	myTargetNode(NULL),
	myNavigationMode(NavDisabled),
	myNavigationSpeed(4.0f),
	myNavigationStrafeMultiplier(1.0f),
	myNavigationYawMultiplier(0.002f),
	myNavigationPitchMultiplier(0.002f),
	myNavigationMoveFlags(0),
	myPosition(Vector3f::Zero()),
	myScale(1.0),
	myOrientation(Quaternion::Identity())
{
	myYaw = 0;
	myPitch = 0;
	myRotating = false;
	myLastPointerPosition = Vector3f::Zero();

	// Set a standard focal offset of 1 meter away from camera.
	myFocalOffset = Vector3f::UnitZ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::handleEvent(const Event& evt)
{
	if(myNavigationMode == NavFreeFly)
	{
		if(evt.getServiceType() == Service::Keyboard)
		{
			if(evt.isKeyDown('d')) myNavigationMoveFlags |= MoveRight;
			if(evt.isKeyUp('d')) myNavigationMoveFlags &= ~MoveRight; 
			if(evt.isKeyDown('a')) myNavigationMoveFlags |= MoveLeft;
			if(evt.isKeyUp('a')) myNavigationMoveFlags &= ~MoveLeft;
			if(evt.isKeyDown('w')) myNavigationMoveFlags |= MoveForward;
			if(evt.isKeyUp('w')) myNavigationMoveFlags &= !MoveForward;
			if(evt.isKeyDown('s')) myNavigationMoveFlags |= MoveBackward;
			if(evt.isKeyUp('s')) myNavigationMoveFlags &= ~MoveBackward;
			if(evt.isKeyDown('r')) myNavigationMoveFlags |= MoveUp;
			if(evt.isKeyUp('r')) myNavigationMoveFlags &= ~MoveUp;
			if(evt.isKeyDown('f')) myNavigationMoveFlags |= MoveDown;
			if(evt.isKeyUp('f')) myNavigationMoveFlags &= ~MoveDown;
		}
		else if(evt.getServiceType() == Service::Pointer)
		{
			if(evt.isFlagSet(Event::Left)) myRotating = true;
			else myRotating = false;
			
			if(myRotating)
			{
				Vector3f dpos = evt.getPosition() - myLastPointerPosition;
				myYaw += dpos.x() * myNavigationYawMultiplier;
				myPitch += dpos.y() * myNavigationPitchMultiplier;
			}
			myLastPointerPosition = evt.getPosition();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::update(const UpdateContext& context)
{
	//if(myAutoAspect)
	//{
	//	Rect viewport = Rect(0, 0, myRenderTarget->getWidth(), myRenderTarget->getHeight());
	//	float aspect = (float)viewport.width() / viewport.height();
	//	updateProjection(myFov, aspect, myNearZ, myFarZ);
	//}

	if(myNavigationMode == NavFreeFly)
	{
		Vector3f speed = Vector3f::Zero();
		// Update the observer position offset using current speed, orientation and dt.
		if(myNavigationMoveFlags & MoveRight) speed += Vector3f(-myNavigationSpeed * myNavigationStrafeMultiplier, 0, 0);
		if(myNavigationMoveFlags & MoveLeft) speed += Vector3f(myNavigationSpeed * myNavigationStrafeMultiplier, 0, 0);
		if(myNavigationMoveFlags & MoveUp) speed += Vector3f(0, -myNavigationSpeed, 0);
		if(myNavigationMoveFlags & MoveDown) speed += Vector3f(0, myNavigationSpeed, 0);
		if(myNavigationMoveFlags & MoveForward) speed += Vector3f(0, 0, myNavigationSpeed);
		if(myNavigationMoveFlags & MoveBackward) speed += Vector3f(0, 0, -myNavigationSpeed);

		Quaternion navOrientation =  AngleAxis(-Math::Pi, Vector3f::UnitZ()) * AngleAxis(-myYaw, Vector3f::UnitY()) * AngleAxis(-myPitch, Vector3f::UnitX());
		myOrientation =   AngleAxis(myPitch, Vector3f::UnitX()) * AngleAxis(myYaw, Vector3f::UnitY()) * AngleAxis(-Math::Pi, Vector3f::UnitZ());

		Vector3f ns = navOrientation * speed;
		//ofmsg("|dt: %1%   t %2%", %context.dt %context.time);

		myPosition += ns * context.dt ;

	}
	else if(myNavigationMode == NavFollow && myTargetNode != NULL)
	{
		myPosition = myTargetNode->getPosition();
		myOrientation = myTargetNode->getOrientation();
	}
	//ofmsg("@Camera position: %1% orientation %2% %3%", %myPosition %myYaw %myPitch);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::focusOn(SceneNode* node, float scaledSize)
{
	const Sphere& bs = node->getBoundingSphere();
	myOrientation = Quaternion::Identity();
	myPosition = bs.getCenter();
	if(scaledSize != 0)
	{
		myScale = scaledSize / bs.getRadius();
	}
	else
	{
		myScale = 1.0f;
	}
	ofmsg("|Camera focus changed: position %1%  scale %2%", %myPosition %myScale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//void Camera::updateView(const Vector3f& position, const Quaternion& orientation)
//{
//	myPosition = position;
//	myOrientation = orientation;
//	myView = Math::makeViewMatrix(position, orientation);
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//void Camera::updateProjection(float fov, float aspect, float nearZ, float farZ)
//{
//	myProjection = Math::makePerspectiveMatrix(fov, aspect, nearZ, farZ);
//	myFov = fov;
//	myNearZ = nearZ;
//	myFarZ = farZ;
//	myAspect = aspect;
//}
