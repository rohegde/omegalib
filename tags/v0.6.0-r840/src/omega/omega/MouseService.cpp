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
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omega/MouseService.h"

#ifdef OMEGA_USE_DISPLAY_GLUT
#include "GL/freeglut.h"
#endif

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MouseService* MouseService::mysInstance = NULL;
unsigned int sButtonFlags = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MouseService::serverX = 1;
int MouseService::serverY = 1; 
int MouseService::screenX = 1;
int MouseService::screenY = 1;
int MouseService::screenOffsetX = 0; 
int MouseService::screenOffsetY = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::mouseWheelCallback(int btn, int wheel, int x, int y)
{
	if(mysInstance)
	{
		mysInstance->lockEvents();

		Event* evt = mysInstance->writeHead();
		evt->reset(Event::Zoom, Service::Pointer);
		evt->setPosition(x, y);

		evt->setExtraDataType(Event::ExtraDataIntArray);
		evt->setExtraDataInt(0, wheel);

		mysInstance->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::mouseMotionCallback(int x, int y)
{
	oassert(serverX != 0 && serverY != 0);

	if(mysInstance)
	{
		mysInstance->lockEvents();

		x = x * screenX / serverX + screenOffsetX;
		y = y * screenY / serverY + screenOffsetY;

		Event* evt = mysInstance->writeHead();
		evt->reset(Event::Move, Service::Pointer);
		evt->setPosition(x, y);
		evt->setFlags(sButtonFlags);

		mysInstance->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::mouseButtonCallback(int button, int state, int x, int y)
{
	oassert(serverX != 0 && serverY != 0);

	if(mysInstance)
	{
		mysInstance->lockEvents();

		x = x * screenX / serverX + screenOffsetX;
		y = y * screenY / serverY + screenOffsetY;

		Event* evt = mysInstance->writeHead();
#ifdef OMEGA_USE_DISPLAY_GLUT
		// Glut mouse callback button states are inverted wrt equalizer callbacks.
		if(SystemManager::instance()->getDisplaySystem()->getId() == DisplaySystem::Glut)
		{
			evt->reset(state ? Event::Up : Event::Down, Service::Pointer);

			if(button == 3) mouseWheelCallback(button, 1, x, y);
			if(button == 4) mouseWheelCallback(button, -1, x, y);

			// Update button flags
			if(evt->getType() == Event::Down)
			{
				if(button == GLUT_LEFT_BUTTON) sButtonFlags |= Event::Left;
				if(button == GLUT_RIGHT_BUTTON) sButtonFlags |= Event::Right;
				if(button == GLUT_MIDDLE_BUTTON) sButtonFlags |= Event::Middle;
			}
			else
			{
				if(button == GLUT_LEFT_BUTTON) sButtonFlags &= ~Event::Left;
				if(button == GLUT_RIGHT_BUTTON) sButtonFlags &= ~Event::Right;
				if(button == GLUT_MIDDLE_BUTTON) sButtonFlags &= ~Event::Middle;
			}
		}
		else
#endif
		{
			evt->reset(state ? Event::Down : Event::Up, Service::Pointer);
			sButtonFlags = button;
		}
		evt->setPosition(x, y);
		evt->setFlags(sButtonFlags);

		mysInstance->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::setup(Setting& settings)
{
	if(settings.exists("serverX"))
	{
		serverX =  settings["serverX"];
	}
	if(settings.exists("serverY"))
	{
		serverY =  settings["serverY"];
	}
	if(settings.exists("screenX"))
	{
		screenX =  settings["screenX"];
	}
	if(settings.exists("screenY"))
	{
		screenY =  settings["screenY"];
	}
	if(settings.exists("screenOffsetX"))
	{
		screenOffsetX =  settings["screenOffsetX"];
	}
	if(settings.exists("screenOffsetY"))
	{
		screenOffsetY =  settings["screenOffsetY"];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::initialize() 
{
	mysInstance = this;
#ifdef OMEGA_USE_DISPLAY_GLUT
	if(SystemManager::instance()->getDisplaySystem()->getId() == DisplaySystem::Glut)
	{
		glutPassiveMotionFunc(mouseMotionCallback);
		glutMotionFunc(mouseMotionCallback);
		glutMouseFunc(mouseButtonCallback);
		glutMouseWheelFunc(mouseWheelCallback);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::dispose() 
{
	mysInstance = NULL;
}
