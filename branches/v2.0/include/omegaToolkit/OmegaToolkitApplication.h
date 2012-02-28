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
#ifndef __OMEGA_TOOLKIT_APPLICATION_H__
#define __OMEGA_TOOLKIT_APPLICATION_H__

#include "omegaToolkitConfig.h"
#include "omega/Application.h"
#include "UiModule.h"
#include "LightingPass.h"
#include "DefaultRenderPass.h"
#include "TransparentRenderPass.h"
#include "OverlayRenderPass.h"

namespace omegaToolkit {
	using namespace omega;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OmegaToolkitEngineModule: public ServerModule
	{
	public:
		OmegaToolkitEngineModule()
		{
			ModuleServices::addModule(new UiModule());
		}

		virtual void initialize() 
		{
			omsg("OmegaToolkitEngineModule initializing...");

			ServerEngine* engine = getServer();

			// Setup default render chain.
			engine->registerRenderPassClass("LightingPass", (ServerEngine::RenderPassFactory)LightingPass::createInstance);
			engine->registerRenderPassClass("DefaultRenderPass", (ServerEngine::RenderPassFactory)DefaultRenderPass::createInstance);
			engine->registerRenderPassClass("TransparentRenderPass", (ServerEngine::RenderPassFactory)TransparentRenderPass::createInstance);
			engine->registerRenderPassClass("OverlayRenderPass", (ServerEngine::RenderPassFactory)OverlayRenderPass::createInstance);

			engine->addRenderPass("LightingPass");
			engine->addRenderPass("DefaultRenderPass");
			engine->addRenderPass("TransparentRenderPass");
			engine->addRenderPass("OverlayRenderPass");

			omsg("OmegaToolkitEngineModule initialization OK");
		}
		virtual void update(const UpdateContext& context) {}
		virtual void handleEvent(const Event& evt) {};
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! A convenience application class to create omegaToolkit applications
	template<typename T> 
	class OmegaToolkitApplication: public Application<T>
	{
	public:
		OmegaToolkitApplication(const String& name): Application<T>(name)
		{ModuleServices::addModule(new OmegaToolkitEngineModule());}
		//virtual ServerBase* createServer() 
		//{ return new T(this); }

	private:
	};
}; // namespace omega

#endif
