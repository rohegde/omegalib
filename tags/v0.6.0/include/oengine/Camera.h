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
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "oenginebase.h"
#include "omega/RenderTarget.h"

namespace oengine {
	class SceneManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API Camera
	{
	public:
		Camera(SceneManager* scene, RenderTarget* target);

		const AffineTransform3& getViewTransform();
		const AffineTransform3& getProjectionTransform();
		void setProjectionTransform(const AffineTransform3& value);
		void setViewTransform(const AffineTransform3& value);

		void updateView(const Vector3f& position, const Quaternion& orientation);
		void updateProjection(float fov, float aspect, float nearZ, float farZ);

		RenderTarget* getRenderTarget();
		SceneManager* getScene();

		bool getAutoAspect();
		void setAutoAspect(bool value);
		void render();

	private:
		RenderTarget* myRenderTarget;
		SceneManager* myScene;

		//! Current view transform
		AffineTransform3 myView;
		AffineTransform3 myProjection;

		//! Observer current position.
		Vector3f myPosition;
		//! Observer current rotation.
		Quaternion myOrientation;
		//! Field of view (in radians)
		float myFov;
		float myAspect;
		float myNearZ;
		float myFarZ;
		bool myAutoAspect;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AffineTransform3& Camera::getViewTransform() 
	{ return myView; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AffineTransform3& Camera::getProjectionTransform()
	{ return myProjection; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Camera::setProjectionTransform(const AffineTransform3& value)
	{ myProjection = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Camera::setViewTransform(const AffineTransform3& value)
	{ myView = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline RenderTarget* Camera::getRenderTarget()
	{ return myRenderTarget; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneManager* Camera::getScene()
	{ return myScene; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Camera::getAutoAspect()
	{ return myAutoAspect; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Camera::setAutoAspect(bool value)
	{ myAutoAspect = value; }
}; // namespace oengine

#endif