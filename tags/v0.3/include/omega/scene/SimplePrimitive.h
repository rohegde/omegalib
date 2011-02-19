/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __SIMPLE_PRIMITIVE_H__
#define __SIMPLE_PRIMITIVE_H__

#include "omega/osystem.h"
#include "omega/scene/Drawable.h"
#include "omega/scene/Effect.h"

namespace omega
{
namespace scene
{
	// Forward declarations
	class SceneNode;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements some simple primitive drawing through GLUT calls.
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class OUTILS_API SimplePrimitive: public Drawable
	{
	public:
		enum PrimitiveType { SolidCube, SolidSphere, SolidTeapot };

	public:
		SimplePrimitive(): myPrimitiveType(SolidCube), mySize(1), myResolution1(16), myResolution2(16), myEffect(NULL) {}

		virtual void draw(SceneNode* node);

		PrimitiveType getPrimitiveType() { return myPrimitiveType; }
		void setPrimitiveType(PrimitiveType value) { myPrimitiveType = value; }

		float getSize() { return mySize; }
		void setSize( float value) { mySize = value; }

		int getResolution1() { return myResolution1; }
		void setResolution1( float value) { myResolution1 = value; }

		int getResolution2() { return myResolution2; }
		void setResolution2( float value) { myResolution2 = value; }

		Effect* getEffect() { return myEffect; }
		void setEffect(Effect* value) { myEffect = value; }

	private:
		float mySize;
		int myResolution1;
		int myResolution2;
		PrimitiveType myPrimitiveType;
		Effect* myEffect;
	};
}; // namespace scene
}; // namespace omega

#endif