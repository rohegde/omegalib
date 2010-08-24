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
	// Allocator function
	static MouseService* New() { return new MouseService(); }

public:
	static void mouseMotionCallback(int x, int y);
	static void mouseButtonCallback(int x, int y, int btn, int k);

	OMEGA_API virtual void initialize();
	OMEGA_API virtual void dispose();

private:
	static MouseService* mysInstance;
};

}; // namespace omega

#endif