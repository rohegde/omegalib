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
	myController(NULL),
	myControllerEnabled(false),
	myPosition(Vector3f::Zero()),
	myOrientation(Quaternion::Identity())
{
	myProjectionOffset = -Vector3f::UnitZ();

	for(int i = 0; i < GpuContext::MaxContexts; i++)
	{
		myOutput[i] = new CameraOutput(isOffscreen());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::setup(Setting& s)
{
	String controllerName;
	controllerName = Config::getStringValue("controller", s);
	StringUtils::toLowerCase(controllerName);

	myController = NULL;
	if(controllerName == "keyboardmouse") myController = new KeyboardMouseCameraController();
	if(controllerName == "wand") myController = new WandCameraController();
	if(controllerName == "gamepad") myController = new GamepadCameraController();

	setController(myController);
	if(myController != NULL) 
	{
		myController->setup(s);
		setControllerEnabled(true);
	}

	if(myController == NULL)
	{
		// Process the old-style enableNavigation parameter, for compatibility.
		if(Config::getBoolValue("enableNavigation", s, false))
		{
			setController(new KeyboardMouseCameraController());
			setControllerEnabled(true);
		}
	}

    Vector3f camPos = Config::getVector3fValue("position", s); 
    setPosition(camPos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::handleEvent(const Event& evt)
{
	if(isControllerEnabled())
	{
		myController->handleEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::update(const UpdateContext& context)
{
	if(isControllerEnabled())
	{
		myController->update(context);
	}
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
