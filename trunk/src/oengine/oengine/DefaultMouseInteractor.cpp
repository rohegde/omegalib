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
#include "oengine/DefaultMouseInteractor.h"
#include "omega/StringUtils.h"

using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultMouseInteractor::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Pointer)
	{
		myPointerButton1Pressed = false;
		myPointerButton2Pressed = false;
		myPointerEventReceived = true;
		myPointerEventData = 0;
		myPointerPosition = Vector2f(evt.getPosition(0), evt.getPosition(1));
		// We just care about Up / Down events.
		if(evt.getType() != Event::Move)
		{
			myPointerEventType = evt.getType();
		}
		if(evt.isFlagSet(myMoveButtonFlag)) myPointerButton1Pressed = true;
		if(evt.isFlagSet(myRotateButtonFlag)) myPointerButton2Pressed = true;
		evt.setProcessed();

		if(evt.getExtraDataLength() == 2)
		{
			myPointerRay.setOrigin(evt.getExtraDataVector3(0));
			myPointerRay.setDirection(evt.getExtraDataVector3(1));
		}
		if(evt.getType() == Event::Zoom) myPointerEventData = evt.getExtraDataInt(0);

		updateNode();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultMouseInteractor::updateNode()
{
	// Exit immediately if we received no pointer event or if there is no node attached to this
	// interactor
	if(!myPointerEventReceived || myNode == NULL) return;

	if(myPointerEventType == Event::Down)
	{
		Vector3f handlePos;
		if(myNode->hit(myPointerRay, &handlePos, SceneNode::HitBoundingSphere))
		{
			myStartBSphere = myNode->getBoundingSphere();
			myStartOrientation = myNode->getOrientation();
			myStartScale = myNode->getScale()[0];
			myHandlePosition = handlePos; 
			myHandleDistance = (myHandlePosition - myPointerRay.getOrigin()).norm();
			ofmsg("Handle Distance: %1%", %myHandleDistance);
			myNode->setSelected(true);
		}
	}
	else if(myPointerEventType == Event::Up)
	{
		myNode->setSelected(false);
	}
	
	//else if(myPointerEventType == Event::Move)
	{
		// Manipulate object, if one is active.
		if(myNode->isSelected())
		{
			if(myPointerButton1Pressed)
			{
				Vector3f newPos = myPointerRay.getPoint(myHandleDistance) - (myHandlePosition - myStartBSphere.getCenter());
				myNode->setPosition(newPos);
			}
			else if(myPointerButton2Pressed)
			{
				// Intersect the ray with the bounding sphere. 
				// If the point is outside the bounding sphere, perform no rotation.
				std::pair<bool, float> p = myPointerRay.intersects(myStartBSphere);
				if(p.first)
				{
					Vector3f pt = myPointerRay.getPoint(p.second);
					pt -= myStartBSphere.getCenter();
					Quaternion rot = Math::buildRotation(myHandlePosition, pt , Vector3f::Zero() );
					myNode->setOrientation(rot * myStartOrientation);
				}
			}
		}
	}
	
	if(myPointerEventType == Event::Zoom)
	{
		// Manipulate object, if one is active.
		float sc;
		if(myPointerEventData < 0) sc = 0.9f;
		else sc = 1.1f;
		myNode->scale(sc, sc, sc);
	}

	myPointerEventReceived = false;
	myPointerEventType = Event::Null;
}
