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

#include "omega/osystem.h"
#include "omega/DrawContext.h"

namespace outk
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
	OMEGA_API static void BeginOverlayMode(omega::DrawContext& context);

	// Restores the coordinate system specified before a Begin2DCoords call.
	OMEGA_API static void EndOverlayMode();

	// Prints text. 
	OMEGA_API static void DrawText(int x, int y, const char* s, Font f = Helvetica12);

	OMEGA_API static void DrawSolidTeapot(float size);
	OMEGA_API static void DrawWireTeapot(float size);

private:
	GfxUtils() {};
};

}; // namespace gfx
}; // namespace omega

#endif