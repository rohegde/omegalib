/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * GfxUtils implements static utility methods for 2d and 3d drawing. Most of these methods are useful for debug 
 * purposes or for the rapid developement of sample applications.
 *********************************************************************************************************************/
#ifndef __GFX_UTILS_H__
#define __GFX_UTILS_H__

#include "osystem.h"

namespace omega
{
namespace gfx
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GfxUtils
{
public:
	enum Font { Helvetica12, Helvetica18 };

public:
	static void Begin2DCoords(int width, int height);
	static void Begin2DCoords();
	static void End2DCoords();
	static void Print(int x, int y, const char* s, Font f = Helvetica12);

private:
	GfxUtils() {};
};

}; // namespace gfx
}; // namespace omega

#endif