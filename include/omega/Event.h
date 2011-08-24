/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 * Contributors:
 *  Victor Mateevitsi		mvictoras@gmail.com
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
#ifndef __EVENT_H__
#define __EVENT_H__

#include "osystem.h"
#include "Service.h"

// Used for timestamp
#include <sys/timeb.h>

#define FLOAT_PTR(x) *((float*)&x)
#define INT_PTR(x) *((int*)&x)


namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Events are generated by Service instances. 
	class Event: public DynamicObject
	{
	// Friend the class that implements methods used for event serialization by equalizer.
	friend class EqUtils; 
	public:
		static const int ExtraDataSize = 1024;
		static const int MaxExtraDataItems = 32;

		//! Supported event types.
		enum Type 
		{ 
			//! Select: generated when the source of the event gets selected or activated.
			//! Used primarily for use iterface controls.
			Select,
			//! Toggle: generated when some boolean state in the event source changes. Can represent
			//! state changes in physical switches and buttons, or in user interface controls like
			//! check boxes and radio buttons.
			Toggle,
			//!ChangeValue: generated when the source of an event changes it's internal value or state.
			//! Different from Update because ChangeValue is not usually fired at regular intervals,
			//! while Update events are normally sent at a constant rate.
			ChangeValue,
			//! Update: Generated when the soruce of an event gets updated (what 'update') means depends
			//! on the event source.
			Update,
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
			//! Null: generic null value for event type.
			Null
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

		enum ExtraDataType
		{
			ExtraDataNull,
			ExtraDataFloatArray,
			ExtraDataIntArray,
			ExtraDataVector3Array,
			ExtraDataString
		};

	public:
		Event();

		void copyFrom(const Event& e);

		void reset(Type type, Service::ServiceType serviceType, unsigned int sourceId = 0, int serviceId = 0);

		//! id of the source of this event. Input services associate unique ids to each of their event sources.
		unsigned int getSourceId() const;

		//! Type of the service that generated this event.
		Service::ServiceType getServiceType() const;

		//! Unique id of the service and / or hardware device that generated this event.
		int getServiceId() const;

		//! The event type.
		Type getType() const;

		//! Gets the event timestamp. The timestamp is updated everytime the Event::reset is called.
		unsigned int getTimestamp() const;

		//! Set to true if this event has been processed already.
		void setProcessed() const;
		bool isProcessed() const;

		//! Get the event position 
		const Vector3f& getPosition() const;
		float getPosition(int component) const;
		void setPosition(const Vector3f& value);
		void setPosition(float x, float y, float z);
		void setPosition(float x, float y);

		//! Rotation.
		const Quaternion& getOrientation() const;
		void setOrientation(const Quaternion& value);
		void setOrientation(float w, float x, float y, float z);

		
		//! Event flags.
		bool isFlagSet(uint flag) const;
		void setFlags(uint flags);
		void clearFlags();

		//! Point set
		void setExtraDataType(ExtraDataType type);
		ExtraDataType getExtraDataType() const;
		float getExtraDataFloat(int index) const;
		void setExtraDataFloat(int index, float value);
		int getExtraDataInt(int index) const;
		void setExtraDataInt(int index, int value);
		Vector3f getExtraDataVector3(int index) const;
		void setExtraDataVector3(int index, const Vector3f value);
		const char* getExtraDataString() const;
		void setExtraDataString(const String& value);
		void resetExtraData();
		bool isExtraDataNull(int pointId) const;

		//! Returns the size in bytes of the event extra data.
		int getExtraDataSize() const;

		//! Returns the number of elements stored in the extra data section of the Event.
		int getExtraDataLength() const;
	private:
		unsigned int mySourceId;
		enum Service::ServiceType myServiceType;
		int myServiceId;
		enum Type myType;

		Vector3f myPosition;
		Quaternion myOrientation;
		mutable bool myProcessed;
		int myTimestamp;

		unsigned int myFlags;

		ExtraDataType myExtraDataType;
		int myExtraDataLength;
		int myExtraDataValidMask;
		byte myExtraData[ExtraDataSize];
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Event::Event():
		myProcessed(false),
		myFlags(0),
		myExtraDataType(ExtraDataNull),
		myExtraDataValidMask(0)
	{}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::reset(Type type, Service::ServiceType serviceType, unsigned int sourceId, int serviceId)
	{
		myType = type;
		mySourceId = sourceId;
		myServiceType = serviceType;
		myServiceId = serviceId;
		myFlags = 0;
		myExtraDataLength = 0;
		myExtraDataValidMask = 0;
		myExtraDataType = ExtraDataNull;

		timeb tb;
		ftime( &tb );
		int curTime = tb.millitm + (tb.time & 0xfffff) * 1000; // Millisecond timer
		myTimestamp = curTime;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::copyFrom(const Event& e)
	{ memcpy(this, &e, sizeof(Event)); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline unsigned int Event::getTimestamp() const
	{ return myTimestamp; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline unsigned int Event::getSourceId() const
	{ return mySourceId; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Service::ServiceType Event::getServiceType() const
	{ return myServiceType;	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Event::getServiceId() const
	{ return myServiceId; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Event::Type Event::getType() const
	{ return myType; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setProcessed() const
	{ myProcessed = true; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Event::isProcessed() const
	{ return myProcessed; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Vector3f& Event::getPosition() const
	{ return myPosition; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Event::getPosition(int component) const
	{ return myPosition[component]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setPosition(const Vector3f& v)
	{
		myPosition[0] = v[0];
		myPosition[1] = v[1];
		myPosition[2] = v[2];
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setPosition(float x, float y, float z)
	{
		myPosition[0] = x;
		myPosition[1] = y;
		myPosition[2] = z;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setPosition(float x, float y)
	{
		myPosition[0] = x;
		myPosition[1] = y;
		myPosition[2] = 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Quaternion& Event::getOrientation() const
	{ return myOrientation; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setOrientation(const Quaternion& value)
	{ myOrientation = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setOrientation(float w, float x, float y, float z)
	{ myOrientation = Quaternion(w, x, y, z); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Event::isFlagSet(uint flag) const
	{ return (myFlags & flag) == flag; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setFlags(uint flags)
	{ myFlags |= flags; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::clearFlags()
	{ myFlags = 0; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setExtraDataType(Event::ExtraDataType type)
	{ myExtraDataType = type; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Event::ExtraDataType Event::getExtraDataType() const
	{ return myExtraDataType; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Event::getExtraDataFloat(int index) const
	{
		oassert(myExtraDataType == ExtraDataFloatArray);
		oassert(!isExtraDataNull(index));
		return FLOAT_PTR(myExtraData[index * 4]);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setExtraDataFloat(int index, float value)
	{
		oassert(myExtraDataType == ExtraDataFloatArray);
		oassert(index < MaxExtraDataItems);
		if(index >= myExtraDataLength) myExtraDataLength = index + 1;
		// Mark this entry bit as valid in the extra data validity mask
		myExtraDataValidMask |= (1 << index);
		FLOAT_PTR(myExtraData[index * 4]) = value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Event::getExtraDataInt(int index) const
	{
		oassert(myExtraDataType == ExtraDataIntArray);
		oassert(!isExtraDataNull(index));
		return INT_PTR(myExtraData[index * 4]);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setExtraDataInt(int index, int value)
	{
		oassert(myExtraDataType == ExtraDataIntArray);
		oassert(index < MaxExtraDataItems);
		if(index >= myExtraDataLength) myExtraDataLength = index + 1;
		// Mark this entry bit as valid in the extra data validity mask
		myExtraDataValidMask |= (1 << index);
		INT_PTR(myExtraData[index * 4]) = value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector3f Event::getExtraDataVector3(int index) const
	{
		oassert(myExtraDataType == ExtraDataVector3Array);
		oassert(!isExtraDataNull(index));
		int offset = index * 3 * 4;
		float x = FLOAT_PTR(myExtraData[offset]);
		float y = FLOAT_PTR(myExtraData[offset + 4]);
		float z = FLOAT_PTR(myExtraData[offset + 8]);
		return Vector3f(x, y, z);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setExtraDataVector3(int index, const Vector3f value)
	{
		oassert(myExtraDataType == ExtraDataVector3Array);
		oassert(index < MaxExtraDataItems);
		if(index >= myExtraDataLength) myExtraDataLength = index + 1;
		// Mark this entry bit as valid in the extra data validity mask
		myExtraDataValidMask |= (1 << index);
		int offset = index * 3 * 4;
		FLOAT_PTR(myExtraData[offset]) = value[0];
		FLOAT_PTR(myExtraData[offset + 4]) = value[1];
		FLOAT_PTR(myExtraData[offset + 8]) = value[2];
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const char* Event::getExtraDataString() const
	{
		oassert(myExtraDataType == ExtraDataString);
		return (const char*)myExtraData;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::setExtraDataString(const String& value)
	{
		oassert(myExtraDataType == ExtraDataString);
		strcpy((char*)myExtraData, value.c_str());
		myExtraDataLength = value.size();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Event::resetExtraData()
	{ 
		myExtraDataValidMask = 0; 
		myExtraDataType = ExtraDataNull;
		myExtraDataLength = 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Event::isExtraDataNull(int index) const
	{ return !((myExtraDataValidMask & (1 << index)) == (1 << index)); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Event::getExtraDataLength() const
	{ return myExtraDataLength; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Event::getExtraDataSize() const
	{
		switch(myExtraDataType)
		{
		case ExtraDataNull:
			return 0;
		case ExtraDataFloatArray:
		case ExtraDataIntArray:
			return myExtraDataLength * 4;
			break;
		case ExtraDataVector3Array:
			return myExtraDataLength * 4 * 3;
		case ExtraDataString:
			return myExtraDataLength;
		default:
			oerror("Event::getExtraDataSize: unknown extra data type");
		}

		// Default: return data length
		return myExtraDataLength;
	}
}; // namespace omega

#undef FLOAT_PTR
#undef INT_PTR

#endif
