/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "osystem.h"

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Observer
{
public:
	OMEGA_API Observer();

	OMEGA_API Matrix4f GetWorldToEmitter() { return myWorldToEmitter; }
	OMEGA_API void SetWorldToEmitter(Matrix4f value) { myWorldToEmitter = value; }

	OMEGA_API Matrix4f GetSensorToObject() { return mySensorToObject; }
	OMEGA_API void SetSensorToObject(Matrix4f value) { mySensorToObject = value; }

	OMEGA_API Matrix4f GetHeadMatrix() { return myHeadMatrix; }

	OMEGA_API void Update(float x, float y, float z, float yaw, float pitch, float roll);

private:
	Matrix4f myWorldToEmitter;
	Matrix4f mySensorToObject;
	Matrix4f myHeadMatrix;
};

}; // namespace omega

#endif