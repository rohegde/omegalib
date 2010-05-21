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

/**********************************************************************************************************************
 * Devices enum. A device has its own unique id here if it needs a specific driver. If two different physical devices
 * share the same omegalib driver, they will have a single device id here.
 */
enum OMDevice
{
	OM_DEVICE_MOUSE,
	OM_DEVICE_MULTITOUCH,
	OM_DEVICE_TRACKER,
	OM_DEVICE_COMPOSITE
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



#endif