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
#include "Renderable.h"
#include "Actor.h"
#include "Font.h"
#include "ui/Container.h"
#include "ui/WidgetFactory.h"
#include "omega/Application.h"

namespace oengine {

	typedef List<EngineClient*> EngineClientList;
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API EngineServer: public ApplicationServer
	{
	public:
		typedef RenderPass* (*RenderPassFactory)();

	public:
		static const int MaxScenes = 3;
		static const int MaxUis = 3;

	public:
		EngineServer(Application* app);

		ServiceManager* getServiceManager();

		void addClient(EngineClient* client);
		EngineClientList& getClients();

		//! Render pass management
		//@{
		template<typename T> void registerRenderPassClass();
		void addRenderPass(String renderPass, bool addToFront = false);
		void removeRenderPass(String renderPass);
		//@}

		//! Font management
		//@{
		void setDefaultFont(const FontInfo& font);
		const FontInfo& getDefaultFont();
		//@}

		SceneNode* getScene(int id);
		ui::Container* getUi(int id);
		ui::WidgetFactory* getWidgetFactory();

		void addActor(Actor* actor);
		void removeActor(Actor* actor);

		virtual void initialize();
		virtual void handleEvent(const Event& evt);
		virtual void update(const UpdateContext& context);

	private:
		List<EngineClient*> myClients;

		Dictionary<String, RenderPassFactory> myRenderPassFactories;

		SceneNode* myScene[MaxScenes];
		ui::Container* myUi[MaxUis];

		List<Actor*> myActors;

		// Resources
		FontInfo myDefaultFont;
		ui::WidgetFactory* myWidgetFactory;
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
		
}; // namespace oengine

#endif
