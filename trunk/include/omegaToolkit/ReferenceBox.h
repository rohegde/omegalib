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
#ifndef __REFERENCE_BOX_H__
#define __REFERENCE_BOX_H__

#include "RenderableSceneObject.h"
#include "SceneRenderable.h"

namespace oengine {
	// Forward declarations
	class SceneNode;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API ReferenceBox: public RenderableSceneObject
	{
	friend class ReferenceBoxRenderable;
	public:
		enum Side { Left, Right, Top, Bottom, Front, Back, NumSides };

	public:
		ReferenceBox();

		void setup(Setting& s);

		virtual Renderable* createRenderable();

		Color& getColor(Side side);
		void setColor(Side side, const Color& color);

		bool isFaceEnabled(Side side);
		void setFaceEnabled(Side side, bool enabled);

		Color& getPrimaryLineColor();
		void setPrimaryLineColor(const Color& color);

		float getPrimaryLineInterval();
		void setPrimaryLineInterval(float value);

		const Vector3f& getSize();
		void setSize(const Vector3f value);

		virtual const AlignedBox3* getBoundingBox();
		virtual bool hasBoundingBox();

	private:
		bool mySideEnabled[NumSides];
		Color mySideColor[NumSides];
		Color myPrimaryLineColor;
		float myPrimaryLineInterval;
		Vector3f mySize;
		AlignedBox3 myBox;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API ReferenceBoxRenderable: public SceneRenderable
	{
	public:
		ReferenceBoxRenderable(ReferenceBox* owner);
		virtual ~ReferenceBoxRenderable();
		void draw(RenderState* state);
	
	private:
		void drawReferencePlane(const Vector3f& min, const Vector3f& max, Axis normal, const Color& color);
		void drawReferenceGrid(const Vector3f& min, const Vector3f& max, Axis normal, const Color& color, int lines);

	private:
		ReferenceBox* myOwner;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color& ReferenceBox::getColor(Side side)
	{ return mySideColor[side];	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ReferenceBox::setColor(Side side, const Color& color)
	{ mySideColor[side] = color; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool ReferenceBox::isFaceEnabled(Side side)
	{ return mySideEnabled[side]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ReferenceBox::setFaceEnabled(Side side, bool enabled)
	{ mySideEnabled[side] = enabled; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color& ReferenceBox::getPrimaryLineColor()
	{ return myPrimaryLineColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ReferenceBox::setPrimaryLineColor(const Color& value)
	{ myPrimaryLineColor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float ReferenceBox::getPrimaryLineInterval()
	{ return myPrimaryLineInterval;	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ReferenceBox::setPrimaryLineInterval(float value)
	{ myPrimaryLineInterval = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Vector3f& ReferenceBox::getSize()
	{ return mySize; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ReferenceBox::setSize(const Vector3f value)
	{
		mySize = value;
		myBox = AlignedBox3(-mySize / 2, mySize / 2);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AlignedBox3* ReferenceBox::getBoundingBox() 
	{ return &myBox; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool ReferenceBox::hasBoundingBox() 
	{ return true; }
}; // namespace oengine

#endif