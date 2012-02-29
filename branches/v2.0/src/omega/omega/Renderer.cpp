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
#include "omega/Renderer.h"
#include "omega/ServerEngine.h"
#include "omega/MasterEngine.h"
#include "omega/PortholeTabletService.h"

#include "omega/DisplaySystem.h"
#include "omega/GpuManager.h"
#include "omega/Texture.h"
#include "omega/glheaders.h"

// Uncomment to print debug messages about client flow.
//#define OMEGA_DEBUG_FLOW

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderer::Renderer(ServerBase* server):
	RendererBase(server)
{
	myRenderer = new DrawInterface();
	myServer = (ServerEngine*)server;
	myServer->addClient(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::addRenderPass(RenderPass* pass, bool addToFront)
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
void Renderer::removeRenderPass(RenderPass* pass)
{
	myRenderPassList.remove(pass);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::removeAllRenderPasses()
{
	myRenderPassList.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
RenderPass* Renderer::getRenderPass(const String& name)
{
	foreach(RenderPass* rp, myRenderPassList)
	{
		if(rp->getName() == name) return rp;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::initialize()
{
	ofmsg("@Renderer::Initialize: id = %1%", %getGpuContext()->getId());

	// Create the default font.
	const FontInfo& fi = myServer->getDefaultFont();
	if(fi.size != 0)
	{
		Font* fnt = myRenderer->createFont(fi.name, fi.filename, fi.size);
		myRenderer->setDefaultFont(fnt);
	}

	myServer->clientInitialize(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::queueRenderableCommand(RenderableCommand& cmd)
{
	myRenderableCommands.push(cmd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::startFrame(const FrameInfo& frame)
{
#ifdef OMEGA_DEBUG_FLOW
	ofmsg("Renderer::startFrame %1%", %frame.frameNum);
#endif

	MasterEngine* master = myServer->asMaster();
	if(master != NULL)
	{
		foreach(Camera* cam, master->getCameras())
		{
			cam->startFrame(frame);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::finishFrame(const FrameInfo& frame)
{
#ifdef OMEGA_DEBUG_FLOW
	ofmsg("Renderer::finishFrame %1%", %frame.frameNum);
#endif

	MasterEngine* master = myServer->asMaster();
	if(master != NULL)
	{
		foreach(Camera* cam, master->getCameras())
		{
			cam->finishFrame(frame);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::draw(const DrawContext& context)
{
#ifdef OMEGA_DEBUG_FLOW
	String eyeName = "Cyclops";
	if(context.eye == DrawContext::EyeLeft) eyeName = "EyeLeft";
	else if(context.eye == DrawContext::EyeRight) eyeName = "EyeRight";
	String task = "Scene";
	if(context.task == DrawContext::OverlayDrawTask) task = "Overlay";
	ofmsg("Renderer::draw frame=%1% task=%2% eye=%3%", %context.frameNum %task %eyeName);
#endif

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

	MasterEngine* master = myServer->asMaster();
	if(master != NULL)
	{
		// On the master server, Perform draw for the additional enabled cameras.
		foreach(Camera* cam, master->getCameras())
		{
			// See if camera is enabled for the current client and draw context.
			if(cam->isEnabled(context))
			{
				// Begin drawing with the camera: get the camera draw context.
				const DrawContext& cameraContext = cam->beginDraw(context);
				innerDraw(cameraContext);
				cam->endDraw(context);
			}
		}
	}

	// Draw once for the default camera (using the passed main draw context).
	innerDraw(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::innerDraw(const DrawContext& context)
{
	// Execute all render passes in order. 
	foreach(RenderPass* pass, myRenderPassList)
	{
		pass->render(this, context);
	}

	// Draw the pointers and console
	if(context.task == DrawContext::OverlayDrawTask && 
		context.eye == DrawContext::EyeCyclop)
	{
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

