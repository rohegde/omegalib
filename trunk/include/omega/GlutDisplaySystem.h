/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __GLUT_DISPLAY_SYSTEM_H__
#define __GLUT_DISPLAY_SYSTEM_H__

#include "DisplaySystem.h"
#include "RenderTarget.h"
#include "GpuManager.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements a display system based on GLUT, offering a single render window and mouse input support.
	//! Note: this display system is not well maintained. It may not work / work partially. Developers are advised to
	//! use EqualizerDisplaySystem in most circumstances.
	class OMEGA_API GlutDisplaySystem: public DisplaySystem
	{
	public:
		GlutDisplaySystem();
		virtual ~GlutDisplaySystem();

		// sets up the display system. Called before initalize.
		void setup(Setting& setting);

		virtual void initialize(SystemManager* sys); 
		virtual void run(); 
		virtual void cleanup(); 

		void updateProjectionMatrix();

		// Layer and view management.
		virtual void setLayer(const char* viewName, Layer::Enum layer);
		virtual Layer::Enum getLayer(const char* viewName);

		Observer& getObserver() { return myObserver; }

		DisplaySystemType getId() { return DisplaySystem::Glut; }

		ApplicationServer* getApplicationServer() { return myAppServer; }
		ApplicationClient* getApplicationClient() { return myAppClient; }

		Vector2i getCanvasSize(const String& canvasName) { return myResolution; }
	private:
		void initLayers();
		void initObservers();

	private:
		// Display config
		Setting* mySetting;
		Vector2i myResolution;
		Observer myObserver;
		int myFov;
		float myAspect;
		double myNearz;
		double myFarz;

		SystemManager* mySys;
		ApplicationClient* myAppClient;
		ApplicationServer* myAppServer;
		RenderTarget* myFrameBuffer;
		Layer::Enum myLayer;
	};
}; // namespace omega

#endif