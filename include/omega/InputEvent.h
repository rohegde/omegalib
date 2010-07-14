/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include <vector>
#include "osystem.h"
#include "InputService.h"

namespace omega
{

//will be used to store point data for the markers that make up MoCap rigid bodies
struct Point
{
	float x;
	float y;
	float z;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct InputEvent
{
	// Supported event types.
	enum Type 
	{ 
		// Move: Generated whenever the source of an event moves.
		Move, 
		// Down: generated when the source of an event goes to a logical 'down' state (i.e. touch on a surface or 
		// a mouse button press count as Down events)
		Down, 
		// Up: generated when the source of an event goes to a logical 'up' state (i.e. remove touch from a surface or 
		// a mouse button release count as Up events)
		Up, 
		// Trace: generated when a new object is identified by the device managed by the input service 
		// (i.e head tracking, or a mocap system rigid body).
		Trace, 
		// Trace: generated when a traced object is lost by the device managed by the input service 
		// (i.e head tracking, or a mocap system rigid body).
		Untrace
	};

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

	//Point set
	int numberOfPoints;
	Vector3f pointSet[32];
};

}; // namespace omega

#endif