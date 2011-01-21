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
 * GfxUtils implements static utility methods for 2d and 3d drawing. Most of these methods are useful for debug 
 * purposes or for the rapid developement of sample applications.
 *********************************************************************************************************************/
#ifndef __GFX_UTILS_H__
#define __GFX_UTILS_H__

#include "osystem.h"
#include "omega/Application.h"
#include "omega/Color.h"
#include "omega/DrawContext.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API GfxUtils
	{
	public:
		enum Font { Helvetica12, Helvetica18 };

	public:
		static void getViewRay(float viewX, float viewY, omega::Vector3f* origin, omega::Vector3f* direction);

		//! Changes the coordinate system to pixel-based 2d coords. The coordinate system viewport will correspond to the
		//! viewport parameters specified in the draw context.
		static void beginOverlayMode(const omega::DrawContext& context);

		//! Restores the coordinate system specified before a Begin2DCoords call.
		static void endOverlayMode();

		//! Light management
		//@{
		//! Enables or disables lighting support.
		static void setLightingEnabled(bool value);
		static void setLightEnabled(int lightId, bool value);
		static void setLightPosition(int lightId, const Vector3f& pos);
		static void setLightColor(int lightId, const Color& pos);
		//@}

		//! Drawing functions
		//@{
		//! Prints text. 
		static void drawText(int x, int y, const char* s, Font f = Helvetica12);

		static void drawSolidTeapot(float size);
		static void drawWireTeapot(float size);

		static void drawHGradient(omega::Vector2i pos, omega::Vector2i size, omega::Color startColor, omega::Color endColor, float pc = 0.0f);
		static void drawVGradient(omega::Vector2i pos, omega::Vector2i size, omega::Color startColor, omega::Color endColor, float pc = 0.0f);

		static void drawDRect(omega::Vector2f pos, omega::Vector2f size, int d);
		//@}
	private:
		GfxUtils() {};
	};
}; // namespace omega

#endif