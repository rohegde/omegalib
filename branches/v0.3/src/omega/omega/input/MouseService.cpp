/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * MouseService method definitions. See MouseService.h for more details.
 *********************************************************************************************************************/
#include "omega/GLUTDisplaySystem.h"
#include "omega/SystemManager.h"
#include "omega/input/MouseService.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MouseService* MouseService::mysInstance = NULL;

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
		/*if(SystemManager::instance()->getDisplaySystem()->getId() == GLUTDisplaySystem::Id)
		{
			evt->type = InputEvent::Down;
			evt->position[0] = x;
			evt->position[1] = y;
		}
		else*/
		{
			evt->type = state ? InputEvent::Down : InputEvent::Up;
			evt->position[0] = x;
			evt->position[1] = y;
		}

		mysInstance->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::initialize() 
{
	mysInstance = this;
	/*if(SystemManager::instance()->getDisplaySystem()->getId() == GLUTDisplaySystem::Id)
	{
		glutPassiveMotionFunc(mouseMotionCallback);
		glutMouseFunc(mouseButtonCallback);
	}*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::dispose() 
{
	mysInstance = NULL;
}
