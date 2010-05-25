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
	OM_DD_GL,
	OM_DD_EQUALIZER
};

enum OMGLDisplayFlags
{
	OM_GDF_NONE = 0,
	OM_GDF_STEREO = 1
};

typedef struct _OMConfig
{
	char appName[OM_STDLEN];

	enum OMDisplayDriver displayDriver;

	// OpenGl driver-specific options
	int height;
	int width;
	enum OMGLDisplayFlags glDisplayFlags;

	// Equalizer driver-specific options
	char displayCfgName[OM_STDLEN];
} OMConfig;

void ocfgInitDefault(OMConfig* cfg);

#endif