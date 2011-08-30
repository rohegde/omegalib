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
#include "oengine/FontManager.h"
#include "oengine/ui/UiManager.h"
#include "oengine/SceneManager.h"
#include "oengine/EffectManager.h"
#include "oengine/MeshManager.h"
#include "oengine/Renderer.h"

#include "omega/GpuManager.h"
#include "omega/TextureManager.h"
#include "omega/Texture.h"
#include "omega/glheaders.h"


using namespace omega;
using namespace oengine;
using namespace oengine::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::initialize()
{
	myGpuManager = getGpu();

	myFontManager = new FontManager();
	myTextureManager = new TextureManager();
	myUiManager = new UiManager();
	myFontManager = new FontManager();

	mySceneManager = new SceneManager(myGpuManager);
	myEffectManager = new EffectManager(myGpuManager);
	myMeshManager = new MeshManager(myGpuManager, myEffectManager);

	mySceneManager->initialize();

	//myUiManager->initialize(myClient);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::setTextureBackground(DrawContext::Eye eye, Texture* value)
{
	switch(eye)
	{
	case DrawContext::EyeLeft:
	case DrawContext::EyeCyclop:
		myLeftBackgroundTexture = value;
		break;
	case DrawContext::EyeRight:
		myRightBackgroundTexture = value;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
Texture* EngineClient::getTextureBackground(DrawContext::Eye eye)
{
	switch(eye)
	{
	case DrawContext::EyeLeft:
	case DrawContext::EyeCyclop:
		return myLeftBackgroundTexture;
	case DrawContext::EyeRight:
		return myRightBackgroundTexture;
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::update(const UpdateContext& context)
{
	myUiManager->update(context);
	mySceneManager->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::handleEvent(const Event& evt)
{
	myUiManager->handleEvent(evt);
	if(!evt.isProcessed())
	{
		return mySceneManager->handleEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::draw(const DrawContext& context)
{
	// first two bits = scene layer
	int sceneId = context.layer & 0x03;
	// second two bits = ui layer
	int overlayId = (context.layer >> 2) & 0x03;

	myGpuManager->beginDraw();
	// NOTE: Background rendering is active only for SCENE rendering.
	// When rendering overlays only, don't draw background again or it may delete whatever has been drawn before overlays.
	if(myTextureBackgroundEnabled && (sceneId != 0 || overlayId == 0))
	{
		drawBackGrd( context );
	}
	
	if(sceneId != 0)
	{
		mySceneManager->draw(context);
	}
	if(overlayId != 0)
	{
		myUiManager->draw(context);
	}
	myGpuManager->endDraw();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::drawBackGrd( const DrawContext& context )
{
	Vector2f position(0, 0);
	Vector2f size(context.viewport.width(), context.viewport.height());

	Renderer backGrdPainter;
	backGrdPainter.beginDraw2D(context);
	glEnable(GL_TEXTURE_2D);

	switch( context.eye )
	{
		case DrawContext::EyeLeft:
		oassert(myLeftBackgroundTexture);
		backGrdPainter.drawRectTexture( myLeftBackgroundTexture , position, size );
		break;
		case DrawContext::EyeRight:
		oassert(myRightBackgroundTexture);
		backGrdPainter.drawRectTexture( myRightBackgroundTexture , position, size );
		break;
		case DrawContext::EyeCyclop:
		oassert(myLeftBackgroundTexture);
		backGrdPainter.drawRectTexture( myLeftBackgroundTexture , position, size );
		break;
	}
	
	backGrdPainter.endDraw();
}

