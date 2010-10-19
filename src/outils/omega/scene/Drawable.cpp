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

#include "omega/scene/Drawable.h"

using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimplePrimitive::draw()
{
	switch(myPrimitiveType)
	{
	case SimplePrimitive::SolidCube:
		glutSolidCube(mySize);
		break;
	case SimplePrimitive::SolidSphere:
		glutSolidSphere(mySize, myResolution1, myResolution2);
		break;
	case SimplePrimitive::SolidTeapot:
		glutSolidTeapot(mySize);
		break;
	};
}
