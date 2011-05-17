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

#ifdef OMEGA_USE_DISPLAY
#include "omega/GpuManager.h"
#endif

#include "omega/StringUtils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Layer::Enum Layer::fromString(const String& str)
{
	String s = StringUtils::replaceAll(str, " ", "");
	StringUtils::toLowerCase(s);
	if(s == "scene0") return Scene0;
	if(s == "scene1") return Scene1;
	if(s == "scene2") return Scene2;
	if(s == "ui0") return UI0;
	if(s == "scene0ui0") return Scene0UI0;
	if(s == "scene1ui0") return Scene1UI0;
	if(s == "scene2ui0") return Scene2UI0;
	if(s == "ui1") return UI1;
	if(s == "scene0ui1") return Scene0UI1;
	if(s == "scene1ui1") return Scene1UI1;
	if(s == "scene2ui1") return Scene2UI1;
	if(s == "ui2") return UI2;
	if(s == "scene0ui2") return Scene0UI2;
	if(s == "scene1ui2") return Scene1UI2;
	if(s == "scene2ui2") return Scene2UI2;
	return Null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ApplicationClient::ApplicationClient(Application* app): myApplication(app) 
{
#ifdef OMEGA_USE_DISPLAY
	myGpu = new GpuManager();
#else
	myGpu = NULL;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ApplicationClient::~ApplicationClient() 
{
#ifdef OMEGA_USE_DISPLAY
	delete myGpu;
	myGpu = NULL;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicationClient::initialize() 
{
#ifdef OMEGA_USE_DISPLAY
	myGpu->initialize();
#endif
}
