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

#include "omega/osystem.h"
#include "omega/InputManager.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MouseService: public InputService
{
public:
	static void mouseMotionCallback(int x, int y);

	virtual void Initialize();
	virtual void Dispose();

private:
	static MouseService* myInstance;
};

}; // namespace omega

#endif