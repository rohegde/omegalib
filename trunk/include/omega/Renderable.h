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
#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include "osystem.h"
#include "RenderPass.h"

namespace omega {
	class EngineServer;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Renderable: public ReferenceType
	{
	public:
		Renderable();

		void setClient(EngineClient* client);
		EngineClient* getClient();
		Renderer* getRenderer();

		virtual void initialize() {}
		virtual void dispose() {}
		virtual void refresh() {}
		virtual void draw(RenderState* state) = 0;

	private:
		EngineClient* myClient;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct RenderableCommand
	{
		enum Command { Initialize, Dispose, Refresh };
		Renderable* renderable;
		Command command;

		RenderableCommand(Renderable* r, Command c): renderable(r), command(c) {}
		void execute()
		{
			switch(command)
			{
			case Initialize: renderable->initialize(); break;
			case Dispose: renderable->dispose(); break;
			case Refresh: renderable->refresh(); break;
			}
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Base class for objects that can create renderables.
	class OMEGA_API RenderableFactory: public ReferenceType
	{
	public:
		RenderableFactory();
		virtual ~RenderableFactory();
		virtual Renderable* createRenderable() = 0;
		virtual void initialize(EngineServer* srv);
		void dispose();
		void refresh();
		virtual bool isInitialized();
		Renderable* getRenderable(EngineClient* client);
		Renderable* getFirstRenderable();
		EngineServer* getServer();
		List<Renderable*>::ConstRange getRenderables() const;

	protected:
		Renderable* addRenderable(EngineClient* cli);

	private:
		bool myInitialized;
		EngineServer* myServer;
		List<Renderable*> myRenderables;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Renderable::setClient(EngineClient* value)
	{ myClient = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline EngineClient* Renderable::getClient()
	{ return myClient; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool RenderableFactory::isInitialized()
	{ return myInitialized; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline EngineServer* RenderableFactory::getServer()
	{ return myServer; }

}; // namespace omega

#endif