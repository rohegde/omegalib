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
#ifndef __TEXTURED_QUAD_H__
#define __TEXTURED_QUAD_H__

#include <omega.h>
#include <oengine.h>

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class TexturedQuad: public SceneObject
	{
	OMEGA_DECLARE_TYPE(TexturedQuad)
	public:
		TexturedQuad();
		~TexturedQuad();
		virtual Renderable* createRenderable();

		const AlignedBox3* getBoundingBox();
		bool hasBoundingBox();

		ImageData* getImage();
		void setImage(ImageData* value);

	private:
		AlignedBox3 myBoundingBox;
		ImageData* myImage;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class TexturedQuadRenderable: public SceneRenderable
	{
	OMEGA_DECLARE_TYPE(TexturedQuadRenderable)
	public:
		TexturedQuadRenderable(TexturedQuad* tq);
		~TexturedQuadRenderable();
		virtual void initialize();
		virtual void dispose();
		void draw(RenderState* state);
		void refresh();

	private:
		TexturedQuad* myTexturedQuad;
		VertexBuffer* myVertexBuffer;
		Texture* myTexture;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ImageData* TexturedQuad::getImage() 
	{ return myImage; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void TexturedQuad::setImage(ImageData* value) 
	{ myImage = value; refresh(); }
}; // namespace oengine

#endif