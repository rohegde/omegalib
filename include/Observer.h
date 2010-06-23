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
	Observer();

	Matrix4f GetWorldToEmitter() { return myWorldToEmitter; }
	void SetWorldToEmitter(Matrix4f value) { myWorldToEmitter = value; }

	Matrix4f GetSensorToObject() { return mySensorToObject; }
	void SetSensorToObject(Matrix4f value) { mySensorToObject = value; }

	Matrix4f GetHeadMatrix() { return myHeadMatrix; }

	void Update(float x, float y, float z, float yaw, float pitch, float roll);

private:
	Matrix4f myWorldToEmitter;
	Matrix4f mySensorToObject;
	Matrix4f myHeadMatrix;
};

}; // namespace omega

#endif