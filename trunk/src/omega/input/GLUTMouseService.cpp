/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "input/GLUTMouseService.h"
#include "GLUTDisplaySystem.h"
#include "SystemManager.h"
#include "Log.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLUTMouseService* GLUTMouseService::myInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTMouseService::mouseMotionCallback(int x, int y)
{
	myInstance->LockEvents();

	InputEvent* evt = myInstance->WriteHead();
	//	evt->id = OM_ID_MOUSE;
	//	evt->source = OM_DC_POINTER;
	//	evt->type = OM_EVENT_MOVE;
	evt->x = x;
	evt->y = y;

	myInstance->UnlockEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTMouseService::Initialize() 
{
	OASSERT(SystemManager::GetInstance()->GetDisplaySystem()->GetId() == GLUTDisplaySystem::Id);

	myInstance = this;
	glutPassiveMotionFunc(mouseMotionCallback);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLUTMouseService::Dispose() 
{
	myInstance = NULL;
}
