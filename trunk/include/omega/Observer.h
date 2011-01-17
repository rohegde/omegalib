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
class OMEGA_API Observer
{
public:
	Observer();

	Matrix4f getWorldToEmitter() { return myWorldToEmitter; }
	void setWorldToEmitter(Matrix4f value) { myWorldToEmitter = value; }

	Matrix4f getSensorToObject() { return mySensorToObject; }
	void setSensorToObject(Matrix4f value) { mySensorToObject = value; }

	Matrix4f getHeadMatrix() { return myHeadMatrix; }

	void update(const Vector3f& position, const Vector3f& rotation);

private:
	Matrix4f myWorldToEmitter;
	Matrix4f mySensorToObject;
	Matrix4f myHeadMatrix;
};

}; // namespace omega

#endif