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
#ifndef __BOUNDING_SPHERE_DRAWABLE_H__
#define __BOUNDING_SPHERE_DRAWABLE_H__

#include "omega/osystem.h"
#include "omega/scene/Effect.h"

namespace omega
{
namespace scene
{
	// Forward declarations
	class SceneNode;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether 
	//! in future versions.
	class OUTILS_API BoundingSphereDrawable: public Drawable
	{
	public:
		BoundingSphereDrawable(): 
		  myDrawOnSelected(false), 
 		  myVisible(true),
		  mySlices(5),
		  mySegments(32),
		  myColor(0.8f, 0.8f, 1.0f, 1.0f) {}

		virtual void draw(SceneNode* node);

		void setDrawOnSelected(bool value);
		bool getDrawOnSelected();

		void setVisible(bool value);
		bool getVisible();

		Color getColor();
		void setColor(const Color& value);

		int getSegments();
		void setSegments(int value);

		int getSlices();
		void setSlices(int value);

	private:
		bool myVisible;
		bool myDrawOnSelected;
		Color myColor;
		int mySegments;
		int mySlices;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphereDrawable::setDrawOnSelected(bool value)
	{
		myDrawOnSelected = value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool BoundingSphereDrawable::getDrawOnSelected()
	{
		return myDrawOnSelected;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphereDrawable::setVisible(bool value)
	{
		myVisible = value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool BoundingSphereDrawable::getVisible()
	{
		return myVisible;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color BoundingSphereDrawable::getColor()
	{ return myColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphereDrawable::setColor(const Color& value)
	{ myColor = value;}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int BoundingSphereDrawable::getSegments()
	{ return mySegments; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphereDrawable::setSegments(int value)
	{ mySegments = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int BoundingSphereDrawable::getSlices()
	{ return mySlices;}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphereDrawable::setSlices(int value)
	{ mySlices = value; }
}; // namespace scene
}; // namespace omega

#endif