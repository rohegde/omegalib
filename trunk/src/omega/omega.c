/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * omega functions implementation. See omega.h for more details.
 *********************************************************************************************************************/
#include "omega.h"
#include "oinput.h"
#include "oevent.h"
#include "display/ogl.h"

#include <libconfig.h>

/**********************************************************************************************************************
 * Module globals
 */
OMConfig gCfg;
void (*gUpdateCallback)(float dt) = NULL;

/**********************************************************************************************************************
*/
void omegaSetUpdateCallback(void (*func)(float))
{
	gUpdateCallback = func;
}

/**********************************************************************************************************************
*/
void omegaInit(const OMConfig* cfg)
{
	memcpy(&gCfg, cfg, sizeof(OMConfig));

	if(gCfg.displayDriver == OM_DD_EQUALIZER)
	{
		oeqInit(&gCfg);
	}
	else
	{
		oglInit(&gCfg);
	}
	oinputInit(&gCfg);
}

/**********************************************************************************************************************
*/
void omegaCleanup()
{
	oinputCleanup();
}

/**********************************************************************************************************************
*/
void omegaRun()
{
	oinputStart();

	if(gCfg.displayDriver == OM_DD_EQUALIZER)
	{
		oeqStart();
	}
	else
	{
		oglStart();
	}
}

/**********************************************************************************************************************
*/
void omegaRequestExit()
{
}

/**********************************************************************************************************************
*/
int omegaAvailableEvents()
{
	return oeventGetAvailableEvents();
}

/**********************************************************************************************************************
*/
int omegaLostEvents()
{
	int lostEvts = oeventGetDroppedEvents();
	oeventResetDroppedEvents();
	return lostEvts;
}

/**********************************************************************************************************************
*/
int omegaGetEvents(OMEvent* ptr, int maxEvents, unsigned int flags)
{
	int returnedEvents = 0;
	OMEvent* evt;
	
	oeventLock();
	do
	{
		if(flags == OM_GEF_FIFO) evt = oeventReadHead();
		else evt = oeventReadTail();
		if(evt)	
		{
			memcpy(&ptr[returnedEvents], evt, sizeof(OMEvent));
			returnedEvents++;
		}
	} while(evt && returnedEvents < maxEvents);
	oeventUnlock();

	return returnedEvents;
}