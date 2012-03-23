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
 * Exposes some basic math functions / constants.
 *********************************************************************************************************************/
#ifndef __MATH_H__
#define __MATH_H__

#include "osystem.h"

#include <math.h>

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math
{
public:
	OMEGA_API static const float Pi;
	OMEGA_API static const float DegToRad;
	OMEGA_API static const float RadToDeg;

	OMEGA_API static float rnd();

private:
	Math() {}
};
}; // namespace omega

#endif