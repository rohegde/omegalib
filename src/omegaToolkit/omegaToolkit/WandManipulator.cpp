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
#include "omegaToolkit/WandManipulator.h"
#include "omega/DisplaySystem.h"

using namespace omegaToolkit;

///////////////////////////////////////////////////////////////////////////////////////////////////
void WandManipulator::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Wand && !evt.isProcessed())
	{
		// If a node is assigned to this actor and is selected, we consider mouse events consumed
		// by this actor.
		if(myNode != NULL && myNodeActive) evt.setProcessed();

		myPointerEventReceived = true;
		//myPointerEventData = 0;

		//myPointerPosition = Vector2f(evt.getPosition().x(), evt.getPosition().y());
		myPointerEventType = evt.getType();
		
		if(evt.isFlagSet(myRotateButtonFlag)) 
		{
				//omsg("BUNGA");
			myButton2Pressed = true;
		}
		else myButton2Pressed = false;
		
		SystemManager::instance()->getDisplaySystem()->getViewRayFromEvent(evt, myPointerRay);

		if(evt.getExtraDataItems() >= 2 && evt.getExtraDataType() == Event::ExtraDataFloatArray)
		{
			myXAxis = evt.getExtraDataFloat(0);
			myYAxis = evt.getExtraDataFloat(1);
		}
		
		// if axis 4 is available, use it as button 1
		if(evt.getExtraDataItems() >= 4 && evt.getExtraDataType() == Event::ExtraDataFloatArray)
		{
			if(evt.getExtraDataFloat(4) > 0.5f) 
			{	
				if(myButton1Pressed == false)
				{
					myPointerEventType = Event::Down;
				}
				myButton1Pressed = true;
			}
			else 
			{
				if(myButton1Pressed == true)
				{
					myPointerEventType = Event::Up;
				}
				myButton1Pressed = false;
			}
		}
		else
		{
			// If axis 4 is not available, handle button 1 as a normal button
			if(evt.isFlagSet(myMoveButtonFlag)) myButton1Pressed = true;
			else myButton1Pressed = true;
		}
		myWandOrientation = evt.getOrientation();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WandManipulator::update(const UpdateContext& context)
{
	// Exit immediately if we received no pointer event or if there is no node attached to this
	// interactor
	if(!myPointerEventReceived || myNode == NULL || !myNode->isVisible()) return;
	if(myPointerEventType == Event::Down)
	{
		Vector3f handlePos;
		if(myNode->hit(myPointerRay, &handlePos, SceneNode::HitBest))
		{
			myStartBSphere = myNode->getBoundingSphere();
			myStartWandOrientationInv = myWandOrientation.inverse();
			myStartOrientation = myWandOrientation.inverse() * myNode->getOrientation();
			//myStartRayDirection = myPointerRay.getDirection();
			myHandlePosition = (handlePos - myNode->getPosition()); 
			myHandleDistance = (handlePos - myPointerRay.getOrigin()).norm();
			myNodeActive = true;
			
			//omsg("HITTTTTTTTTTTTTTTTTTTTTTTTT");
			
			//ofmsg("handlepos %1% myHandlePosition %2%", %handlePos %myHandlePosition);
			//ofmsg("Ray origin %1% Direction %2% Handle Distance: %3%", %myPointerRay.getOrigin() %myPointerRay.getDirection() %myHandleDistance);
		}
	}
	else if(myPointerEventType == Event::Up)
	{
		myNodeActive = false;
	}
	else if(myPointerEventType == Event::Update)
	{
		// Manipulate object, if one is active.
		if(myNodeActive)
		{
			if(myButton1Pressed)
			{
				Quaternion newO = myStartWandOrientationInv * myWandOrientation;
				Vector3f hp  = newO * myHandlePosition;
				Vector3f newPos = myPointerRay.getPoint(myHandleDistance) - hp;
				myNode->setPosition(newPos);
				myNode->setOrientation(myWandOrientation * myStartOrientation);
			}
			else if(myButton2Pressed)
			{
				myNode->setOrientation(myWandOrientation * myStartOrientation);
			}
		}
	}
	
	myPointerEventReceived = false;
	myPointerEventType = Event::Null;
}
