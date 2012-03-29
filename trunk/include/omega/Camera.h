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

#include "osystem.h"
#include "omega/ApplicationBase.h"
#include "omega/SceneNode.h"
#include "omega/RenderTarget.h"
#include "omega/Observer.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API CameraOutput: public ReferenceType
	{
	public:
		CameraOutput(bool offscreen = false): myEnabled(false), myRenderTarget(NULL), myOffscreen(offscreen),
			myReadbackColorTarget(NULL), myReadbackDepthTarget(NULL)
		{}

		void beginDraw(const DrawContext& context);
		void endDraw(const DrawContext& context);
		void startFrame(const FrameInfo& frame);
		void finishFrame(const FrameInfo& frame);

		bool isEnabled() { return myEnabled; }
		void setEnabled(bool value) { myEnabled = value; }

		void setReadbackTarget(PixelData* color, PixelData* depth = NULL);
		void setReadbackTarget(PixelData* color, PixelData* depth, const Rect& readbackViewport);

		const Rect& getReadbackViewport() { return myReadbackViewport; }

		RenderTarget* getRenderTarget() { return myRenderTarget; }

	private:
		bool myEnabled;
		bool myOffscreen;
		RenderTarget* myRenderTarget;

		PixelData* myReadbackColorTarget;
		PixelData* myReadbackDepthTarget;
		Rect myReadbackViewport;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Camera: public ReferenceType
	{
	public:
		enum NavigationMode { NavDisabled, NavFollow, NavFreeFly };
		enum MoveFlags { 
			MoveLeft = Event::ButtonLeft,
			MoveRight = Event::ButtonRight,
			MoveUp = Event::Button5,
			MoveDown = Event::Button6,
			MoveForward = Event::ButtonUp,
			MoveBackward = Event::ButtonDown,

			FocusLeft = 1 << 7,
			FocusRight = 1 << 8,
			FocusForward = 1 << 9,
			FocusBackward = 1 << 10,
			FocusUp = 1 << 11,
			FocusDown = 1 << 12
			};

		enum CameraFlags
		{
			ForceMono = 1 << 1,
			DrawScene = 1 << 2,
			DrawOverlay = 1 << 3,
			Offscreen = 1 << 4,
			DefaultFlags = DrawScene | DrawOverlay
		};

	public:
		Camera(uint flags = DefaultFlags);

		bool isOffscreen() { return myFlags & Offscreen; }

		CameraOutput* getOutput(uint contextId);

		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		const Vector3f& getProjectionOffset() { return myProjectionOffset; }
		void setProjectionOffset(const Vector3f& value) { myProjectionOffset = value; }

		const Quaternion& getOrientation();
		void setOrientation(const Quaternion& value);

		const Vector3f& getPosition();
		void setPosition(const Vector3f& value);

		void setProjection(float fov, float aspect, float nearZ, float farZ);
		void setModelView(const Vector3f& position, const Quaternion& orientation);

		//! Returns a view ray given an origin point in normalized coordinates.
		//! @param normalizedPoint - the origin point for the ray in normalized ([0, 1]) coordinates
		Ray getViewRay(const Vector2f& normalizedPoint);

		//const AffineTransform3& getViewTransform();
		//const AffineTransform3& getProjectionTransform();
		//void setProjectionTransform(const AffineTransform3& value);
		//void setViewTransform(const AffineTransform3& value);

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

		//! Returns true if this camera is enabled in the specified draw context.
		bool isEnabled(const DrawContext& context);

		void endDraw(const DrawContext& context);
		const DrawContext& beginDraw(const DrawContext& context);
		void startFrame(const FrameInfo& frame);
		void finishFrame(const FrameInfo& frame);

	private:
		//! Current view transform
		AffineTransform3 myModelView;
		Transform3 myProjection;

		//! Observer current position.
		Vector3f myPosition;

		//! Observer current rotation.
		Quaternion myOrientation;

		//! Field of view (in radians)
		float myFov;
		float myAspect;
		float myNearZ;
		float myFarZ;

		//! Projection plane offset.
		//! this is the position of the center of the projection plane when using a head tracked system + off-axis projection. 
		//! When omegalib is using head tracking, usually the observer update service takes care of setting this value
		//! directly in the Observer class, so this value is ignored. On desktop system, the user can set this value
		//! manually to test off-axis projection.
		Vector3f myProjectionOffset;

		//! When set to true, the aspect is computed depending on the height & width of the camera render target.
		bool myAutoAspect;

		// Offscren rendering stuff
		uint myFlags;
		Ref<CameraOutput> myOutput[GpuContext::MaxContexts];
		DrawContext myDrawContext[GpuContext::MaxContexts];

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
		Vector3f myControllerMoveVector;
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

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Quaternion& Camera::getOrientation()
	{ return myOrientation; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Camera::setOrientation(const Quaternion& value) 
	{ myOrientation = value; setModelView(myPosition, myOrientation);}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Vector3f& Camera::getPosition() 
	{ return myPosition; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Camera::setPosition(const Vector3f& value) 
	{ myPosition = value; setModelView(myPosition, myOrientation); }
}; // namespace omega

#endif