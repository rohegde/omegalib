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
#include "omega/GpuManager.h"
#include "omega/EngineClient.h"
#include "omegaToolkit/ui/Image.h"
#include "omega/Renderer.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Image::Image(EngineServer* srv):
	Widget(srv),
	myAutoRefresh(false),
	myData(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
Image::~Image()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Image::createRenderable()
{
	return new ImageRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ImageRenderable::refresh()
{
	GpuManager* gpu = getClient()->getGpuContext()->getGpu();
	if(myOwner->myData != NULL)
	{
		PixelData* pd = myOwner->myData;
		if(myTexture == NULL)
		{
			myTexture = new Texture(getClient()->getGpuContext());
			myTexture->initialize(pd->getWidth(), pd->getHeight());
		}
		myTexture->writePixels(pd);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ImageRenderable::drawContent()
{
	WidgetRenderable::drawContent();

	if(myTexture != NULL)
	{
		// If autorefresh is enabled, refresh texture data every time you draw.
		// Not very efficient but works for now.
		if(myOwner->myAutoRefresh) refresh();

		getRenderer()->drawRectTexture(myTexture, Vector2f::Zero(), myOwner->getSize());
	}
	else
	{
		refresh();
	}
}