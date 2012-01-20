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
#include "omega/Camera.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::setReadbackTarget(PixelData* color, PixelData* depth)
{
	myReadbackColorTarget = color;
	myReadbackDepthTarget = depth;
	if(myReadbackColorTarget != NULL)
	{
		myReadbackViewport = Rect(
			0, 0, 
			myReadbackColorTarget->getWidth(), myReadbackColorTarget->getHeight());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::setReadbackTarget(PixelData* color, PixelData* depth, const Rect& readbackViewport)
{
	setReadbackTarget(color, depth);
	myReadbackViewport = readbackViewport;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::beginDraw(const DrawContext& context)
{
	if(myRenderTarget == NULL)
	{
		if(myOffscreen) 
		{
			myRenderTarget = new RenderTarget(context.gpuContext, RenderTarget::RenderOffscreen);
		}
		else
		{
			myRenderTarget = new RenderTarget(context.gpuContext, RenderTarget::RenderOnscreen);
		}
	}
	myRenderTarget->setReadbackTarget(myReadbackColorTarget, myReadbackDepthTarget, myReadbackViewport);
	myRenderTarget->bind();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::endDraw(const DrawContext& context)
{
	if(myRenderTarget != NULL)
	{
		// HACK
		myRenderTarget->readback();

		myRenderTarget->unbind();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::startFrame(const FrameInfo& frame)
{
	if(myRenderTarget != NULL)
	{
		myRenderTarget->clear();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::finishFrame(const FrameInfo& frame)
{
	if(myRenderTarget != NULL)
	{
		// HACK
		//myRenderTarget->readback();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera::Camera(uint flags):
	myAutoAspect(false),
	myFlags(flags),
	myTargetNode(NULL),
	myNavigationMode(NavDisabled),
	myNavigationSpeed(2.0f),
	myNavigationStrafeMultiplier(1.0f),
	myNavigationYawMultiplier(0.002f),
	myNavigationPitchMultiplier(0.002f),
	myNavigationMoveFlags(0),
	myPosition(Vector3f::Zero()),
	myOrientation(Quaternion::Identity())
{
	myYaw = 0;
	myPitch = 0;
	myRotating = false;
	myLastPointerPosition = Vector3f::Zero();
	myControllerMoveVector = Vector3f::Zero();
	myControllerRotateVector = Vector2f::Zero();

	// Set a standard focal offset of 1 meter away from camera.
	myProjectionOffset = -Vector3f::UnitZ();

	for(int i = 0; i < GpuContext::MaxContexts; i++)
	{
		myOutput[i] = new CameraOutput(isOffscreen());
	}
}

#define NAV_KEY(k, f) if(evt.isKeyDown(k)) myNavigationMoveFlags |= f; if(evt.isKeyUp(k)) myNavigationMoveFlags &= ~f; 

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::handleEvent(const Event& evt)
{
	if(myNavigationMode == NavFreeFly)
	{
		if(evt.getServiceType() == Service::Keyboard)
		{
			if(evt.isFlagSet(Event::Alt))
			{
				// Control focus position
				NAV_KEY('d', FocusRight);
				NAV_KEY('a', FocusLeft);
				NAV_KEY('w', FocusForward);
				NAV_KEY('s', FocusBackward);
				NAV_KEY('r', FocusUp);
				NAV_KEY('f', FocusDown);
			}
			else
			{
				NAV_KEY('d', MoveRight);
				NAV_KEY('a', MoveLeft);
				NAV_KEY('w', MoveForward);
				NAV_KEY('s', MoveBackward);
				NAV_KEY('r', MoveUp);
				NAV_KEY('f', MoveDown);
			}
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
		else if(evt.getServiceType() == Service::Controller)
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
			if(yaw < tresh && yaw > -tresh) yaw = 0;
			if(pitch < tresh && pitch > -tresh) pitch = 0;

			myControllerMoveVector = Vector3f(x, y, z);
			myControllerRotateVector = Vector2f(yaw, pitch);

			//ofmsg("%1% %2% %3% %4%", %x %y %yaw %pitch);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::update(const UpdateContext& context)
{
	if(myNavigationMode == NavFreeFly)
	{
		Vector3f speed = Vector3f::Zero();
		Vector3f focusSpeed = Vector3f::Zero();
		// Update the observer position offset using current speed, orientation and dt.
		if(myNavigationMoveFlags & MoveRight) speed += Vector3f(myNavigationSpeed * myNavigationStrafeMultiplier, 0, 0);
		if(myNavigationMoveFlags & MoveLeft) speed += Vector3f(-myNavigationSpeed * myNavigationStrafeMultiplier, 0, 0);
		if(myNavigationMoveFlags & MoveUp) speed += Vector3f(0, myNavigationSpeed, 0);
		if(myNavigationMoveFlags & MoveDown) speed += Vector3f(0, -myNavigationSpeed, 0);
		if(myNavigationMoveFlags & MoveForward) speed += Vector3f(0, 0, -myNavigationSpeed);
		if(myNavigationMoveFlags & MoveBackward) speed += Vector3f(0, 0, myNavigationSpeed);

		speed[2] += myControllerMoveVector[1] * myNavigationSpeed;
		speed[1] += myControllerMoveVector[2] * myNavigationSpeed;
		speed[0] += myControllerMoveVector[0] * myNavigationSpeed;

		myYaw += myControllerRotateVector[0] * context.dt;
		myPitch += myControllerRotateVector[1] * context.dt;

		myControllerMoveVector *= 0.5f;
		myControllerRotateVector *= 0.5f;

		// Update the observer position offset using current speed, orientation and dt.
		float fs = 0.1f;
		if(myNavigationMoveFlags & FocusRight) focusSpeed += Vector3f(-fs, 0, 0);
		if(myNavigationMoveFlags & FocusLeft) focusSpeed += Vector3f(fs, 0, 0);
		if(myNavigationMoveFlags & FocusUp) focusSpeed += Vector3f(0, -fs, 0);
		if(myNavigationMoveFlags & FocusDown) focusSpeed += Vector3f(0, fs, 0);
		if(myNavigationMoveFlags & FocusForward) focusSpeed += Vector3f(0, 0, fs);
		if(myNavigationMoveFlags & FocusBackward) focusSpeed += Vector3f(0, 0, -fs);

		myProjectionOffset += focusSpeed * context.dt;

		Quaternion navOrientation =  AngleAxis(0, Vector3f::UnitZ()) * AngleAxis(-myYaw, Vector3f::UnitY()) * AngleAxis(-myPitch, Vector3f::UnitX());
		myOrientation =   AngleAxis(myPitch, Vector3f::UnitX()) * AngleAxis(myYaw, Vector3f::UnitY()) * AngleAxis(0, Vector3f::UnitZ());

		Vector3f ns = navOrientation * speed;

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
void Camera::focusOn(SceneNode* node)
{
	const Sphere& bs = node->getBoundingSphere();
	myOrientation = Quaternion::Identity();
	myPosition = bs.getCenter() + Vector3f(0, 0, bs.getRadius());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::updateObserver(Observer* obs)
{
	obs->updateHead(-myProjectionOffset, Quaternion::Identity());
	obs->updateView(myPosition + myProjectionOffset, myOrientation);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
CameraOutput* Camera::getOutput(uint contextId)
{
	oassert(contextId < GpuContext::MaxContexts);
	return myOutput[contextId];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Camera::isEnabled(const DrawContext& context)
{
	CameraOutput* output = getOutput(context.gpuContext->getId());
	if(!output->isEnabled()) return false;
	if((myFlags & DrawScene) && context.task == DrawContext::SceneDrawTask)
	{
		// When forcing mono rendering, we only render cyclops eye or left eye 
		// (hyjiacking it to cyclops), so skip right eye rendering.
		if((myFlags & ForceMono) && context.eye == DrawContext::EyeRight) return false;
		return true;
	}
	if((myFlags & DrawOverlay) && context.task == DrawContext::OverlayDrawTask) return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
const DrawContext& Camera::beginDraw(const DrawContext& context)
{
	CameraOutput* output = getOutput(context.gpuContext->getId());
	DrawContext& dc = myDrawContext[context.gpuContext->getId()];

	dc = context;
	dc.modelview = myModelView;
	dc.viewport = output->getReadbackViewport();
	if(myAutoAspect)
	{
		float aspect = (float)dc.viewport.width() / dc.viewport.height();
		setProjection(myFov, aspect, myNearZ, myFarZ);
	}
	dc.projection = myProjection;

	output->beginDraw(dc);
	if(myFlags & Offscreen)
	{
		dc.drawBuffer = output->getRenderTarget();
	}
	return dc;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::endDraw(const DrawContext& context)
{
	CameraOutput* output = getOutput(context.gpuContext->getId());
	DrawContext& dc = myDrawContext[context.gpuContext->getId()];
	output->endDraw(dc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::startFrame(const FrameInfo& frame)
{
	CameraOutput* output = getOutput(frame.gpuContext->getId());
	output->startFrame(frame);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::finishFrame(const FrameInfo& frame)
{
	CameraOutput* output = getOutput(frame.gpuContext->getId());
	output->finishFrame(frame);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::setModelView(const Vector3f& position, const Quaternion& orientation)
{
	myPosition = position;
	myOrientation = orientation;
	myModelView = Math::makeViewMatrix(position, orientation);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::setProjection(float fov, float aspect, float nearZ, float farZ)
{
	myProjection = Math::makePerspectiveMatrix(fov * Math::DegToRad, aspect, nearZ, farZ);
	myFov = fov;
	myNearZ = nearZ;
	myFarZ = farZ;
	myAspect = aspect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
Ray Camera::getViewRay(const Vector2f& normalizedPoint)
{
	Vector2f pt(
		normalizedPoint[0] * 2.0f - 1.0f,
		normalizedPoint[1] * 2.0f - 1.0f);
	return Math::unprojectNormalized(pt, myModelView, myProjection);
}
