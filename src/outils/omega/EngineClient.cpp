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
#include "omega/Application.h"
#include "omega/EngineClient.h"
#include "omega/GpuManager.h"
#include "omega/FontManager.h"
#include "omega/TextureManager.h"
#include "omega/ui/UIManager.h"
#include "omega/scene/SceneManager.h"
#include "omega/scene/EffectManager.h"
#include "omega/scene/MeshManager.h"

using namespace omega;
using namespace omega::ui;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::initialize()
{
	myGpuManager = myClient->getGpu();

	myFontManager = new FontManager();
	myTextureManager = new TextureManager();
	myUIManager = new UIManager();
	myFontManager = new FontManager();

	mySceneManager = new SceneManager(myGpuManager);
	myEffectManager = new EffectManager(myGpuManager);
	myMeshManager = new MeshManager(myGpuManager, myEffectManager);

	mySceneManager->initialize();

	//myUIManager->initialize(myClient);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::update(const UpdateContext& context)
{
	myUIManager->update(context);
	mySceneManager->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool EngineClient::handleEvent(const Event& evt, UpdateContext& context)
{
	return mySceneManager->handleEvent(evt, context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool EngineClient::handleEvent(const Event& evt, DrawContext& context)
{
	if(!myUIManager->processInputEvent(evt))
	{
		return mySceneManager->handleEvent(evt, context);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::draw(const DrawContext& context, uint flags)
{
	myGpuManager->beginDraw();
	if((flags & DrawScene) == DrawScene)
	{
		mySceneManager->draw(context);
	}
	if((flags & DrawUI) == DrawUI)
	{
		myUIManager->draw(context);
	}
	myGpuManager->endDraw();
}
