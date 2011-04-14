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
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "osystem.h"
#include "SystemManager.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class SystemManager;
	class ServiceManager;
	class DisplaySystem;
	class Application;
	class ChannelImpl;
	class GpuManager;
	struct Event;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Layer
	{
	public:
		enum Enum
		{
			Null = 0,
			Scene0 = 1,
			Scene1 = 2,
			Scene2 = 3,

			UI0 = 4,
			Scene0UI0 = 5,
			Scene1UI0 = 6,
			Scene2UI0 = 7,

			UI1 = 8,
			Scene0UI1 = 9,
			Scene1UI1 = 10,
			Scene2UI1 = 11,

			UI2 = 12,
			Scene0UI2 = 13,
			Scene1UI2 = 14,
			Scene2UI2 = 15
		};

		static Enum fromString(const String& str);

	private:
		Layer() {}
	};

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
		AffineTransform3 modelview;
		Transform3 projection;
		//! The pixel viewport coordinates of this context with respect to the owner window of the context.
		Rect viewport;
		//! The pixel viewport coordinates of this context with respect to the global canvas managed by the display system.
		Rect globalViewport;
		IGLContextManager* glContext;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ApplicationClient: public DynamicObject
	{
	friend class DisplaySystem;
	public:
		ApplicationClient(Application* app);
		virtual ~ApplicationClient(); 

		virtual void setup() {}
		virtual void initialize();

		virtual void update(const UpdateContext& context) {}
		virtual void draw(const DrawContext& context) {}
		virtual bool handleEvent(const Event& evt, DrawContext& context) { return false; }
		virtual bool handleEvent(const Event& evt, UpdateContext& context) { return false; }

		Application* getApplication() { return myApplication; }
		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		ServiceManager*   getServiceManager()   { return SystemManager::instance()->getServiceManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
		GpuManager*		getGpu() { return myGpu; }

	protected:
		void resetGLContext();

	private:

	private:
		Application* myApplication;
		GpuManager* myGpu;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ApplicationServer: public DynamicObject
	{
	public:
		ApplicationServer(Application* app): myApplication(app) {}
		virtual ~ApplicationServer() {}

		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual bool handleEvent(const Event& evt) { return false; }

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

		//SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		//ServiceManager*   getInputManager()   { return SystemManager::instance()->getServiceManager(); }
		//DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
	};
}; // namespace omega

#endif