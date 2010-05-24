/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * This file contains system / platform defines and is included in every other file.
 *********************************************************************************************************************/
#ifndef __OSYSTEM_H__
#define __OSYSTEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#endif

// OpenGL includes
#include <GL/gl.h>
#include <GL/glut.h>

/**********************************************************************************************************************
 * The current omegalib version string.
 */
#define OM_VERSION "0.1"

#define OM_STDLEN 256 /* Standard string length */

/**********************************************************************************************************************
 * The device class identifies devices depending on their capabilities. Different devices, implemented with different
 * technologies and drivers, may belong to the same device class (i.e. PQLabs and TacTile are both multitouch devices).
 */
enum OMDeviceClass
{
	OM_DC_POINTER = 1,
	OM_DC_TOUCH = 1 << 1,
	OM_DC_MOCAP = 1 << 2
};

/**********************************************************************************************************************
 * Devices enum. A device has its own unique id here if it needs a specific driver. If two different physical devices
 * share the same omegalib driver, they will have a single device id here. Devices may also be non-physical (i.e. drivers
 * that convers or compose input, or socket drivers to access remote input streams).
 */
enum OMDevice
{
	OM_D_MOUSE,
	OM_D_TOUCH_PQLABS,
	OM_D_NETWORK,
	OM_D_POINTER_TO_TOUCH,
	OM_D_MOCAP_TO_POINTER
};

#endif