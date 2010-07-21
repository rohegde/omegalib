/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Implementation of Config class methods. See Config.h for detailed information.
 *********************************************************************************************************************/
#include "omega/Observer.h"
#include "omega/Math.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Observer::Observer()
{
	myWorldToEmitter = Matrix4f::IDENTITY;
	mySensorToObject = Matrix4f::IDENTITY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::update(Vector3f position, Vector3f rotation)
{
	myHeadMatrix = eq::Matrix4f::IDENTITY;

	myHeadMatrix.rotate_x(rotation[0] * Math::DegToRad);
	myHeadMatrix.rotate_z(rotation[2] * Math::DegToRad);
	myHeadMatrix.rotate_y(rotation[1] * Math::DegToRad);

	myHeadMatrix.set_translation(position);

	myHeadMatrix = myWorldToEmitter * myHeadMatrix * mySensorToObject;
}
