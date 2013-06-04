/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __CAMERA_OUTPUT_H__
#define __CAMERA_OUTPUT_H__

#include "osystem.h"
#include "omega/ApplicationBase.h"
#include "omega/SceneNode.h"
#include "omega/RenderTarget.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API CameraOutput: public ReferenceType
	{
	public:
		CameraOutput(bool offscreen = false): myEnabled(false), myRenderTarget(NULL), myOffscreen(offscreen),
			myReadbackColorTarget(NULL), myReadbackDepthTarget(NULL)
		{}

		void beginDraw(const DrawContext& context);
		void endDraw(const DrawContext& context);
		void startFrame(const FrameInfo& frame);
		void finishFrame(const FrameInfo& frame);

		bool isEnabled() { return myEnabled; }
		void setEnabled(bool value) { myEnabled = value; }

		void setReadbackTarget(PixelData* color, PixelData* depth = NULL);
		void setReadbackTarget(PixelData* color, PixelData* depth, const Rect& readbackViewport);

		const Rect& getReadbackViewport() { return myReadbackViewport; }

		RenderTarget* getRenderTarget() { return myRenderTarget; }

		void lock() { myLock.lock();}
		void unlock() { myLock.unlock();}

	private:
		bool myEnabled;
		bool myOffscreen;
		RenderTarget* myRenderTarget;

		PixelData* myReadbackColorTarget;
		PixelData* myReadbackDepthTarget;
		Rect myReadbackViewport;

		Lock myLock;
	};
}; // namespace omega

#endif