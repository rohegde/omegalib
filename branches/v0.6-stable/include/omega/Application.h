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
#include "IEventListener.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class SystemManager;
	class ServiceManager;
	class DisplaySystem;
	class Application;
	class ChannelImpl;
	class GpuManager;
	class Event;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! The Layer class contains enumerated values representing drawing layers.
	class Layer
	{
	public:
		enum Enum
		{
			Null = 0,
			Scene0 = 1,
			Scene1 = 2,
			Scene2 = 3,

			Overlay0 = 4,
			Scene0Overlay0 = 5,
			Scene1Overlay0 = 6,
			Scene2Overlay0 = 7,

			Overlay1 = 8,
			Scene0Overlay1 = 9,
			Scene1Overlay1 = 10,
			Scene2Overlay1 = 11,

			Overlay2 = 12,
			Scene0Overlay2 = 13,
			Scene1Overlay2 = 14,
			Scene2Overlay2 = 15
		};

		static Enum fromString(const String& str);

	private:
		Layer() {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Contains information about the context in which an update operation is taking place
	struct UpdateContext
	{
		uint64 frameNum;
		float time;
		float dt;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Contains information about a drawing channel.
	struct ChannelInfo
	{
		//! The index of this channel inside the view.
		Vector2i index;
		//! The width and height in pixels of the channel
		Vector2i size;
		//! The pixel offset of the channel inside the view
		Vector2i offset;
		//! The total size of the canvas in pixels
		Vector2i canvasSize;
		//! The number of horizontal and vertical channels composing the canvas.
		Vector2i canvasChannels;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Contains information about the context in which a drawing operation is taking place
	struct DrawContext
	{
		enum Eye { EyeLeft , EyeRight, EyeCyclop };
		uint64 frameNum;
		unsigned int layer;
		AffineTransform3 modelview;
		Transform3 projection;
		//! The pixel viewport coordinates of this context with respect to the owner window of the context.
		Rect viewport;
		//! The eye being rendered for this context.
		Eye eye;
		//! Information about the drawing channel associated with this context.
		ChannelInfo* channel;

		bool isSceneActive(int id) const;
		bool isOverlayActive(int id) const;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool DrawContext::isSceneActive(int id) const
	{ return (((layer & 0x03) - 1) == id); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool DrawContext::isOverlayActive(int id) const
	{ return (((layer >> 2) & 0x03) == id); }

	class ApplicationServer;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ApplicationClient: public DynamicObject, public IEventListener
	{
	friend class DisplaySystem;
	friend class ApplicationServer;
	public:
		ApplicationClient(ApplicationServer* app);
		virtual ~ApplicationClient(); 

		//! Returns a numeric identifier for this client (relative to server).
		uint getId();

		//virtual void setup() {}
		virtual void initialize();
		virtual void finalize() {}

		virtual void update(const UpdateContext& context) {}
		virtual void draw(const DrawContext& context) {}
		virtual void handleEvent(const Event& evt) { }

		ApplicationServer* getServer() { return myServer; }
		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		ServiceManager*   getServiceManager()   { return SystemManager::instance()->getServiceManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
		GpuManager*		getGpu() { return myGpu; }

	protected:
		void resetGLContext();

	private:

	private:
		uint myId;

		ApplicationServer* myServer;
		GpuManager* myGpu;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline uint ApplicationClient::getId()
	{ return myId; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ApplicationServer: public DynamicObject, public IEventListener
	{
	friend class ApplicationClient;
	public:
		ApplicationServer(Application* app): myApplication(app), myClientId(0) {}
		virtual ~ApplicationServer() {}

		virtual void initialize() {}
		virtual void finalize() {}
		virtual void update(const UpdateContext& context) {}
		virtual void handleEvent(const Event& evt) { }

		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		Application* getApplication() { return myApplication; }

	private:
		void addClient(ApplicationClient* cli);

	private:
		uint myClientId;
		Application* myApplication;
		List<ApplicationClient*> myClients;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
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
		virtual ApplicationClient* createClient(ApplicationServer* server) { return new ApplicationClient(server); };

		//! Called once for entire application initialization tasks.
		virtual void initialize() {}
	};
}; // namespace omega

#endif