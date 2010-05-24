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
#ifndef __OMEGA_H__
#define __OMEGA_H__

#include "osystem.h"
#include "ocfg.h"
#include "oevent.h"
#include "olog.h"

/**********************************************************************************************************************
*/
enum OMGetEventsFlags
{
	OM_GEF_FIFO = 1
};

/**********************************************************************************************************************
*/
void omegaInit(const OMConfig* cfg);

/**********************************************************************************************************************
*/
void omegaSetUpdateCallback(void (*func)(float));

/**********************************************************************************************************************
*/
void omegaRun();

/**********************************************************************************************************************
*/
int omegaAvailableEvents();

/**********************************************************************************************************************
*/
int omegaLostEvents();

/**********************************************************************************************************************
*/
int omegaGetEvents(OMEvent* ptr, int maxEvent, unsigned int flags);

#endif