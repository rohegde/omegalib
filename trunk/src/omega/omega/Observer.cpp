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
#include "omega/StringUtils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Observer::Observer()
{
	myViewPosition = Vector3f::Zero();
	myViewTransform = AffineTransform3::Identity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::load(Setting& setting)
{
	if(setting.exists("referencePosition"))
	{
		Setting& st = setting["referencePosition"];
		myViewPosition.x() = (float)st[0];
		myViewPosition.y() = (float)st[1];
		myViewPosition.z() = (float)st[2];
	}

	Vector3f position = Vector3f::Zero();
	if(setting.exists("position"))
	{
		Setting& st = setting["position"];
		position.x() = (float)st[0];
		position.y() = (float)st[1];
		position.z() = (float)st[2];
	}

	// Set observer initial position, neutral orientation.
	updateHead(position, Quaternion::Identity());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::updateHead(const Vector3f& position, const Quaternion& orientation)
{
	myHeadPosition = position;
	myHeadOrientation = orientation;
	//myHeadTransform = Math::makeViewMatrix(position, orientation);
	myHeadTransform = AffineTransform3::Identity();

	myHeadTransform.translate(position);
	myHeadTransform.rotate(orientation);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::updateView(const Vector3f& position, const Quaternion& orientation, float scale)
{
	myViewPosition = position;
	myViewOrientation = orientation;
	//myViewTransform = Math::makeViewMatrix(position, orientation);
	myViewTransform = AffineTransform3::Identity();
	myViewTransform.translate(myHeadPosition);
	myViewTransform.rotate(orientation);
	myViewTransform.translate(-myHeadPosition);
	myViewTransform.translate(position);
	myViewTransform.scale(scale);
}
