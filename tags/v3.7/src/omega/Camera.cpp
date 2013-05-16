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
#include "omega/RenderTarget.h"
#include "omega/Camera.h"
#include "omega/CameraOutput.h"
#include "omega/ModuleServices.h"
#include "omega/WandCameraController.h"
#include "omega/GamepadCameraController.h"
#include "omega/MouseCameraController.h"
#include "omega/KeyboardMouseCameraController.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
Camera::Camera(Engine* e, uint flags):
	SceneNode(e),
	myAutoAspect(false),
	myFlags(flags),
	myController(NULL),
	myControllerEnabled(false),
	myTrackingEnabled(false),
	myTrackerSourceId(-1),
	myHeadOrientation(Quaternion::Identity()),
	myHeadOffset(Vector3f::Zero()),
	myMask(0),
	myEyeSeparation(0.06f)
{
	//myProjectionOffset = -Vector3f::UnitZ();

	// set camera Id and increment the counter
	this->myCameraId = omega::CamerasCounter++;

	for(int i = 0; i < GpuContext::MaxContexts; i++)
	{
		myOutput[i] = new CameraOutput(isOffscreen());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::setup(Setting& s)
{
	//set position of camera
    Vector3f camPos = Config::getVector3fValue("position", s, getPosition()); 
    setPosition(camPos);

	//set orientation of camera
	// NOTE: we want to either read orientation from the config or keep the default one.
	// Since orientation is expressed in yaw, pitch roll in the config file but there is no
	// way to get that from the camera (rotation is only as a quaternion) we cannot use the default
	// value in the Config::getVector3fValue.
	if(s.exists("orientation"))
	{
		Vector3f camOri = Config::getVector3fValue("orientation", s); 
		setPitchYawRoll(camOri * Math::DegToRad);
	}
	
	myTrackerSourceId = Config::getIntValue("trackerSourceId", s, -1);
	if(myTrackerSourceId != -1) myTrackingEnabled = true;

	//setup camera controller.  The camera needs to be setup before this otherwise its values will be rewritten

	String controllerName;
	controllerName = Config::getStringValue("controller", s);
	StringUtils::toLowerCase(controllerName);

	if(controllerName != "")
	{
		CameraController* controller = NULL;
		ofmsg("Camera controller: %1%", %controllerName);
		if(controllerName == "keyboardmouse") controller = new KeyboardMouseCameraController();
		if(controllerName == "mouse") controller = new MouseCameraController();
		if(controllerName == "wand") controller = new WandCameraController();
		if(controllerName == "gamepad") controller = new GamepadCameraController();

		setController(controller);
		if(myController != NULL) 
		{
			myController->setup(s);
			setControllerEnabled(true);
		}
	}

	Vector3f position = Vector3f::Zero();
	if(s.exists("headOffset"))
	{
		Setting& st = s["headOffset"];
		myHeadOffset.x() = (float)st[0];
		myHeadOffset.y() = (float)st[1];
		myHeadOffset.z() = (float)st[2];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::handleEvent(const Event& evt)
{
	if(myTrackingEnabled)
	{
		if(evt.getServiceType() == Event::ServiceTypeMocap && evt.getSourceId() == myTrackerSourceId)
		{
			myHeadOffset = evt.getPosition();
			myHeadOrientation = evt.getOrientation();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::updateTraversal(const UpdateContext& context)
{
	// Update the view transform
	myHeadTransform = AffineTransform3::Identity();
	myHeadTransform.translate(myHeadOffset);
	myHeadTransform.rotate(myHeadOrientation);

	// BUG: if we attach a child node to the camera, isUpdateNeeded gets reset at the wrong
	// time and the camera view transform does not get updated.
	// Needs fixing, but for now best solution is to disable the check and always update
	// the view transform
	//if(isUpdateNeeded())
	{
		// Update view transform.
		myViewTransform = Math::makeViewMatrix(getDerivedPosition(), getDerivedOrientation());
	}
	
	SceneNode::updateTraversal(context);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::lookAt(const Vector3f& position, const Vector3f& upVector)
{
	Node::lookAt(myHeadOffset - position, upVector);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::focusOn(SceneNode* node)
{
	// Compute direction vector
	Vector3f dir = mPosition - node->getPosition();
	dir.normalize();

	const Sphere& bs = node->getBoundingSphere();
	ofmsg("Camera:focuson %1% %2%", %bs.getCenter() %bs.getRadius());
	mPosition = bs.getCenter() + Vector3f(0, 0, bs.getRadius() * 2) - myHeadOffset;
	lookAt(node->getPosition(), Vector3f::UnitY());
	//mOrientation = Math::buildRotation(Vector3f::UnitZ(), dir, Vector3f::UnitY());
	//mPosition = bs.getCenter() + Vector3f(0, 0, bs.getRadius() * 2);
    //needUpdate();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
CameraOutput* Camera::getOutput(uint contextId)
{
	oassert(contextId < GpuContext::MaxContexts);
	return myOutput[contextId].get();
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
	//AffineTransform3 hti = myHeadTransform.inverse();
	//dc.modelview = hti * myViewTransform;

	// DO not apply any head transformation for custom cameras
	dc.modelview = myViewTransform;

	dc.viewport = output->getReadbackViewport();
	if(myAutoAspect)
	{
		float aspect = (float)dc.viewport.width() / dc.viewport.height();
		setProjection(myFov, 1.0f / aspect, myNearZ, myFarZ);
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
	return Math::unprojectNormalized(pt, myViewTransform, myProjection);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f Camera::localToWorldPosition(const Vector3f& position)
{
	Vector3f res = mPosition + mOrientation * position;
    return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Quaternion Camera::localToWorldOrientation(const Quaternion& orientation)
{
	return mOrientation * orientation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f Camera::worldToLocalPosition(const Vector3f& position)
{
	Vector3f res = mOrientation.inverse() * (position - mPosition);
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::setController(CameraController* value) 
{ 
	if(myController != NULL)
	{
		ModuleServices::removeModule(myController);
	}

	myController = value; 
	if(myController != NULL)
	{
		myController->setCamera(this);
		ModuleServices::addModule(myController);
	}
}
