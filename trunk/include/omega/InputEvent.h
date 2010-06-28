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
#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include "osystem.h"
#include "InputService.h"

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct InputEvent
{
	// Supported event types.
	enum Type {Move, Down, Up, Trace, Untrace};

	// id of the source of this event. Input services associate unique ids to each of their event sources.
    unsigned int sourceId;
	
	// Type of the service that generated this event.
	enum InputService::ServiceType serviceType;

	// Unique id of the service that generated this event.
	int serviceId;

	// The event type.
    enum Type type;

	// Event flags.
	unsigned int flags;

	// Event timestamp.
	unsigned int timestamp;

	// Position.
    float x;
	float y;
	float z;

	// Rotation.
	float rw;
    float rx;
	float ry;
	float rz;
};

}; // namespace omega

#endif