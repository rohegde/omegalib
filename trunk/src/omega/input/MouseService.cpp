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
		//	evt->id = OM_ID_MOUSE;
		//	evt->source = OM_DC_POINTER;
		//	evt->type = OM_EVENT_MOVE;
		evt->serviceType = InputService::Pointer;
		evt->type = InputEvent::Move;
		evt->x = x;
		evt->y = y;

		mysInstance->unlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::initialize() 
{
	mysInstance = this;
	if(SystemManager::instance()->getDisplaySystem()->getId() == GLUTDisplaySystem::Id)
	{
		glutPassiveMotionFunc(mouseMotionCallback);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::dispose() 
{
	mysInstance = NULL;
}
