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
#ifndef __ENGINE_APPLICATION_H__
#define __ENGINE_APPLICATION_H__

#include "osystem.h"
#include "Renderable.h"
#include "Renderer.h"
#include "ServerEngine.h"
#include "omega/ApplicationBase.h"
#include "omega/SystemManager.h"

namespace omega {
	class RenderPass;
	class ServerEngine;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ServerModule
	{
	public:
		ServerModule(): myInitialized(false), myEngine(NULL) {}

		virtual void initialize() = 0;
		virtual void update(const UpdateContext& context) {}
		virtual void handleEvent(const Event& evt) {}

		void doInitialize(ServerEngine* server) { myEngine = server; if(!myInitialized) initialize(); myInitialized = true; }
		virtual bool isInitialized() { return myInitialized; }

		ServerEngine* getServer() { return myEngine; }

	private:
		bool myInitialized;
		ServerEngine* myEngine;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ModuleServices
	{
	public:
		static void addModule(ServerModule* module)
		{ 
			mysModules.push_front(module); 
		}

		static List<ServerModule*>::ConstRange getModules() 
		{ 
			return List<ServerModule*>::ConstRange(mysModules.begin(), mysModules.end()); 
		}

		static void initialize(ServerEngine* engine)
		{
			foreach(ServerModule* module, mysModules)
			{
				module->doInitialize(engine);
			}
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
			foreach(ServerModule* module, mysModules)
			{
				module->doInitialize(srv);
				module->handleEvent(evt);
			}
		}
	private:
		static List<ServerModule*> mysModules;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! a convenience application class to create omegaToolkit applications
	void OMEGA_API registerPortholeTabletService();
	template<typename T> 
	class Application: public ApplicationBase
	{
	public:
		Application(const String& name): myAppName(name) 
		{ }

		virtual const char* getName() 
		{ return myAppName.c_str(); }

		virtual void initialize() 
		{ registerPortholeTabletService(); }

		virtual RendererBase* createClient(ServerBase* server) 
		{ return new Renderer(server); }

		virtual ServerBase* createServer() 
		{ return new ServerEngine(this, false); }

		virtual ServerBase* createMaster() 
		{ 
			MasterEngine* me = new MasterEngine(this); 
			me->addInteractive(new T());
			return me;
		}
	private:
		String myAppName;
	};
}; // namespace omega

#endif
