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
#include "omega/Renderable.h"
#include "omega/EngineServer.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable::Renderable():
	myClient(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
DrawInterface* Renderable::getRenderer()
{ 
	return myClient->getRenderer(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
RenderableFactory::RenderableFactory():
	myInitialized(false),
	myServer(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
RenderableFactory::~RenderableFactory()
{
	dispose();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* RenderableFactory::addRenderable(Renderer* cli)
{
	Renderable* r = createRenderable();
	r->setClient(cli);
	myRenderables.push_back(r);
	RenderableCommand rc(r, RenderableCommand::Initialize);
	cli->queueRenderableCommand(rc);
	return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RenderableFactory::initialize(EngineServer* srv)
{
	if(!myInitialized)
	{
		//ofmsg("Initializing renderable factory: %1%", %toString());
		myServer = srv;
		foreach(Renderer* client, srv->getClients())
		{
			addRenderable(client);
		}
		myInitialized = true;
	}
	else
	{
		owarn("!RenderableFactory::initialize - renderable already initialized");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RenderableFactory::dispose()
{
	if(myInitialized)
	{
		//ofmsg("Disposing renderable factory: %1%", %toString());
		foreach(Renderable* r, myRenderables)
		{
			RenderableCommand rc(r, RenderableCommand::Dispose);
			r->getClient()->queueRenderableCommand(rc);
		}
		myRenderables.empty();
		myInitialized = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RenderableFactory::refresh()
{
	foreach(Renderable* r, myRenderables)
	{
		RenderableCommand rc(r, RenderableCommand::Refresh);
		r->getClient()->queueRenderableCommand(rc);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* RenderableFactory::getRenderable(Renderer* client)
{
	foreach(Renderable* r, myRenderables)
	{
		if(r->getClient() == client) return r;
	}
	// A renderable has not ben created yet for the specified client. do it now.
	return addRenderable(client);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* RenderableFactory::getFirstRenderable()
{
	if(myRenderables.size() != 0) return myRenderables.front();
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
List<Renderable*>::ConstRange RenderableFactory::getRenderables() const
{
	return List<Renderable*>::ConstRange(myRenderables.begin(), myRenderables.end());
}
