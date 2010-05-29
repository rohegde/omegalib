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
#ifndef __GLUT_MOUSE__SERVICE_H__
#define __GLUT_MOUSE__SERVICE_H__

#include "osystem.h"
#include "InputManager.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GLUTMouseService: public InputService
{
public:
	virtual void Initialize();
	virtual void Dispose();

private:
	static void mouseMotionCallback(int x, int y);

private:
	static GLUTMouseService* myInstance;
};

}; // namespace omega

#endif