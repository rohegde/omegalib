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
#include "omega/EngineClient.h"
#include "omega/GpuManager.h"
#include "omega/FontManager.h"
#include "omega/TextureManager.h"
#include "omega/Texture.h"
#include "omega/ui/UIManager.h"
#include "omega/scene/SceneManager.h"
#include "omega/scene/EffectManager.h"
#include "omega/scene/MeshManager.h"
#include "omega/glheaders.h"
#include "omega/ui/Painter.h"


using namespace omega;
using namespace omega::ui;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::initialize()
{
	myGpuManager = getGpu();

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
void EngineClient::draw(const DrawContext& context)
{
	// first two bits = scene layer
	int sceneLayer = context.layer & 0x03;
	// second two bits = ui layer
	int uiLayer = (context.layer >> 2) & 0x03;

	myGpuManager->beginDraw();
	if(myTextureBackgroundEnabled)
	{
		drawBackGrd( context );
	}
	
	if(sceneLayer != 0)
	{
		mySceneManager->draw(context);
	}
	if(uiLayer != 0)
	{
		myUIManager->draw(context);
	}
	myGpuManager->endDraw();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineClient::drawBackGrd( const DrawContext& theContext )
{
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, theContext.viewport.height() - 1, 0.0);
    glScalef(1.0, -1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, theContext.viewport.width(), 0, theContext.viewport.height(), -1, 1);

    glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Vector2f position(0, 0);
	Vector2f size(theContext.viewport.width(), theContext.viewport.height());

	Painter backGrdPainter;
	switch( theContext.eye )
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
	//draw here
	
	glDisable (GL_BLEND);
	glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	
}

