/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Omegalib configuration
 *********************************************************************************************************************/
#ifndef __OCFG_H__
#define __OCFG_H__

#include "osystem.h"

enum OMDisplayDriver
{
	OM_DISPLAY_DRIVER_NONE,
	OM_DISPLAY_DRIVER_GL
};

typedef struct _OMConfig
{
	enum OMDisplayDriver displayDriver;
} OMConfig;

#endif