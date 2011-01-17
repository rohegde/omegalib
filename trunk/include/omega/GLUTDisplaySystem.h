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

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements a display system based on GLUT, offering a single render window and mouse input support.
	class GlutDisplaySystem: public DisplaySystem
	{
	public:
		OMEGA_API GlutDisplaySystem();
		OMEGA_API virtual ~GlutDisplaySystem();

		// sets up the display system. Called before initalize.
		OMEGA_API void setup(Setting& setting);

		OMEGA_API virtual void initialize(SystemManager* sys); 
		OMEGA_API virtual void run(); 
		OMEGA_API virtual void cleanup(); 

		// Layer and view management.
		OMEGA_API virtual void setLayerEnabled(int layerNum, const char* viewName, bool enabled);
		OMEGA_API virtual bool isLayerEnabled(int layerNum, const char* viewName);

		Observer& getObserver() { return myObserver; }

		DisplaySystemType getId() { return DisplaySystem::Glut; }

		ApplicationServer* getApplicationServer() { return myAppServer; }
		ApplicationClient* getApplicationClient() { return myAppClient; }

	private:
		void initLayers();
		void initObservers();

	private:
		// Display config
		Setting* mySetting;
		Vector2i myResolution;
		Observer myObserver;

		SystemManager* mySys;
		ApplicationClient* myAppClient;
		ApplicationServer* myAppServer;
		bool* myLayerEnabled;
	};
}; // namespace omega

#endif