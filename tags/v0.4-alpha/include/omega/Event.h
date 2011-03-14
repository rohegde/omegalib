/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include "osystem.h"
#include "Service.h"

#ifdef OMEGA_USE_DISPLAY_EQUALIZER
// Equalizer includes
#include <eq/eq.h>
#endif

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	struct Event: public DynamicObject
	{
		Event();
		//! Supported event types.
		enum Type 
		{ 
			//! Move: Generated whenever the source of an event moves.
			Move, 
			//! Down: generated when the source of an event goes to a logical 'down' state (i.e. touch on a surface or 
			//! a mouse button press count as Down events)
			Down, 
			//! Up: generated when the source of an event goes to a logical 'up' state (i.e. remove touch from a surface or 
			//! a mouse button release count as Up events)
			Up, 
			//! Trace: generated when a new object is identified by the device managed by the input service 
			//! (i.e head tracking, or a mocap system rigid body).
			Trace, 
			//! Trace: generated when a traced object is lost by the device managed by the input service 
			//! (i.e head tracking, or a mocap system rigid body).
			Untrace,

			//! Click: generated on a down followed by an immediate up event.
			//! parameters: position
			Click,
			//! DoubleClick: generated by a sequence of quick down/up/down/up events.
			//! parameters: position.
			DoubleClick,
			//! MoveLeft: generated when the source of event goes toward the left of the screen.
			//! parameters: position.
			MoveLeft,
			//! MoveRight: generated when the source of event goes toward the right of the screen.
			//! parameters: position.
			MoveRight,
			//! MoveUp: generated when the source of event goes toward the top of the screen.
			//! parameters: position.
			MoveUp,
			//! MoveDown: generated when the source of event goes toward the bottom of the screen.
			//! parameters: position.
			MoveDown,
			//! Zoom: zoom event.
			Zoom,
			//! SplitStart: generated at the start of a split/zoom gesture.
			//! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
			SplitStart,
			//! SplitEnd: generated at the end of a split/zoom gesture.
			//! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
			SplitEnd,
			//! Split: generated during a split/zoom gesture. 
			//! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions), value[0] (delta distance) value[1] (delta ratio) .
			Split,
			//! RotateStart: generated at the start of a rotation gesture.
			//! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
			RotateStart,
			//! RotateEnd: generated at the end of a rotation gesture.
			//! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
			RotateEnd,
			//! Rotate: generated when an event source is stationary while a second source is rotating around the first.
			//! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions), rotation[0] (degrees).
			Rotate,
		};

		//! Defines some generic input event flags
		enum Flags
		{
			//! Used for right mouse button or equivalent events.
			Left = 1 << 1,
			//! Used for right mouse button or equivalent events.
			Right = 1 << 2,
			//! Used for middle mouse button or equivalent events.
			Middle = 1 << 3,
			//! Used for ctrl key presses or equivalent events.
			Ctrl = 1 << 4,
			//! Used for ctrl key presses or equivalent events.
			Alt = 1 << 5,
			//! Used for ctrl key presses or equivalent events.
			Shift = 1 << 6,

			//! User flags should offset this value: 16 user flags available (USER to USER << 16)
			User = 1 << 16
		};

		static const int MaxPointSetSize = 32;

		//! Set to true if this even has been processed already.
		bool processed;

		//! id of the source of this event. Input services associate unique ids to each of their event sources.
		unsigned int sourceId;
	
		//! Type of the service that generated this event.
		enum Service::ServiceType serviceType;

		//! Unique id of the service that generated this event.
		int serviceId;

		//! The event type.
		enum Type type;

		//! Event flags.
		unsigned int flags;

		//! Event timestamp.
		unsigned int timestamp;

		//! Position 
		Vector3f position;

		//! Rotation.
		Quaternion orientation;

		//! Vector storing additional event parameters (i.e. split distance / ratio for Split events)
		Vector3f value;

		//! Point set
		int numberOfPoints;
		Vector3f pointSet[MaxPointSetSize];

		int userDataSize;
		void* userData;

		// NOTE: event serialization services are used only in clustered / multidisplay systems.
		// If display system building is disabled, this code can be safely excluded.
		// Right now, event serialization for NetService / oinputserver and display system
		// serialization (this) are implemented as separate facilities. We may think of joining them
		// sometime in the future.
		// ALso, this feature relies on Equalizer data streams, so it is enabled only for builds
		// that support equalizer.
	#ifdef OMEGA_USE_DISPLAY_EQUALIZER
		//! Serialize an Event instance.
		void serialize(co::DataOStream& os)
		{
			os << processed;
			os << sourceId;
			os << serviceType;
			os << type;
			os << flags;
			os << timestamp;
			os << position[0] << position[1] << position[2];
			os << orientation.x() << orientation.y() << orientation.z() << orientation.w();
			os << value[0] << value[1] << value[2];
			os << numberOfPoints;
			for(int i = 0; i < numberOfPoints; i++)
			{
				os << pointSet[i][0] << pointSet[i][1] << pointSet[i][2];
			}
		}

		//! Deserialize an Event instance.
		void deserialize( co::DataIStream& is)
		{
			is >> processed;
			is >> sourceId;
			is >> serviceType;
			is >> type;
			is >> flags;
			is >> timestamp;
			is >> position[0] >> position[1] >> position[2];
			is >> orientation.x() >> orientation.y() >> orientation.z() >> orientation.w();
			is >> value[0] >> value[1] >> value[2];
			is >> numberOfPoints;
			for(int i = 0; i < numberOfPoints; i++)
			{
				is >> pointSet[i][0] >> pointSet[i][1] >> pointSet[i][2];
			}
		}
	#endif
		bool isFlagSet(uint flag) const;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Event::isFlagSet(uint flag) const
	{ return (flags & flag) == flag; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Event::Event():
		processed(false)
	{}

}; // namespace omega

#endif