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
#ifndef __TABLET_MANAGER_MODULE_H__
#define __TABLET_MANAGER_MODULE_H__

#include "oenginebase.h"
#include "oengine/TabletInterface.h"
#include "oengine/EngineServer.h"
#include "omega/TabletService.h"

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API TabletManagerModule: public Service
	{
	public:
		TabletManagerModule();

		// The tablet manager is implemented as an event service because it needs to intercept 
		// tablet events and process them before they get delivered to the application.
		virtual void poll();
		void processEvent(Event* evt);

		void initialize(EngineServer* engine, bool hires = true, bool offscreen = true);
		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		void beginGui();
		void addGuiElement(TabletGuiElement* elem);
		void finishGui();

		Camera* getCamera() { return myTabletCamera; }

		void setEnabled(bool value) { myEnabled = value; }
		bool isEnabled() { return myEnabled; }

		uint getEventFlags() { return myEventFlags; }
		void setEventFlags(uint value) { myEventFlags = value; }

	private:
		EngineServer* myEngine;

		uint myEventFlags;

		TabletService* myTabletService;

		bool myEnabled;

		float myLastUpdateTime;
		float myAutoUpdateInterval;
		PixelData* myTabletPixels;
		Camera* myTabletCamera;

		List<TabletGuiElement*> myGuiElements;

		List<TabletInterface*> myTablets;
	};
}; 

#endif