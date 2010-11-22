/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
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

	OMEGA_API Matrix4f getWorldToEmitter() { return myWorldToEmitter; }
	OMEGA_API void setWorldToEmitter(Matrix4f value) { myWorldToEmitter = value; }

	OMEGA_API Matrix4f getSensorToObject() { return mySensorToObject; }
	OMEGA_API void setSensorToObject(Matrix4f value) { mySensorToObject = value; }

	OMEGA_API Matrix4f getHeadMatrix() { return myHeadMatrix; }

	OMEGA_API void update(const Vector3f& position, const Vector3f& rotation);

private:
	Matrix4f myWorldToEmitter;
	Matrix4f mySensorToObject;
	Matrix4f myHeadMatrix;
};

}; // namespace omega

#endif