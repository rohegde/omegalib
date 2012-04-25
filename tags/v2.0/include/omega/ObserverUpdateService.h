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
#ifndef __OBSERVER_UPDATE_SERVICE_H__
#define __OBSERVER_UPDATE_SERVICE_H__

#include "omega/osystem.h"
//#include "omega/SagePointerService.h"

namespace omega
{
	class Observer;
	///////////////////////////////////////////////////////////////////////////////////////////////
	class ObserverUpdateService: public Service
	{
	public:
		enum DynamicSourceTokenAttachPoint { AttachHead, AttachLeftHand, AttachRightHand };
		// Allocator function
		static ObserverUpdateService* New() { return new ObserverUpdateService(); }

	public:
		ObserverUpdateService();

		virtual void setup(Setting& settings);
		virtual void initialize();
		virtual void poll();
		virtual void dispose();
		int getSourceId(); 
		void setSourceId(int value); 

	private:
		void updateDynamicSource(Event* evt);

	private:
		Observer* myObserver;
		int mySourceId;
		int myOrientationSourceId;
		bool myEnableOrientationSource;
		Quaternion myLastOrientation;
		Vector3f myLastPosition;
		Vector3f myLookAt;
		bool myEnableLookAt;
		bool myDebug;
		bool myUseHeadPointId;

		// Dynamic source stuff.
		bool myUseDynamicSource;
		int myDynamicSourceTokenId;
		DynamicSourceTokenAttachPoint myDynamicSourceTokenAttachPoint;
		float myDynamicSourceActivationDistance;
		Vector3f myLastTokenPosition;

		// Movement smoothing
		float myCurrentMovementThreshold;
		float myMovementThresholdTarget;
		float myMovementThresholdCoeff;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int ObserverUpdateService::getSourceId()
	{
		return mySourceId;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ObserverUpdateService::setSourceId(int value)
	{
		mySourceId = value;
	}
}; // namespace omega

#endif