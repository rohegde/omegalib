/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "osystem.h"
#include "SystemManager.h"
//#include "InputEvent.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class SystemManager;
	class InputManager;
	class DisplaySystem;
	class Application;
	class ChannelImpl;
	struct InputEvent;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Interface for objects that manage a single OpenGL context.
	class IGLContextManager
	{
	public:
		//! Makes the context current.
		virtual void makeCurrent() = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct UpdateContext
	{
		uint64 frameNum;
		float dt;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct DrawContext
	{
		uint64 frameNum;
		unsigned int layer;
		IGLContextManager* glContext;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ApplicationClient
	{
	friend class DisplaySystem;
	public:
		ApplicationClient(Application* app): myApplication(app) {}
		virtual ~ApplicationClient() {}

		virtual void setup() {}
		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual void draw(const DrawContext& context) {}
		virtual bool handleEvent(const InputEvent& evt) { return false; }

		Application* getApplication() { return myApplication; }
		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		InputManager*   getInputManager()   { return SystemManager::instance()->getInputManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }

		const Recti& getViewport() const { return myViewport; }
		const Matrix4f& getModelViewMatrix() const { return myModelView; }
		const Matrix4f& getProjectionMatrix() const { return myProjection; }
		const Vector2i getResolution() const { return myResolution; }

		//! helper method to get view ray from a point on the near projection plane.
		Ray unproject(Vector2f point)
		{
			return Math::unproject(point, myModelView, myProjection, myViewport);
		}

	protected:
		void resetGLContext();

	private:
		void setViewport(const Recti& value) { myViewport = value; }
		void setModelViewMatrix(const Matrix4f& value) { myModelView = value; }
		void setProjectionMatrix(const Matrix4f& value) { myProjection = value; }
		void setResolution(const Vector2i& value) { myResolution = value; }

	private:
		Application* myApplication;
		Recti myViewport;
		Matrix4f myModelView;
		Matrix4f myProjection;
		Vector2i myResolution;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ApplicationServer
	{
	public:
		ApplicationServer(Application* app): myApplication(app) {}
		virtual ~ApplicationServer() {}

		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual bool handleEvent(const InputEvent& evt) { return false; }

		Application* getApplication() { return myApplication; }

	private:
		Application* myApplication;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Application
	{
	public:
		static const int MaxLayers = 16;

	public:
		virtual const char* getName() { return "OmegaLib " OMEGA_VERSION; }

		//! Instantiates a new Channel instance.
		//! Users redefine this method to create instances of their own Channel objects.
		//! @param impl - the internal DisplaySystem-dependent channel implementation.
		virtual ApplicationServer* createServer() { return new ApplicationServer(this); };
		virtual ApplicationClient* createClient() { return new ApplicationClient(this); };

		//! Called once for entire application initialization tasks.
		virtual void initialize() {}

		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		InputManager*   getInputManager()   { return SystemManager::instance()->getInputManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
	};
}; // namespace omega

#endif