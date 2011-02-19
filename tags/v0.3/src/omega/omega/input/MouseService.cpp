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
#include "omega/input/MouseService.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MouseService* MouseService::mysInstance = NULL;
unsigned int sButtonFlags = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::mouseMotionCallback(int x, int y)
{
	if(mysInstance)
	{
		mysInstance->lockEvents();

		InputEvent* evt = mysInstance->writeHead();
		evt->serviceType = InputService::Pointer;
		evt->type = InputEvent::Move;
		evt->position[0] = x;
		evt->position[1] = y;
		evt->flags = sButtonFlags;

		mysInstance->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::mouseButtonCallback(int button, int state, int x, int y)
{
	if(mysInstance)
	{
		mysInstance->lockEvents();

		InputEvent* evt = mysInstance->writeHead();
		evt->serviceType = InputService::Pointer;
#ifdef OMEGA_USE_DISPLAY_GLUT
		// Glut mouse callback button states are inverted wrt equalizer callbacks.
		if(SystemManager::instance()->getDisplaySystem()->getId() == DisplaySystem::Glut)
		{
			evt->type = state ? InputEvent::Up : InputEvent::Down;

			// Update button flags
			if(evt->type == InputEvent::Down)
			{
				if(button == GLUT_LEFT_BUTTON) sButtonFlags |= InputEvent::Left;
				if(button == GLUT_RIGHT_BUTTON) sButtonFlags |= InputEvent::Right;
				if(button == GLUT_MIDDLE_BUTTON) sButtonFlags |= InputEvent::Middle;
			}
			else
			{
				if(button == GLUT_LEFT_BUTTON) sButtonFlags &= ~InputEvent::Left;
				if(button == GLUT_RIGHT_BUTTON) sButtonFlags &= ~InputEvent::Right;
				if(button == GLUT_MIDDLE_BUTTON) sButtonFlags &= ~InputEvent::Middle;
			}
		}
		else
#endif
		{
			evt->type = state ? InputEvent::Down : InputEvent::Up;
			//if(evt->type == InputEvent::Down)
			{
				sButtonFlags = button;
			}
			//else
			{
			//	sButtonFlags &= ~button;
			}
		}
		evt->position[0] = x;
		evt->position[1] = y;
		evt->flags = sButtonFlags;

		mysInstance->unlockEvents();
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
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::dispose() 
{
	mysInstance = NULL;
}
