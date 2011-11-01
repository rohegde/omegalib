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
#include "oengine/EngineClient.h"
#include "oengine/EngineServer.h"

#include "omega/DisplaySystem.h"
#include "omega/GpuManager.h"
#include "omega/Texture.h"
#include "omega/glheaders.h"
#include "omega/StringUtils.h"


using namespace omega;
using namespace oengine;

OMEGA_DEFINE_TYPE(RenderPass, OmegaObject)

///////////////////////////////////////////////////////////////////////////////////////////////////
EngineClient::EngineClient(ApplicationServer* server):
	ApplicationClient(server)
{
	myRenderer = new Renderer();
	myServer = (EngineServer*)server;
	myServer->addClient(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::addRenderPass(RenderPass* pass, bool addToFront)
{
	if(addToFront)
	{
		myRenderPassList.push_front(pass);
	}
	else
	{
		myRenderPassList.push_back(pass);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::removeRenderPass(RenderPass* pass)
{
	myRenderPassList.remove(pass);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::initialize()
{
	ofmsg("@EngineClient::Initialize: id = %1%", %getId());

	// Create the default font.
	const FontInfo& fi = myServer->getDefaultFont();
	if(fi.size != 0)
	{
		Font* fnt = myRenderer->createFont(fi.name, fi.filename, fi.size);
		myRenderer->setDefaultFont(fnt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::queueRenderableCommand(RenderableCommand& cmd)
{
	myRenderableCommands.push(cmd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::draw(const DrawContext& context)
{
	// First of all make sure all render passes are initialized.
	foreach(RenderPass* rp, myRenderPassList)
	{
		if(!rp->isInitialized()) rp->initialize();
	}

	// Execute renderable commands.
	while(!myRenderableCommands.empty())
	{
		myRenderableCommands.front().execute();
		if(myRenderableCommands.front().command == RenderableCommand::Dispose)
		{
			//ofmsg("Client %1% deleting renderable", %getId());
			delete myRenderableCommands.front().renderable;
		}
		myRenderableCommands.pop();
	}

	// Execute all render passes in order.
	foreach(RenderPass* pass, myRenderPassList)
	{
		pass->render(this, context);
	}

	if(context.task == DrawContext::OverlayDrawTask)
	{
		// Draw the pointers and console
		RenderState state;
		state.pass = NULL;
		state.flags = RenderPass::RenderOverlay;
		state.client = this;
		state.context = &context;

		getRenderer()->beginDraw2D(context);
	
		if(myServer->isConsoleEnabled())
		{
			myServer->getConsole()->getRenderable(this)->draw(&state);
		}
		myServer->drawPointers(this, &state);
	
		getRenderer()->endDraw();
	}
}

