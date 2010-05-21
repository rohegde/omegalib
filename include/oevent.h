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
#ifndef __OEVENT_H__
#define __OEVENT_H__

#include "osystem.h"

enum OMEventType
{
	OM_EVENT_MOVE,
	OM_EVENT_DOWN,
	OM_EVENT_UP,
	OM_EVENT_TRACE,
	OM_EVENT_UNTRACE,
};

typedef struct _OMEvent
{
    unsigned int id;
	unsigned int flags;
    enum OMEventType type;
    enum OMDevice device;
    float x;
	float y;
	float z;
    float rx;
	float ry;
	float rz;
} OMEvent;


#endif