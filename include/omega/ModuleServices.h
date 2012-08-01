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
#ifndef __MODULE_SERVICES_H__
#define __MODULE_SERVICES_H__

#include "osystem.h"
//#include "Renderer.h"
#include "ServerEngine.h"
//#include "omega/ApplicationBase.h"
//#include "omega/SystemManager.h"
#include "omega/SharedDataServices.h"

namespace omega {
	class RenderPass;
	class Renderer;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ServerModule: public SharedObject
	{
	public:
		enum Priority { PriorityLow = 0, PriorityNormal = 1, PriorityHigh = 2 };

	public:
		ServerModule(const String& name): myInitialized(false), myEngine(NULL), myName(name), myPriority(PriorityNormal) {}
		ServerModule(): myInitialized(false), myEngine(NULL), myName(mysNameGenerator.generate()), myPriority(PriorityNormal) {}

		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual void handleEvent(const Event& evt) {}
		virtual void commitSharedData(SharedOStream& out) {}
		virtual void updateSharedData(SharedIStream& in) {}

		virtual void initializeRenderer(Renderer*) {}
		virtual void preDraw(Renderer*, const DrawContext& context) {}
		virtual void postDraw(Renderer*, const DrawContext& context) {}

		void doInitialize(ServerEngine* server) 
		{ 
			myEngine = server; 
			if(!myInitialized) 
			{
				initialize(); 
				foreach(Renderer* r, server->getClients())
				{
					initializeRenderer(r);
				}

				SharedDataServices::registerObject(this, myName);
				myInitialized = true; 
			}
		}

		virtual bool isInitialized() { return myInitialized; }

		ServerEngine* getServer() { return myEngine; }

		Priority getPriority() { return myPriority; }
		void setPriority(Priority value) { myPriority = value; }

	private:
		String myName;
		Priority myPriority;
		bool myInitialized;
		ServerEngine* myEngine;
		static NameGenerator mysNameGenerator;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ModuleServices
	{
	public:
		static void addModule(ServerModule* module)
		{ 
			mysModules.push_back(module); 
		}

		static List<ServerModule*>::ConstRange getModules() 
		{ 
			return List<ServerModule*>::ConstRange(mysModules.begin(), mysModules.end()); 
		}

		static void update(ServerEngine* srv, const UpdateContext& context)
		{
			foreach(ServerModule* module, mysModules)
			{
				module->doInitialize(srv);
				module->update(context);
			}
		}

		static void handleEvent(ServerEngine* srv, const Event& evt)
		{
			for(int i = ServerModule::PriorityHigh; i >= ServerModule::PriorityLow; i--)
			{
				foreach(ServerModule* module, mysModules)
				{
					if(module->getPriority() == i)
					{
						module->doInitialize(srv);
						module->handleEvent(evt);
					}
				}
			}
		}

		static void initializeRenderer(ServerEngine* srv, Renderer* r)
		{
			foreach(ServerModule* module, mysModules)
			{
				//module->doInitialize(srv);
				//module->initializeRenderer(r);
			}
		}

		static void preDraw(ServerEngine* srv, Renderer* r, const DrawContext& context)
		{
			foreach(ServerModule* module, mysModules)
			{
				module->doInitialize(srv);
				module->preDraw(r, context);
			}
		}

		static void postDraw(ServerEngine* srv, Renderer* r, const DrawContext& context)
		{
			foreach(ServerModule* module, mysModules)
			{
				module->doInitialize(srv);
				module->postDraw(r, context);
			}
		}

		static void cleanup()
		{
			foreach(ServerModule* module, mysModules)
			{
				delete module;
			}
			mysModules.clear();
		}

	private:
		static List<ServerModule*> mysModules;
	};

}; // namespace omega

#endif
