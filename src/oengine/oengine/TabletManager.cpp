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
TabletInterface::TabletInterface(TabletService* svc, int tabletId):
	myService(svc), myTabletId(tabletId)
{
	myConnection = svc->getConnection(tabletId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::sendImage(PixelData* data)
{
	ByteArray* png = ImageUtils::encode(data, ImageUtils::FormatPng);

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
void TabletInterface::addButton(int id, const String& label, const String& description, const String& text)
{
	char msg[MaxGuiDefSize];
	sprintf(msg, "0:%d:%s:%s:%s:|", id, label.c_str(), description.c_str(), text.c_str());
	strcat(myGuiDef, msg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::addSlider(int id, const String& label, const String& description, int min, int max, int value)
{
	char msg[MaxGuiDefSize];
	sprintf(msg, "1:%d:%s:%s:%d:%d:%d:|", id, label.c_str(), description.c_str(), min, max, value);
	strcat(myGuiDef, msg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletInterface::addSwitch(int id, const String& label, const String& description, bool value)
{
	char msg[MaxGuiDefSize];
	sprintf(msg, "2:%d:%s:%s:%d:|", id, label.c_str(), description.c_str(), value ? 1 : 0);
	strcat(myGuiDef, msg);
}
