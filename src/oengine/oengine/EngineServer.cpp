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
#include "oengine/EngineServer.h"
#include "oengine/LightingPass.h"
#include "oengine/OverlayRenderPass.h"
#include "oengine/DefaultRenderPass.h"
#include "omega/StringUtils.h"

using namespace omega;
using namespace oengine;
//using namespace oengine::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
EngineServer::EngineServer(Application* app):
	ApplicationServer(app)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::addRenderPass(String renderPass, bool addToFront)
{
	RenderPassFactory rpNew = myRenderPassFactories[renderPass];
	if(rpNew != NULL)
	{
		foreach(EngineClient* client, myClients)
		{
			client->addRenderPass(rpNew(), addToFront);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::removeRenderPass(String renderPass)
{
	oassert(false); // Not implemented.
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::addClient(EngineClient* client)
{
	ofmsg("EngineServer::addClient: id = %1%", %client->getId());

	oassert(client != NULL);
	myClients.push_back(client);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::initialize()
{
	for(int i = 0; i < MaxScenes; i++)
	{
		myScene[i] = new SceneNode(this, "root");
	}
	for(int i = 0; i < MaxUis; i++)
	{
		//myUi[i] = new Container(this);
	}

	// Setup default render chain.
	registerRenderPassClass<LightingPass>();
	registerRenderPassClass<OverlayRenderPass>();
	registerRenderPassClass<DefaultRenderPass>();

	addRenderPass("LightingPass");
	addRenderPass("DefaultRenderPass");
	addRenderPass("OverlayRenderPass");
}

///////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* EngineServer::getScene(int id)
{
	oassert(id >= 0 && id < EngineServer::MaxScenes);
	return myScene[id];
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Container* EngineServer::getUi(int id)
//{
//	oassert(id >= 0 && id < EngineServer::MaxUis);
//	return myUi[id];
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::update(const UpdateContext& context)
{
	// Update actors.
	//foreach(Actor* a, myActors)
	//{
	//	a->update(context);
	//}

	// Update scene.
	for(int i = 0; i < MaxScenes; i++)
	{
		myScene[i]->update(false, false);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::handleEvent(const Event& evt)
{
/*	myUiManager->handleEvent(evt);
	if(!evt.isProcessed())
	{
		return mySceneManager->handleEvent(evt);
	}*/
}

