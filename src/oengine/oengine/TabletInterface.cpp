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
#include "oengine/TabletInterface.h"
#include "oengine/ImageUtils.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletGuiElement* TabletGuiElement::createButton(int id, const String& label, const String& description, const String& text)
{
	TabletGuiElement* e = new TabletGuiElement();
	e->setId(id);
	e->setType(TabletGuiElement::ElementTypeButton);
	e->setLabel(label);
	e->setDescription(description);
	e->setText(text);
	return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletGuiElement* TabletGuiElement::createSlider(int id, const String& label, const String& description, int min, int max, int value)
{
	TabletGuiElement* e = new TabletGuiElement();
	e->setId(id);
	e->setType(TabletGuiElement::ElementTypeSlider);
	e->setLabel(label);
	e->setMinimum(min);
	e->setMaximum(max);
	e->setValue(value);
	return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletGuiElement* TabletGuiElement::createSwitch(int id, const String& label, const String& description, bool value)
{
	TabletGuiElement* e = new TabletGuiElement();
	e->setId(id);
	e->setType(TabletGuiElement::ElementTypeSwitch);
	e->setLabel(label);
	e->setValue(value ? 1 : 0);
	return e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const char* TabletGuiElement::getDef()
{
	switch(myType)
	{
	case ElementTypeButton:
		sprintf(myElemDef, "0:%d:%s:%s:%s:|", myId, myLabel.c_str(), myDescription.c_str(), myText.c_str());
		break;
	case ElementTypeSlider:
		sprintf(myElemDef, "1:%d:%s:%s:%d:%d:%d:|", myId, myLabel.c_str(), myDescription.c_str(), myMin, myMax, myValue);
		break;
	case ElementTypeSwitch:
		sprintf(myElemDef, "2:%d:%s:%s:%d:|", myId, myLabel.c_str(), myDescription.c_str(), myValue);
		break;
	}
	return myElemDef;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletInterface::TabletInterface(TabletService* svc, int tabletId):
	myService(svc), myTabletId(tabletId)
{
	myConnection = svc->getConnection(tabletId);
}

//#define DEBUG_PNG

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::sendImage(PixelData* data)
{
	ByteArray* png = ImageUtils::encode(data, ImageUtils::FormatPng);

#ifdef DEBUG_PNG
	static bool doOnce = false;
	if(!doOnce)
	{
		doOnce = true;
		FILE* fpng = fopen("./debug.png", "wb");
		fwrite(png->lock(), 1, png->getSize(), fpng);
		png->unlock();
		fclose(fpng);
	}
#endif

	int size = png->getSize();
	char header[] = {'i', 'p', 'n', 'g'};

	myConnection->write(header, 4);
	myConnection->write(&size, sizeof(int));
	myConnection->write(png->lock(), size);
	png->unlock();
	delete png;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::beginGui()
{
	memset(myGuiDef, 0, MaxGuiDefSize);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::finishGui()
{
	int size = strlen(myGuiDef);
	char header[] = {'m', 'g', 'u', 'i'};

	myConnection->write(header, 4);
	myConnection->write(&size, sizeof(int));
	myConnection->write(myGuiDef, size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::addGuiElement(TabletGuiElement* e)
{
	strcat(myGuiDef, e->getDef());
}
