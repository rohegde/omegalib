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
#ifndef __PORHOLE_TABLET_SERVICE_H__
#define __PORHOLE_TABLET_SERVICE_H__

#include "oenginebase.h"
#include "oengine/EngineServer.h"
#include "oengine/EngineApplication.h"
#include "oengine/Camera.h"
#include "omicron/BasicPortholeService.h"

namespace oengine {

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct OENGINE_API TabletGuiElement
	{
	public:
		enum ElementType { ElementTypeButton, ElementTypeSlider, ElementTypeSwitch };
		static const int MaxElemDefSize = 1024;
	
	public:
		static TabletGuiElement* createButton(int id, const String& label, const String& description, const String& text);
		static TabletGuiElement* createSlider(int id, const String& label, const String& description, int min, int max, int value);
		static TabletGuiElement* createSwitch(int id, const String& label, const String& description, bool value);

	public:

		ElementType getType() { return myType; }
		void setType(ElementType value) { myType = value; }

		int getId() { return myId; }
		void setId(int value) { myId = value; }

		const String& getLabel() { return myLabel; }
		void setLabel(const String& value) { myLabel = value; }

		const String& getDescription() { return myDescription; }
		void setDescription(const String& value) { myDescription = value; }

		const String& getText() { return myText; }
		void setText(const String& value) { myText = value; }

		int getMinimum() { return myMin; }
		void setMinimum(int value) { myMin = value; }

		int getMaximum() { return myMax; }
		void setMaximum(int value) { myMax = value; }

		int getValue() { return myValue; }
		void setValue(int value) { myValue = value; }

		const char* getDef();

	private:
		ElementType myType;
		int myId;
		String myLabel;
		String myDescription;
		String myText;
		int myMin;
		int myMax;
		int myValue;

		char myElemDef[MaxElemDefSize];
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements an interface to tablet device applications using the Porthole protocol.
	class OENGINE_API PortholeTabletService: public Service, IEngineModule
	{
	public:
		static const int MaxGuiDefSize = 1024;

		//! Allocator function (will be used to register the service inside SystemManager)
		static PortholeTabletService* New() { return new PortholeTabletService(); }

	public:
		PortholeTabletService();

		virtual void setup(Setting& settings);
		virtual void poll();
		void processEvent(Event* evt);

		virtual void initialize();
		virtual void initialize(EngineServer* engine);
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
		void refreshGuiDef();
		void sendMessage(TabletConnection* conn, const char* header, void* data, int size);

	private:
		EngineServer* myEngine;

		// Config properties
		bool myHires;
		bool myOffscreen;

		List<TabletConnection*> myTablets;
		BasicPortholeService* myTabletService;

		bool myEnabled;

		float myLastUpdateTime;
		float myAutoUpdateInterval;
		PixelData* myTabletPixels;
		Camera* myTabletCamera;
		uint myEventFlags;

		List<TabletGuiElement*> myGuiElements;
		char myGuiDef[MaxGuiDefSize];

	};
}; 

#endif