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
#include "omega/GfxUtils.h"

using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimplePrimitive::draw()
{
	if(myEffect != NULL)
	{
		myEffect->activate();
	}

	switch(myPrimitiveType)
	{
	case SimplePrimitive::SolidCube:
		glutSolidCube(mySize);
		break;
	case SimplePrimitive::SolidSphere:
		glutSolidSphere(mySize, myResolution1, myResolution2);
		break;
	case SimplePrimitive::SolidTeapot:
		GfxUtils::drawSolidTeapot(mySize);
		break;
	};

	if(myEffect != NULL)
	{
		myEffect->deactivate();
	}
}
