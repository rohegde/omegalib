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
	static const float Pi;
	static const float DegToRad;
	static const float RadToDeg;

private:
	Math() {}
};
}; // namespace omega

#endif