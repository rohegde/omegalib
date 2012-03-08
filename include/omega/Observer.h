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
#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "osystem.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Observer: public ReferenceType
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	public:
		Observer();

		void setViewPosition(const Vector3f& value);
		Vector3f getViewPosition();

		//AffineTransform3 getSensorTransform();
		//void setSensorTransform(const AffineTransform3& value);

		AffineTransform3 getViewTransform();
		const AffineTransform3& getHeadTransform();

		void updateHead(const Vector3f& position, const Quaternion& orientation);
		void updateView(const Vector3f& position, const Quaternion& orientation);
		void load(Setting& setting);

		Vector3f getWorldHeadPosition() { return myHeadPosition - myViewPosition; }

	private:
		//! Current view transform
		AffineTransform3 myViewTransform;
		AffineTransform3 myHeadTransform;
		
		Vector3f myReferencePosition;

		//! Observer current position.
		Vector3f myHeadPosition;
		//! Observer current rotation.
		Quaternion myHeadOrientation;

		//! Position of observer reference frame wrt world origin.
		Vector3f myViewPosition;
		Quaternion myViewOrientation;

		//! Transformation from sensor reference frame to observer reference frame.
		//! Use when sensor is offset and or / rotated from actual eye position.

		// do we need this.
		//AffineTransform3 mySensorTransform;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Observer::setViewPosition(const Vector3f& value) 
	{ myViewPosition = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector3f Observer::getViewPosition() 
	{ return myViewPosition; }

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//inline AffineTransform3 Observer::getSensorTransform()
	//{ return mySensorTransform; }

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//inline void Observer::setSensorTransform(const AffineTransform3& value) 
	//{ mySensorTransform = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline AffineTransform3 Observer::getViewTransform() 
	{ return myViewTransform; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AffineTransform3& Observer::getHeadTransform() 
	{ return myHeadTransform; }
}; // namespace omega

#endif