/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "Engine.h"
//#include "omega/ApplicationBase.h"
//#include "omega/SystemManager.h"
#include "omega/SharedDataServices.h"

namespace omega {
	class RenderPass;
	class Renderer;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API EngineModule: public SharedObject
	{
	public:
		enum Priority { PriorityLow = 0, PriorityNormal = 1, PriorityHigh = 2 };

	public:
		EngineModule(const String& name): myInitialized(false), myEngine(NULL), myName(name), myPriority(PriorityNormal) {}
		EngineModule(): myInitialized(false), myEngine(NULL), myName(mysNameGenerator.generate()), myPriority(PriorityNormal) {}

		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual void handleEvent(const Event& evt) {}
		virtual void handleCommand(const String& cmd) {}
		virtual void commitSharedData(SharedOStream& out) {}
		virtual void updateSharedData(SharedIStream& in) {}

		virtual void initializeRenderer(Renderer*) {}
		//virtual void preDraw(Renderer*, const DrawContext& context) {}
		//virtual void postDraw(Renderer*, const DrawContext& context) {}

		void doInitialize(Engine* server);

		virtual bool isInitialized() { return myInitialized; }

		Engine* getEngine() { return myEngine; }

		Priority getPriority() { return myPriority; }
		void setPriority(Priority value) { myPriority = value; }

	private:
		String myName;
		Priority myPriority;
		bool myInitialized;
		Engine* myEngine;
		static NameGenerator mysNameGenerator;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ModuleServices
	{
	public:
		static void addModule(EngineModule* module);
		static List<EngineModule*>::ConstRange getModules();
		static void update(Engine* srv, const UpdateContext& context);
		static void handleEvent(const Event& evt);
		static void handleCommand(const String& cmd);
		//static void initializeRenderer(Engine* srv, Renderer* r);
		static void cleanup();

	private:
		static List<EngineModule*> mysModules;
	};
}; // namespace omega

#endif
