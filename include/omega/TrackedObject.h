/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __TRACKED_OBJECT__
#define __TRACKED_OBJECT__

#include "omega/Actor.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API TrackedObject: public Actor
	{
	public:
		TrackedObject();
		virtual ~TrackedObject();

		virtual void handleEvent(const Event& evt);
		virtual void update(const UpdateContext& context);

		void setOffset(Vector3f value) { myOffset = value; }
		Vector3f getOffset() { return myOffset; }

		void setOrientationOffset(const Quaternion& value) { myOrientationOffset = value; }
		Quaternion getOrientationOffset() { return myOrientationOffset; }
		
		void setPositionTrackingEnabled(bool value) { myPositionTrackingEnabled = value; }
		bool isPositionTrackingEnabled() { return myPositionTrackingEnabled; }

		void setOrientationTrackingEnabled(bool value) { myOrientationTrackingEnabled = value; }
		bool isOrientationTrackingEnabled() { return myOrientationTrackingEnabled; }

		void setTrackableServiceType(Event::ServiceType value) { myTrackableServiceType = value; }
		Event::ServiceType getTrackableServiceType() { return myTrackableServiceType; }

		void setTrackableSourceId(int value) { myTrackableSourceId = value; }
		int getTrackableSourceId() { return myTrackableSourceId; }

	private:
		Vector3f myTrackedPosition;
		Quaternion myTrackedOrientation;

		bool myPositionTrackingEnabled;
		bool myOrientationTrackingEnabled;
		Vector3f myOffset;
		Quaternion myOrientationOffset;

		Event::ServiceType myTrackableServiceType;
		int myTrackableSourceId;
	};
}; // namespace omegaToolkit

#endif