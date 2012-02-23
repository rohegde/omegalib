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
#include "EngineClient.h"
#include "EngineServer.h"
#include "omega/Application.h"
#include "omega/SystemManager.h"

namespace omega {
	class RenderPass;
	class EngineServer;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API IEngineModule
	{
	public:
		IEngineModule(): myInitialized(false) {}

		virtual void initialize(EngineServer* engine) = 0;
		virtual void update(const UpdateContext& context) = 0;
		virtual void handleEvent(const Event& evt) = 0;

		void doInitialize(EngineServer* server) { if(!myInitialized) initialize(server); myInitialized = true; }
		virtual bool isInitialized() { return myInitialized; }

	private:
		bool myInitialized;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API EngineModuleServices
	{
	public:
		static void addModule(IEngineModule* module)
		{ 
			mysModules.push_back(module); 
		}

		static List<IEngineModule*>::ConstRange getModules() 
		{ 
			return List<IEngineModule*>::ConstRange(mysModules.begin(), mysModules.end()); 
		}

		static void initialize(EngineServer* engine)
		{
			foreach(IEngineModule* module, mysModules)
			{
				module->doInitialize(engine);
			}
		}

		static void update(EngineServer* srv, const UpdateContext& context)
		{
			foreach(IEngineModule* module, mysModules)
			{
				module->doInitialize(srv);
				module->update(context);
			}
		}

		static void handleEvent(EngineServer* srv, const Event& evt)
		{
			foreach(IEngineModule* module, mysModules)
			{
				module->doInitialize(srv);
				module->handleEvent(evt);
			}
		}
	private:
		static List<IEngineModule*> mysModules;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! a convenience application class to create omegaToolkit applications
	void OMEGA_API registerPortholeTabletService();
	template<typename T> 
	class EngineApplication: public Application
	{
	public:
		EngineApplication(const String& name): myAppName(name) {}
		virtual const char* getName() { return myAppName.c_str(); }
		virtual void initialize() 
		{ registerPortholeTabletService(); }
		virtual ApplicationClient* createClient(ApplicationServer* server) 
		{ return new EngineClient(server); }
		virtual ApplicationServer* createServer() 
		{ return new T(this); }

	private:
		String myAppName;
	};
}; // namespace omega

#endif
