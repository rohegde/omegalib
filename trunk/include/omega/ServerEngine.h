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
#ifndef __ENGINE_SERVER_H__
#define __ENGINE_SERVER_H__

#include "osystem.h"
#include "Renderer.h"
#include "SceneNode.h"
#include "Pointer.h"
#include "Renderable.h"
#include "SceneQuery.h"
#include "Camera.h"
#include "Font.h"
#include "Console.h"

namespace omega {
	typedef List<Renderer*> EngineClientList;

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> class RendererObject
	{
	public:
		//typedef typename Dictionary<Renderer*, T>::Item Item;
		//typedef typename Dictionary<Renderer*, T>::Range Range;

	public:
		T& operator[](Renderer* c) { return myObjs[c]; }

		//Range getItems() { return Range(myObjs.begin(), myObjs.end()); }
		//int numItems() { return myObjs.size(); }
		//bool isEmpty() { return myObjs.empty(); }

	private:
		Dictionary<Renderer*, typename T> myObjs;
	};
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ServerEngine: public ServerBase
	{
	public:
		typedef List<Camera*> CameraCollection;

	friend class Renderer;
	public:
		typedef RenderPass* (*RenderPassFactory)(Renderer*);

	public:
		static const int MaxActivePointers = 128;
		static ServerEngine* instance() { return mysInstance; }

	public:
		ServerEngine(ApplicationBase* app, bool master);

		ServiceManager* getServiceManager();

		void addClient(Renderer* client);
		EngineClientList& getClients();

		//! Render pass management
		//@{
		Camera* getDefaultCamera();
		Camera* createCamera(uint flags = Camera::DefaultFlags);
		void destroyCamera(Camera* cam);
		CameraCollection::Range getCameras();
		CameraCollection::ConstRange getCameras() const;
		//@}

		//! Render pass management
		//@{
		void registerRenderPassClass(const String& name, RenderPassFactory factory);
		void addRenderPass(String renderPass, void* userData = NULL, bool addToFront = false);
		void removeRenderPass(String renderPass);
		void removeAllRenderPasses();
		//@}

		//! Font management
		//@{
		void setDefaultFont(const FontInfo& font);
		const FontInfo& getDefaultFont();
		//@}

		//! Scene query
		//@{
		const SceneQueryResultList& querySceneRay(const Ray& ray, uint flags = 0);
		//@}

		SceneNode* getScene();

 		//! Console management
		//@{
		Console* getConsole();
		bool isConsoleEnabled();
		void setConsoleEnabled(bool value);
		//@}

		//! Pointer Management
		//@{
		Pointer* createPointer();
		void destroyPointer(Pointer* p);
		void refreshPointer(int pointerId, int x, int y);
		//@}

		virtual void initialize();
		//! Internal method.
		void clientInitialize(Renderer* client);

		virtual void finalize();
		virtual void handleEvent(const Event& evt);
		virtual void update(const UpdateContext& context);

	private:
		//! Draw pointer objects inside a specific client context.
		void drawPointers(Renderer* client, RenderState* state);

	private:
		static ServerEngine* mysInstance;

		// Engine lock, used when client / server thread synchronization is needed.
		Lock myLock;

		List<Renderer*> myClients;

		Dictionary<String, RenderPassFactory> myRenderPassFactories;

		SceneNode* myScene;

		// Pointers
		List<Pointer*> myPointers;
		std::pair<Pointer*, float> myActivePointers[MaxActivePointers];
		float myActivePointerTimeout;

		// Resources
		FontInfo myDefaultFont;

		// Scene querying
		RaySceneQuery myRaySceneQuery;

		// Console
		Console* myConsole;
		bool myConsoleEnabled;

		bool myEventSharingEnabled;

		// Cameras.
		Camera* myDefaultCamera;
		CameraCollection myCameras;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Camera* ServerEngine::getDefaultCamera()
	{ return myDefaultCamera; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ServiceManager* ServerEngine::getServiceManager()
	{ return SystemManager::instance()->getServiceManager(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ServerEngine::registerRenderPassClass(const String& name, RenderPassFactory factory)
	{
		myRenderPassFactories[name] = factory;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline EngineClientList& ServerEngine::getClients()
	{ return myClients; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ServerEngine::setDefaultFont(const FontInfo& font)
	{ myDefaultFont = font;	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const FontInfo& ServerEngine::getDefaultFont()
	{ return myDefaultFont; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Console* ServerEngine::getConsole()
	{ return myConsole;	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool ServerEngine::isConsoleEnabled()
	{ return myConsoleEnabled; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ServerEngine::setConsoleEnabled(bool value)
	{ myConsoleEnabled = value; }
}; // namespace omega

#endif
