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
#include "omouse.h"
#include "oevent.h"

/**********************************************************************************************************************
*/
void mouseMotionCallback(int x, int y)
{
	OMEvent* evt;

	oeventLock();

	// Write the event to the event list.
	evt = oeventWriteHead();
	evt->id = OM_ID_MOUSE;
	evt->source = OM_DC_POINTER;
	evt->type = OM_EVENT_MOVE;
	evt->x = x;
	evt->y = y;

	oeventUnlock();
}

/**********************************************************************************************************************
*/
void omouseInit(const OMConfig* cfg)
{
	glutPassiveMotionFunc(mouseMotionCallback);
}

/**********************************************************************************************************************
*/
void omouseCleanup()
{
}

/**********************************************************************************************************************
*/
void omouseStart()
{
}

/**********************************************************************************************************************
*/
void omouseStop()
{
}

