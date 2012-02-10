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
#include "omega/RenderTarget.h"
#include "omega/Texture.h"
#include "omega/glheaders.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
RenderTarget::RenderTarget():
	myWidth(0),
	myHeight(0),
	myId(0),
	myColorTarget(NULL),
	myInitialized(false),
	myDrawing(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
void RenderTarget::setColorTarget(Texture* target) 
{
	myColorTarget = target; 
	myWidth = myColorTarget->getWidth();
	myHeight = myColorTarget->getHeight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RenderTarget::initialize(Type type, int width, int height)
{
	myWidth = width;
	myHeight = height;
	myType = type;

	if(type == TypeRenderBuffer || type == TypeTexture)
	{
		glGenFramebuffers(1, &myId);
	}

	myInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RenderTarget::endDraw()
{
	myDrawing = false;
	if(myColorTarget != NULL)
	{
		if(myId == 0)
		{
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, myColorTarget->getWidth(), myColorTarget->getHeight(), 0);
			myColorTarget->unbind();
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RenderTarget::beginDraw()
{
	myDrawing = true;
	if(myColorTarget != NULL)
	{
		if(myId == 0)
		{
			myColorTarget->bind(GpuManager::TextureUnit0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, myId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myColorTarget->getGLTexture(), 0);
		}
	}
}
