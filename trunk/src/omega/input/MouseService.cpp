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
MouseService* MouseService::myInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::mouseMotionCallback(int x, int y)
{
	if(myInstance)
	{
		myInstance->LockEvents();

		InputEvent* evt = myInstance->WriteHead();
		//	evt->id = OM_ID_MOUSE;
		//	evt->source = OM_DC_POINTER;
		//	evt->type = OM_EVENT_MOVE;
		evt->serviceId = InputService::Pointer;
		evt->type = InputEvent::Move;
		evt->x = x;
		evt->y = y;

		myInstance->UnlockEvents();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::Initialize() 
{
	myInstance = this;
	if(SystemManager::GetInstance()->GetDisplaySystem()->GetId() == GLUTDisplaySystem::Id)
	{
		glutPassiveMotionFunc(mouseMotionCallback);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MouseService::Dispose() 
{
	myInstance = NULL;
}
