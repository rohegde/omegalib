/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __PAINTER_H__
#define __PAINTER_H__

#include "omega/ui/Widget.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class Texture;
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class Painter
	{
	public:
		Painter();

		//! Painter options
		//@{
		void setTargetTexture(Texture* texture);
		Texture* getTargetTexture();
		//@}

		//! Drawing control
		//@{
		void beginDraw();
		void endDraw();
		bool isDrawing();
		//@}

		//! Drawing methods
		//@{
		void drawRectGradient(Vector2f pos, Vector2f size, Widget::Orientation orientation, 
			Color startColor, Color endColor, float pc = 0.5f);
		void drawRect(Vector2f pos, Vector2f size, Color color);
		void drawRectOutline(Vector2f pos, Vector2f size, Color color);
		void drawText(const String& text, Font* font, const Vector2f& position, unsigned int align);
		void drawRectTexture(Texture* texture, const Vector2f& position, const Vector2f size);
		//@}

	private:
		bool myDrawing;
		Texture* myTargetTexture;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Painter::isDrawing()
	{ return myDrawing; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Painter::setTargetTexture(Texture* texture)
	{ myTargetTexture = texture; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Texture* Painter::getTargetTexture()
	{ return myTargetTexture; }

}; // namespace ui
}; // namespace omega

#endif