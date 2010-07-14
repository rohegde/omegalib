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
 * DrawContext
 *********************************************************************************************************************/
#ifndef __COLOR_H__
#define __COLOR_H__

#include "osystem.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Color: public vmml::vector< 4, byte >
	{
	public:
	public:
		Color(byte r, byte g, byte b, byte a = 255):
		  vmml::vector< 4, byte >(r, g, b, a) {}

		static Color floats(float r, float g, float b, float a)
		{
		  return Color((byte)(r * 256), (byte)(g * 256), (byte)(b * 256), (byte)(a * 256));
		}

		operator byte* () { return begin(); }
	};
}; // namespace omega

#endif