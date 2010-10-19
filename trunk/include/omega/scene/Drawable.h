/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * DrawContext
 *********************************************************************************************************************/
#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "omega/osystem.h"

namespace omega
{
namespace scene
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class Drawable
	{
	public:
		virtual void draw() = 0;

	private:
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Implements some simple primitive drawing through GLUT calls.
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class SimplePrimitive: public Drawable
	{
	public:
		enum PrimitiveType { SolidCube, SolidSphere, SolidTeapot};

	public:
		SimplePrimitive(): myPrimitiveType(SolidCube), mySize(1), myResolution1(16), myResolution2(16) {}

		OUTILS_API virtual void draw();

		PrimitiveType getPrimitiveType() { return myPrimitiveType; }
		void setPrimitiveType(PrimitiveType value) { myPrimitiveType = value; }

		float getSize() { return mySize; }
		void setSize( float value) { mySize = value; }

		int getResolution1() { return myResolution1; }
		void setResolution1( float value) { myResolution1 = value; }

		int getResolution2() { return myResolution2; }
		void setResolution2( float value) { myResolution2 = value; }

	private:
		float mySize;
		int myResolution1;
		int myResolution2;
		PrimitiveType myPrimitiveType;
	};

}; // namespace scene
}; // namespace omega

#endif