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
#ifndef __ENGINE_CLIENT_H__
#define __ENGINE_CLIENT_H__

#include "oenginebase.h"
#include "Renderable.h"
#include "omega/Application.h"

namespace oengine {
	class RenderPass;
	class EngineServer;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API EngineClient: public ApplicationClient
	{
	public:
		EngineClient(ApplicationServer* server);

		EngineServer* getServer();

		void addRenderPass(RenderPass* pass, bool addToFront = false);
		void removeRenderPass(RenderPass* pass);

		void queueRenderableCommand(RenderableCommand& cmd);

		virtual void initialize();
		//virtual void handleEvent(const Event& evt);
		//virtual void update(const UpdateContext& context);
		virtual void draw(const DrawContext& context);

		Renderer* getRenderer();

	private:
		EngineServer* myServer;
		Renderer* myRenderer;
		List<RenderPass*> myRenderPassList;
		Queue<RenderableCommand> myRenderableCommands;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! a convenience application class to create oengine applications
	template<typename T> 
	class OENGINE_API EngineApplication: public Application
	{
	public:
		virtual ApplicationClient* createClient(ApplicationServer* server) 
		{ return new EngineClient(server); }
		virtual ApplicationServer* createServer() 
		{ return new T(this); }
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Renderer* EngineClient::getRenderer()
	{ return myRenderer; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline EngineServer* EngineClient::getServer()
	{ return myServer; }
}; // namespace oengine

#endif
