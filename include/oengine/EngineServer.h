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

#include "oenginebase.h"
#include "EngineClient.h"
#include "SceneNode.h"
#include "Pointer.h"
#include "Renderable.h"
#include "SceneQuery.h"
#include "Actor.h"
#include "Camera.h"
#include "Font.h"
#include "Light.h"
#include "Console.h"
#include "ui/Container.h"
#include "ui/WidgetFactory.h"
#include "omega/Application.h"

namespace oengine {

	typedef List<EngineClient*> EngineClientList;
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API EngineServer: public ApplicationServer
	{
	friend class EngineClient;
	public:
		typedef RenderPass* (*RenderPassFactory)(EngineClient*);
		typedef List<Camera*> CameraCollection;

	public:
		static const int MaxScenes = 3;
		static const int MaxUis = 3;
		static const int MaxActivePointers = 128;
		static const int MaxLights = 16;

		static EngineServer* mysInstance;
		static EngineServer* instance() { return mysInstance; }

	public:
		EngineServer(Application* app);

		ServiceManager* getServiceManager();

		void addClient(EngineClient* client);
		EngineClientList& getClients();

		//! Render pass management
		//@{
		template<typename T> void registerRenderPassClass();
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
		const SceneQueryResultList& querySceneRay(int sceneId, const Ray& ray, uint flags = 0);
		//@}

		SceneNode* getScene(int id);
		ui::Container* getUi(int id);
		ui::WidgetFactory* getWidgetFactory();

		void addActor(Actor* actor);
		void removeActor(Actor* actor);

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
		//@}

		//! Lights
		//@{
		Light* getLight(int index);
		Color& getAmbientLightColor();
		void setAmbientLightColor(const Color& value);
		//@}

		Camera* getDefaultCamera();
		Camera* createCamera(uint flags = Camera::DefaultFlags);
		void destroyCamera(Camera* cam);
		CameraCollection::Range getCameras();
		CameraCollection::ConstRange getCameras() const;

		virtual void initialize();
		virtual void handleEvent(const Event& evt);
		virtual void update(const UpdateContext& context);

	private:
		//! Draw pointer objects inside a specific client context.
		void drawPointers(EngineClient* client, RenderState* state);

	private:
		List<EngineClient*> myClients;

		Dictionary<String, RenderPassFactory> myRenderPassFactories;

		SceneNode* myScene[MaxScenes];
		ui::Container* myUi[MaxUis];
		Light myLights[MaxLights];
		Color myAmbientColor;

		// Cameras.
		Camera* myDefaultCamera;
		CameraCollection myCameras;

		// Pointers
		List<Pointer*> myPointers;
		std::pair<Pointer*, float> myActivePointers[MaxActivePointers];
		float myActivePointerTimeout;

		List<Actor*> myActors;

		// Resources
		FontInfo myDefaultFont;
		ui::WidgetFactory* myWidgetFactory;

		// Scene querying
		RaySceneQuery myRaySceneQuery;

		// Console
		Console* myConsole;
		bool myConsoleEnabled;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ServiceManager* EngineServer::getServiceManager()
	{ return SystemManager::instance()->getServiceManager(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> 
	inline void EngineServer::registerRenderPassClass()
	{
		myRenderPassFactories[T::Type->getName()] = (RenderPassFactory)T::createInstance;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline EngineClientList& EngineServer::getClients()
	{ return myClients; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void EngineServer::setDefaultFont(const FontInfo& font)
	{ myDefaultFont = font;	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const FontInfo& EngineServer::getDefaultFont()
	{ return myDefaultFont; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ui::WidgetFactory* EngineServer::getWidgetFactory()
	{ return myWidgetFactory; }
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Light* EngineServer::getLight(int index)
	{ return &myLights[index]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color& EngineServer::getAmbientLightColor()
	{ return myAmbientColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void EngineServer::setAmbientLightColor(const Color& value)
	{ myAmbientColor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Camera* EngineServer::getDefaultCamera()
	{ return myDefaultCamera; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Console* EngineServer::getConsole()
	{ return myConsole;	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool EngineServer::isConsoleEnabled()
	{ return myConsoleEnabled; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void EngineServer::setConsoleEnabled(bool value)
	{ myConsoleEnabled = value; }
}; // namespace oengine

#endif
