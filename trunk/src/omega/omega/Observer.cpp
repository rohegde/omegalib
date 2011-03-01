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
	myReferencePosition = Vector3f::ZERO;
	mySensorTransform = Matrix4f::IDENTITY;
	myViewTransform = Matrix4f::IDENTITY;
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

	Vector3f position = Vector3f::ZERO;
	if(setting.exists("position"))
	{
		Setting& st = setting["position"];
		position.x() = (float)st[0];
		position.y() = (float)st[1];
		position.z() = (float)st[2];
	}

	// Set observer initial position to origin, neutral orientation.
	update(position, Quaternion::IDENTITY);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::update(const Vector3f& position, const Quaternion& orientation)
{
	myPosition = position;
	myOrientation = orientation;
	myViewTransform = Math::makeViewMatrix(position + myReferencePosition, orientation);

	myViewTransform *= mySensorTransform;

	myHeadTransform = Matrix4f::IDENTITY;
	float angle;
	Vector3f axis;
	orientation.toAngleAxis(&angle, &axis);
	myHeadTransform.rotate(angle, axis);
	myHeadTransform.set_translation(position);
	//myHeadTransform.rotate(angle, axis);
	Matrix4f w2e = Matrix4f::IDENTITY;
	w2e.set_translation(myReferencePosition);
	myHeadTransform = w2e * myHeadTransform;
}

