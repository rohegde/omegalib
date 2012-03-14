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
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "oenginebase.h"
#include "oengine/Font.h"
#include "omega/Application.h"
#include "omega/Color.h"
#include "omega/Texture.h"
#include "omega/GpuBuffer.h"

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API Renderer
	{
	public:
		enum FlipFlags { FlipX = 1 << 1, FlipY = 1 << 2 };
		enum DrawType { DrawTriangles, DrawLines, DrawPoints };

	public:
		Renderer();

		//! Renderer options
		//@{
		void setTargetTexture(Texture* texture);
		Texture* getTargetTexture();
		//@}

		//! Drawing control
		//@{
		void beginDraw3D(const DrawContext& context);
		void beginDraw2D(const DrawContext& context);
		void endDraw();
		bool isDrawing();
		//@}

		//! Drawing control
		//@{
		void pushTransform(const AffineTransform3& transform);
		void popTransform();
		//@}

		//! 2D Drawing methods
		//@{
		void drawRectGradient(Vector2f pos, Vector2f size, Orientation orientation, 
			Color startColor, Color endColor, float pc = 0.5f);
		void drawRect(Vector2f pos, Vector2f size, Color color);
		void drawRectOutline(Vector2f pos, Vector2f size, Color color);
		void drawText(const String& text, Font* font, const Vector2f& position, unsigned int align);
		void drawRectTexture(Texture* texture, const Vector2f& position, const Vector2f size, uint flipFlags = 0);
		void drawCircleOutline(Vector2f position, float radius, const Color& color, int segments);
		//@}

		//! 3D Drawing methods
		//@{
		void drawWireSphere(const Color& color, int segments, int slices);
		void drawIndexedPrimitives(VertexBuffer* vertices, uint* indices, uint size, DrawType type);
		//@}

		//! Font management
		//@{
		Font* createFont(omega::String fontName, omega::String filename, int size);
		Font* getFont(omega::String fontName);
		Font* getDefaultFont();
		void setDefaultFont(Font* value);
		//@}

	private:
		bool myDrawing;
		Texture* myTargetTexture;
		Dictionary<String, Font*> myFonts;
		Font* myDefaultFont;
		Lock myLock;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Renderer::isDrawing()
	{ return myDrawing; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Renderer::setTargetTexture(Texture* texture)
	{ myTargetTexture = texture; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Texture* Renderer::getTargetTexture()
	{ return myTargetTexture; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Font* Renderer::getDefaultFont()
	{ return myDefaultFont; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Renderer::setDefaultFont(Font* value)
	{ myDefaultFont = value; }
}; // namespace oengine

#endif