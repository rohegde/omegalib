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
#ifndef __TABLET_INTERFACE_H__
#define __TABLET_INTERFACE_H__

#include "oenginebase.h"
#include "omega/TabletService.h"
#include "omega/PixelData.h"

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
	class OENGINE_API TabletInterface: public OmegaObject
	{
	public:
		static const int MaxGuiDefSize = 1024;

	public:
		TabletInterface(TabletService* service, int tabletId);

		void sendImage(PixelData* data);

		void beginGui();
		void finishGui();
		void addGuiElement(TabletGuiElement* e);

	private:
		TabletService* myService;
		int myTabletId;
		TcpConnection* myConnection;
		
		char myGuiDef[MaxGuiDefSize];
	};
}; 

#endif