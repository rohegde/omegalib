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
#include "omega/Observer.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Observer::Observer()
{
	myReferencePosition = Vector3f::Zero();
	mySensorTransform = AffineTransform3::Identity();
	myViewTransform = AffineTransform3::Identity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::load(Setting& setting)
{
	if(setting.exists("referencePosition"))
	{
		Setting& st = setting["referencePosition"];
		myReferencePosition.x() = (float)st[0];
		myReferencePosition.y() = (float)st[1];
		myReferencePosition.z() = (float)st[2];
	}

	Vector3f position = Vector3f::Zero();
	if(setting.exists("position"))
	{
		Setting& st = setting["position"];
		position.x() = (float)st[0];
		position.y() = (float)st[1];
		position.z() = (float)st[2];
	}

	// Set observer initial position to origin, neutral orientation.
	update(position, Quaternion::Identity());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::update(const Vector3f& position, const Quaternion& orientation)
{
	myPosition = position;
	myOrientation = orientation;
	myViewTransform = Math::makeViewMatrix(position + myReferencePosition, orientation);

	myViewTransform = myViewTransform * mySensorTransform;
	myHeadTransform = AffineTransform3::Identity();
	float angle;
	Vector3f axis;

	AngleAxis aa;
	aa = orientation;
	angle = aa.angle();
	axis = aa.axis();

	myHeadTransform.rotate(AngleAxis(angle, axis));
	myHeadTransform.translate(position);
	//myHeadTransform.rotate(angle, axis);
	AffineTransform3 w2e = AffineTransform3::Identity();
	w2e.translate(myReferencePosition);
	myHeadTransform = w2e * myHeadTransform;
}

