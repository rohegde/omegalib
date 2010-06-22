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
#include "DrawContext.h"

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

	// Changes the coordinate system to pixel-based 2d coords. The coordinate system viewport will correspond to the
	// viewport parameters specified in the draw context.
	static void Begin2DCoords(DrawContext& context);

	// Restores the coordinate system specified before a Begin2DCoords call.
	static void End2DCoords();

	// Prints text. The text position is specified in raster coordinates and does not take into account any viewport
	// specification.
	static void RasterPrint(int x, int y, const char* s, Font f = Helvetica12);

	static void DrawSolidTeapot(float size);
	static void DrawWireTeapot(float size);

private:
	GfxUtils() {};
};

}; // namespace gfx
}; // namespace omega

#endif