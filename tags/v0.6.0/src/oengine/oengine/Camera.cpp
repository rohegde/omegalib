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
#include "oengine/SceneManager.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera::Camera(SceneManager* scene, RenderTarget* target):
	myScene(scene),
	myRenderTarget(target),
	myAutoAspect(false)
{
	updateView(Vector3f::Zero(), Quaternion::Identity());
	updateProjection(60 * Math::DegToRad, 1, 0.01f, 100);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::updateView(const Vector3f& position, const Quaternion& orientation)
{
	myPosition = position;
	myOrientation = orientation;
	myView = Math::makeViewMatrix(position, orientation);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::updateProjection(float fov, float aspect, float nearZ, float farZ)
{
	myProjection = Math::makePerspectiveMatrix(fov, aspect, nearZ, farZ);
	myFov = fov;
	myNearZ = nearZ;
	myFarZ = farZ;
	myAspect = aspect;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::render()
{
	oassert(myScene != NULL);
	oassert(myRenderTarget != NULL);

	Rect viewport = Rect(0, 0, myRenderTarget->getWidth(), myRenderTarget->getHeight());

	if(myAutoAspect)
	{
		float aspect = (float)viewport.width() / viewport.height();
		updateProjection(myFov, aspect, myNearZ, myFarZ);
	}

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadMatrixf(myProjection.data());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(myView.data());


	myRenderTarget->beginDraw();
	oerror("Reimplement, needs DrawContext");
	//myScene->draw(viewport);
	myRenderTarget->endDraw();

	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
