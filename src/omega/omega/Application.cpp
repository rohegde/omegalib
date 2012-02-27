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
#include "omega/Application.h"
#include "omega/DisplaySystem.h"
#include "omega/Lock.h"

#include "omega/StringUtils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Layer::Enum Layer::fromString(const String& str)
{
	String tmp = StringUtils::replaceAll(str, " ", "");
	String s = StringUtils::replaceAll(tmp, "ui", "overlay");
	StringUtils::toLowerCase(s);
	if(s == "scene0") return Scene0;
	if(s == "scene1") return Scene1;
	if(s == "scene2") return Scene2;
	if(s == "overlay0") return Overlay0;
	if(s == "scene0overlay0") return Scene0Overlay0;
	if(s == "scene1overlay0") return Scene1Overlay0;
	if(s == "scene2overlay0") return Scene2Overlay0;
	if(s == "overlay1") return Overlay1;
	if(s == "scene0overlay1") return Scene0Overlay1;
	if(s == "scene1overlay1") return Scene1Overlay1;
	if(s == "scene2overlay1") return Scene2Overlay1;
	if(s == "overlay2") return Overlay2;
	if(s == "scene0overlay2") return Scene0Overlay2;
	if(s == "scene1overlay2") return Scene1Overlay2;
	if(s == "scene2overlay2") return Scene2Overlay2;
	return Null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicationServer::addClient(ApplicationClient* cli)
{
	myClients.push_back(cli);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicationServer::getCanvasWidth() 
{
	return getDisplaySystem()->getCanvasSize().x(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicationServer::getCanvasHeight()
{
	return getDisplaySystem()->getCanvasSize().y(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ApplicationClient::ApplicationClient(ApplicationServer* server): myServer(server), myGpuContext(NULL)
{
	myServer->addClient(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ApplicationClient::~ApplicationClient() 
{
}

