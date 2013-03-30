/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __ENGINE_CLIENT_H__
#define __ENGINE_CLIENT_H__

#include "osystem.h"
#include "Renderable.h"
#include "omega/ApplicationBase.h"
#include "omega/SystemManager.h"

namespace omega {
	class RenderPass;
	class Engine;
	class Camera;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Renderer: public ReferenceType
	{
	friend class DisplaySystem;
	//friend class ServerBase;
	public:
		Renderer(Engine* server);

		Engine* getEngine();

		void addRenderPass(RenderPass* pass, bool addToFront);
		void removeRenderPass(RenderPass* pass);
		RenderPass* getRenderPass(const String& name);
		void removeAllRenderPasses();


		void queueCommand(IRendererCommand* cmd);

		virtual void initialize();
		virtual void draw(const DrawContext& context);
		virtual void startFrame(const FrameInfo& frame);
		virtual void finishFrame(const FrameInfo& frame);

		DrawInterface* getRenderer();

		GpuContext* getGpuContext() { return myGpuContext; } 
		void setGpuContext(GpuContext* ctx) { myGpuContext = ctx; } 

		//ServerBase* getEngine() { return myServer; }
		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		ServiceManager*   getServiceManager()   { return SystemManager::instance()->getServiceManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }

		Texture* createTexture();
	private:
		void innerDraw(const DrawContext& context, Camera* camera);

	private:
		Lock myRenderCommandLock;
		Lock myRenderPassLock;

		GpuContext* myGpuContext;

		// Cant use Ref<Engine> because of circular header dependency
		Engine* myServer;

		Ref<DrawInterface> myRenderer;
		List< Ref<RenderPass> > myRenderPassList;
		Queue< Ref<IRendererCommand> > myRenderableCommands;

		List< Ref<Texture> > myTextures;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline DrawInterface* Renderer::getRenderer()
	{ return myRenderer.get(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Engine* Renderer::getEngine()
	{ return myServer; }
}; // namespace omega

#endif
