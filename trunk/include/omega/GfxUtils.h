/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
		static Ray getViewRay(const Vector2f& pos);

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