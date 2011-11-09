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
#include "oengine/SceneNode.h"
#include "omega/RenderTarget.h"
#include "omega/Observer.h"

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API Camera: public DynamicObject
	{
	public:
		enum NavigationMode { NavDisabled, NavFollow, NavFreeFly };
		enum MoveFlags { 
			MoveLeft = 1 << 1,
			MoveRight = 1 << 2,
			MoveUp = 1 << 3,
			MoveDown = 1 << 4,
			MoveForward = 1 << 5,
			MoveBackward = 1 << 6,

			FocusLeft = 1 << 7,
			FocusRight = 1 << 8,
			FocusForward = 1 << 9,
			FocusBackward = 1 << 10,
			FocusUp = 1 << 11,
			FocusDown = 1 << 12
			};
	public:
		Camera();

		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		const Vector3f& getPosition() { return myPosition; }
		void setPosition(const Vector3f& value) { myPosition = value; }

		const Vector3f& getProjectionOffset() { return myProjectionOffset; }
		void setProjectionOffset(const Vector3f& value) { myProjectionOffset = value; }

		const Quaternion& getOrientation() { return myOrientation; }
		void setOrientation(const Quaternion& value) { myOrientation = value; }

		//const AffineTransform3& getViewTransform();
		//const AffineTransform3& getProjectionTransform();
		//void setProjectionTransform(const AffineTransform3& value);
		//void setViewTransform(const AffineTransform3& value);

		//void updateView(const Vector3f& position, const Quaternion& orientation);
		//void updateProjection(float fov, float aspect, float nearZ, float farZ);

		bool getAutoAspect();
		void setAutoAspect(bool value);

		//! Navigation management
		//@{
		void setNavigationMode(NavigationMode value) { myNavigationMode = value; }
		NavigationMode getNavigationMode() { return myNavigationMode; }
		//! Set the target node that will be used when in NavFollow navigation mode.
		void setTargetNode(SceneNode* n) { myTargetNode = n; }
		SceneNode* getTargetNode() { return myTargetNode; }
		//@}

		void focusOn(SceneNode* node);

		void updateObserver(Observer* obs);

	private:
		//! Current view transform
		AffineTransform3 myViewTransform;
		//AffineTransform3 myProjection;

		//! Observer current position.
		//! This is the projection plane position when using a head tracked system + off-axis projection.
		Vector3f myPosition;
		//! Projection plane offset.
		//! this is the position of the center of the projection plane when using a head tracked system + off-axis projection. When using in-axis
		//! projection, only the z value is used to determine the near projection plane.
		//! When omegalib is using head tracking, usually the observer update service takes care of setting this value
		//! directly in the Observer class, so this value is ignored. On desktop system, the user can set this value
		//! manually to test off-axis projection.
		Vector3f myProjectionOffset;
		//! Observer current rotation.
		Quaternion myOrientation;
		//! Field of view (in radians)
		float myFov;
		float myAspect;
		float myFarZ;
		//! When set to true, the aspect is computed depending on the height & width of the camera render target.
		bool myAutoAspect;

		// Navigation stuff.
		NavigationMode myNavigationMode;
		SceneNode* myTargetNode;
		float myNavigationSpeed;
		float myNavigationStrafeMultiplier;
		float myNavigationYawMultiplier;
		float myNavigationPitchMultiplier;
		uint myNavigationMoveFlags;
		float myYaw;
		float myPitch;
		Vector3f myLastPointerPosition;
		bool myRotating;
		Vector2f myControllerMoveVector;
		Vector2f myControllerRotateVector;
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//inline const AffineTransform3& Camera::getViewTransform() 
	//{ return myView; }

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//inline const AffineTransform3& Camera::getProjectionTransform()
	//{ return myProjection; }

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//inline void Camera::setProjectionTransform(const AffineTransform3& value)
	//{ myProjection = value; }

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//inline void Camera::setViewTransform(const AffineTransform3& value)
	//{ myView = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Camera::getAutoAspect()
	{ return myAutoAspect; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Camera::setAutoAspect(bool value)
	{ myAutoAspect = value; }
}; // namespace oengine

#endif