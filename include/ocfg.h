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
	OM_DD_NONE,
	OM_DD_GL
};

typedef struct _OMConfig
{
	char appName[OM_STDLEN];

	enum OMDisplayDriver displayDriver;
	int height;
	int width;
} OMConfig;

void ocfgInitDefault(OMConfig* cfg);

#endif