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

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Observer::Observer()
{
	myWorldToEmitter = Matrix4f::IDENTITY;
	mySensorToObject = Matrix4f::IDENTITY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Observer::Update(float x, float y, float z, float yaw, float pitch, float roll)
{
	eq::Vector3f pos;
	myHeadMatrix = eq::Matrix4f::IDENTITY;

	myHeadMatrix.rotate_x(pitch);
	myHeadMatrix.rotate_z(roll);
	myHeadMatrix.rotate_y(yaw);

	pos.x() = x;
	pos.y() = y;
	pos.z() = z;

	myHeadMatrix.set_translation(pos);

	myHeadMatrix = myWorldToEmitter * myHeadMatrix * mySensorToObject;
}
