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
		if(myNode != NULL && myNodeSelected) evt.setProcessed();

		myButton1Pressed = false;
		myButton2Pressed = false;
		myPointerEventReceived = true;
		myPointerEventData = 0;

		myPointerPosition = Vector2f(evt.getPosition(0), evt.getPosition(1));
		// We just care about Up / Down events.
		if(evt.getType() != Event::Move)
		{
			myPointerEventType = evt.getType();
		}
		if(evt.isFlagSet(myMoveButtonFlag)) myButton1Pressed = true;
		if(evt.isFlagSet(myRotateButtonFlag)) myButton2Pressed = true;
		
		SystemManager::instance()->getDisplaySystem()->getViewRayFromEvent(evt, myPointerRay);

		if(evt.getExtraDataItems() >= 4 && evt.getExtraDataType() == Event::ExtraDataFloatArray)
		{
			myXAxis = evt.getExtraDataFloat(0);
			myYAxis = evt.getExtraDataFloat(1);
			
			// Simulate Button 1 presses using Z Axis.
			float pressThresh = 0.5;
			if(evt.getExtraDataFloat(4) > pressThresh)
			{			
				myButton1Pressed = true;
				if(myZAxis <= pressThresh) myPointerEventType = Event::Down;
			}
			else
			{
				myButton1Pressed = false;
				if(myZAxis > pressThresh) myPointerEventType = Event::Up;
			}
			myZAxis = evt.getExtraDataFloat(4);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WandManipulator::update(const UpdateContext& context)
{
	// Exit immediately if we received no pointer event or if there is no node attached to this
	// interactor
	if(!myPointerEventReceived || myNode == NULL) return;

	if(myPointerEventType == Event::Down && (myButton1Pressed || myButton2Pressed))
	{
		Vector3f handlePos;
		if(myNode->hit(myPointerRay, &handlePos, SceneNode::HitBoundingSphere))
		{
			myStartBSphere = myNode->getBoundingSphere();
			myHandlePosition = handlePos; //myStartBSphere.getCenter() - handlePos; 
			myHandleDistance = (myPointerRay.getOrigin() - myStartBSphere.getCenter()).norm();
			myNodeSelected = true;
			ofmsg("Handle distance: %1% Handle position: %2%", %myHandleDistance %myHandlePosition);
		}
	}

	// Manipulate object, if one is active.
	if(myNode->isSelected())
	{
		if(myButton1Pressed && myNodeSelected)
		{
			//myHandleDistance += myYAxis * context.dt;
			Vector3f newPos = myPointerRay.getPoint(myHandleDistance); 
			myNode->setPosition(newPos);
		}
		else
		{
			myNodeSelected = false;
		}
		
		if(myButton2Pressed)
		{
			float yaw = myXAxis * context.dt;
			float pitch = -myYAxis * context.dt;
			Quaternion quat = AngleAxis(pitch, Vector3f::UnitX()) * AngleAxis(yaw, Vector3f::UnitY());
			myNode->rotate(quat, Node::TransformWorld);
			//Vector3f newPos = myPointerRay.getPoint(myHandleDistance); 
			//myNode->setPosition(newPos);
		}
	}
	
	myPointerEventReceived = false;
	myPointerEventType = Event::Null;
}
