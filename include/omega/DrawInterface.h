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

#include "osystem.h"
#include "omega/Font.h"
#include "omega/ApplicationBase.h"

#include "omega/Texture.h"
#include "omega/GpuBuffer.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API DrawInterface: public ReferenceType
	{
	public:
		enum FlipFlags { FlipX = 1 << 1, FlipY = 1 << 2 };
		enum DrawType { DrawTriangles, DrawLines, DrawPoints, DrawTriangleStrip };

	public:
		DrawInterface();

		//! DrawInterface options
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
		void drawRectTexture(Texture* texture, const Vector2f& position, const Vector2f size, uint flipFlags = 0, const Vector2f& minUV = Vector2f::Zero(), const Vector2f& maxUV = Vector2f::Ones());
		void drawCircleOutline(Vector2f position, float radius, const Color& color, int segments);
		//@}

		//! 3D Drawing methods
		//@{
		void drawWireSphere(const Color& color, int segments, int slices);
		void drawPrimitives(VertexBuffer* vertices, uint* indices, uint size, DrawType type);
		//@}

		//! Font management
		//@{
		Font* createFont(omega::String fontName, omega::String filename, int size);
		Font* getFont(omega::String fontName);
		Font* getDefaultFont();
		void setDefaultFont(Font* value);
		//@}

		// Hack
		void setForceDiffuseColor(bool value) { myForceDiffuseColor = value; }

	private:
		bool myDrawing;
		Texture* myTargetTexture;
		Dictionary<String, Font*> myFonts;
		Font* myDefaultFont;
		Lock myLock;

		bool myForceDiffuseColor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool DrawInterface::isDrawing()
	{ return myDrawing; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void DrawInterface::setTargetTexture(Texture* texture)
	{ myTargetTexture = texture; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Texture* DrawInterface::getTargetTexture()
	{ return myTargetTexture; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Font* DrawInterface::getDefaultFont()
	{ return myDefaultFont; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void DrawInterface::setDefaultFont(Font* value)
	{ myDefaultFont = value; }
}; // namespace omega

#endif