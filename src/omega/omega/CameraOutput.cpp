/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/CameraOutput.h"
#include "omega/Camera.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::setReadbackTarget(PixelData* color, PixelData* depth)
{
	myReadbackColorTarget = color;
	myReadbackDepthTarget = depth;
	if(myReadbackColorTarget != NULL)
	{
		myReadbackViewport = Rect(
			0, 0, 
			myReadbackColorTarget->getWidth(), myReadbackColorTarget->getHeight());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::setReadbackTarget(PixelData* color, PixelData* depth, const Rect& readbackViewport)
{
	setReadbackTarget(color, depth);
	myReadbackViewport = readbackViewport;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::beginDraw(const DrawContext& context)
{
	if(myRenderTarget == NULL)
	{
		if(myOffscreen) 
		{
			myRenderTarget = new RenderTarget(context.gpuContext, RenderTarget::RenderOffscreen);
		}
		else
		{
			myRenderTarget = new RenderTarget(context.gpuContext, RenderTarget::RenderOnscreen);
		}
	}
	myRenderTarget->setReadbackTarget(myReadbackColorTarget, myReadbackDepthTarget, myReadbackViewport);
	myRenderTarget->bind();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::endDraw(const DrawContext& context)
{
	if(myRenderTarget != NULL)
	{
		// HACK
		myRenderTarget->readback();

		myRenderTarget->unbind();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::startFrame(const FrameInfo& frame)
{
	if(myRenderTarget != NULL)
	{
		myRenderTarget->clear();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraOutput::finishFrame(const FrameInfo& frame)
{
	if(myRenderTarget != NULL)
	{
		// HACK
		//myRenderTarget->readback();
	}
}

