/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/SystemManager.h"

#include "omega/DisplaySystem.h"
#include "omega/Observer.h"
#include "omega/ObserverUpdateService.h"

using namespace omega;

// Define to enable a simple version of the service with no update threshold and separate orientation support.
#define OMEGA_SIMPLE_OBSERVER_UPDATE
//#define OMEGA_USE_THRESHOLD

///////////////////////////////////////////////////////////////////////////////////////////////////
ObserverUpdateService::ObserverUpdateService():
	myObserver(NULL),
	mySourceId(0),
	myEnableLookAt(false),
	myDebug(false),
	myEnableOrientationSource(false),
	myUseHeadPointId(false),
	myDynamicSourceTokenId(0),
	myDynamicSourceTokenAttachPoint(AttachHead),
	myDynamicSourceActivationDistance(0.1f)
{
	myLookAt = Vector3f::Zero();
	setPollPriority(Service::PollLast);
	
	myCurrentMovementThreshold = 0;
	myMovementThresholdTarget = 0.02f;
	myMovementThresholdCoeff = 4;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::setup(Setting& settings)
{
	if(settings.exists("sourceId"))
	{
		mySourceId = settings["sourceId"];
	}
	else if(settings.exists("dynamicSource"))
	{
		Setting& st = settings["dynamicSource"];
		myUseDynamicSource = true;
		if(st.exists("tokenAttachPoint"))
		{
			String dstap = st["tokenAttachPoint"];
			if(dstap == "head") myDynamicSourceTokenAttachPoint = AttachHead;
			else if(dstap == "leftHand") myDynamicSourceTokenAttachPoint = AttachRightHand;
			else if(dstap == "rightHand") myDynamicSourceTokenAttachPoint = AttachLeftHand;
		}
		myDynamicSourceTokenId = st["tokenId"];
		if(st.exists("activationDistance"))
		{
			myDynamicSourceActivationDistance = st["activationDistance"];
		}
	}

	if(settings.exists("debug"))
	{
		myDebug = settings["debug"];
	}
	if(settings.exists("useHeadPointId"))
	{
		myUseHeadPointId = settings["useHeadPointId"];
	}
	if(settings.exists("lookAt"))
	{
		myEnableLookAt = true;
		Setting& stLa = settings["lookAt"];
		myLookAt(0) = stLa[0];
		myLookAt(1) = stLa[1];
		myLookAt(2) = stLa[2];
	}
	else if(settings.exists("orientationSource"))
	{
		Setting& st = settings["orientationSource"];
		myOrientationSourceId = st["sourceId"];
		myEnableOrientationSource = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::initialize()
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	myObserver = ds->getObserver(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::poll()
{
	lockEvents();
	int numEvts = getManager()->getAvailableEvents();
	myObserver->updateHead(myLastPosition, Quaternion::Identity());
	for(int i = 0; i < numEvts; i++)
	{
		Event* evt = getEvent(i);
		if(evt->getServiceType() == Service::Mocap)
		{
			// Dynamic source support.
			if(myUseDynamicSource)
			{
				updateDynamicSource(evt);
			}

			if(evt->getSourceId() == mySourceId)
			{
				Vector3f pos = evt->getPosition();
				myLastPosition = pos;
#ifdef OMEGA_SIMPLE_OBSERVER_UPDATE
				myObserver->updateHead(pos, evt->getOrientation());
				if(myDebug)
				{
					ofmsg("Observer pos: %1%", %pos);
				}

#else
				if(!myUseHeadPointId || !evt->isExtraDataNull(Head))
				{
					if(myUseHeadPointId) pos = evt->getExtraDataVector3(Head);
					if(myDebug)
					{
						ofmsg("Observer pos: %1%", %pos);
					}
					Quaternion q = Quaternion::Identity();

					// If look at is enabled, compute orientation using target lookat point.
					if(myEnableLookAt)
					{
						Vector3f dir = myLookAt - pos;
						dir.normalize();
						Vector3f localZ = Vector3f(0, 0, -1);
						q = Math::buildRotation(localZ, dir, Vector3f::Zero());
					}
					else if(myEnableOrientationSource)
					{
						q = myLastOrientation;
					}

#ifdef OMEGA_USE_THRESHOLD
					float d = (myLastPosition - pos).norm();
					if(d > myCurrentMovementThreshold)
					{
						//if(myCurrentMovementThreshold > (myMovementThresholdTarget - 0.01f)) myCurrentMovementThreshold = 0;
						myCurrentMovementThreshold = 0.01f;

						myLastPosition = pos;
					}
					myCurrentMovementThreshold = (myCurrentMovementThreshold * myMovementThresholdCoeff + myMovementThresholdTarget) / (myMovementThresholdCoeff + 1);
					//ofmsg("mvth: %1%", %myCurrentMovementThreshold);
					myObserver->updateHead(myLastPosition, q);
#else
					q = Quaternion::Identity();
					//ofmsg("%1% %2% %3% %4%", %q.x() %q.y() %q.z() %q.w());
					myObserver->updateHead(pos, q);
#endif
				}
#endif
			}
			else if(myEnableOrientationSource && evt->getSourceId() == myOrientationSourceId)
			{
				myLastOrientation = evt->getOrientation();
				//myLastPosition = (myLastPosition + evt->position) / 2;
			}
		}
	}
	unlockEvents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObserverUpdateService::updateDynamicSource(Event* evt)
{
	// Update activator token position
	if(evt->getSourceId() == myDynamicSourceTokenId)
	{
		myLastTokenPosition = evt->getPosition();
	}
	else
	{
		Vector3f attachPt;
		bool validPt = false;
		switch(myDynamicSourceTokenAttachPoint)
		{
		case AttachHead:
			if(!evt->isExtraDataNull(Head))
			{
				validPt = true;
				attachPt = evt->getExtraDataVector3(Head);
			}
			break;
		case AttachLeftHand:
			if(!evt->isExtraDataNull(LeftHand))
			{
				validPt = true;
				attachPt = evt->getExtraDataVector3(LeftHand);
			}
			break;
		case AttachRightHand:
			if(!evt->isExtraDataNull(RightHand))
			{
				validPt = true;
				attachPt = evt->getExtraDataVector3(RightHand);
			}
			break;
		}
		if(validPt)
		{
			float dist = (attachPt - myLastTokenPosition).norm();
			if(myDebug)
			{
				ofmsg("Token distance: %1%", %dist);
			}
			if(dist < myDynamicSourceActivationDistance)
			{
				mySourceId = evt->getSourceId();
			}
		}
	}
}


