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

#include "oenginebase.h"
#include "oengine/Effect.h"
#include "oengine/RenderPass.h"
#include "oengine/Renderable.h"

namespace oengine {
	// Forward declarations
	class SceneNode;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API BoundingSphere: public Renderable
	{
	public:
		BoundingSphere(): 
		  myDrawOnSelected(false), 
 		  myVisible(true),
		  mySlices(5),
		  mySegments(32),
		  myColor(0.8f, 0.8f, 1.0f, 1.0f) {}

		virtual void render(SceneNode* node, RenderState* state);

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
	inline void BoundingSphere::setDrawOnSelected(bool value)
	{
		myDrawOnSelected = value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool BoundingSphere::getDrawOnSelected()
	{
		return myDrawOnSelected;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphere::setVisible(bool value)
	{
		myVisible = value;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool BoundingSphere::getVisible()
	{
		return myVisible;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color BoundingSphere::getColor()
	{ return myColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphere::setColor(const Color& value)
	{ myColor = value;}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int BoundingSphere::getSegments()
	{ return mySegments; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphere::setSegments(int value)
	{ mySegments = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int BoundingSphere::getSlices()
	{ return mySlices;}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void BoundingSphere::setSlices(int value)
	{ mySlices = value; }
}; // namespace oengine

#endif