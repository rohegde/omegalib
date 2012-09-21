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
#include "omega/Camera.h"
#include "omega/WandCameraController.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
WandCameraController::WandCameraController():
	myRotateSpeed(0.01f),
	myYaw(0),
	myPitch(0),
	myLastPointerPosition(0, 0, 0),
	mySpeed(0, 0, 0),
	myTorque(Quaternion::Identity())
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraController::reset()
{
	if( getCamera() != NULL )
	{
		myOriginalOrientation = getCamera()->getOrientation();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void WandCameraController::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Wand)
	{
		//float x = evt.getExtraDataFloat(0);
		//float y = evt.getExtraDataFloat(1);
		
		// Thresholds
		//if(x < 0.1f && x > -0.1f) x = 0;
		//if(y < 0.1f && y > -0.1f) y = 0;
		
		//myYaw = -x * myRotateSpeed;

		//myMoveVector = evt.getOrientation() * Vector3f(0, 0, y);
		
		// Button6 = Left Analog pressed.
		if(evt.isFlagSet(Event::Button6)) 
		{
			if(myFreeFly == false)
			{
				myLastPointerPosition = evt.getPosition();
				myLastPointerOrientation = evt.getOrientation();
			}
			myFreeFly = true;
		}
		else
		{
			myFreeFly = false;
		}
		
		
		if(myFreeFly)
		{
			Vector3f dv = (evt.getPosition() - myLastPointerPosition) * 20;
			//float speedMul = 1 + dv.norm();
			//ofmsg("Speedmul %1%", %speedMul);
			mySpeed = dv;
			myTorque = myLastPointerOrientation.inverse() * evt.getOrientation();
			myTorque = myTorque * AngleAxis(Math::Pi / 2, Vector3f::UnitY());
			myTorque = myTorque.slerp(1.0f - myRotateSpeed, Quaternion::Identity());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void WandCameraController::update(const UpdateContext& context)
{
	//updateCamera(myMoveVector, myYaw, myPitch, 0, context.dt);
	//Quaternion o = getCamera()->getOrientation();
	
	getCamera()->translate(mySpeed * context.dt);
	getCamera()->rotate(myTorque.slerp(context.dt, Quaternion::Identity()), Node::TransformLocal);
	
	
	mySpeed = mySpeed * 0.5f;
	myTorque = myTorque.slerp(0.5f, Quaternion::Identity());
}

