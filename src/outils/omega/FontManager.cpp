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
#include "omega/FontManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Recti Font::computeBoundingBox(const omega::String& text) 
{ 
	FTBBox bbox = myFontImpl->BBox(text.c_str());

	Vector2i pos = Vector2i((int)bbox.Lower().Xf(), (int)bbox.Lower().Yf());
	Vector2i size = Vector2i((int)bbox.Upper().Xf(), (int)bbox.Upper().Yf()) - pos;

	Recti rect;
	rect[0] = pos;
	rect[1] = size;
	return rect;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Font::render(const omega::String& text, omega::Vector2f position, unsigned int align) 
{ 
	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f);

	FTBBox bbox = myFontImpl->BBox(text.c_str());

	// string height and width
	float ht = bbox.Upper().Yf() - bbox.Lower().Yf();
	float wt = bbox.Upper().Xf() - bbox.Lower().Xf();

	float x, y;

	if(align & HALeft) x = position[0];
	else if(align & HARight) x = position[0] - wt;
	else x = position[0] - wt / 2;

	if(align & VATop) y = -position[1] - ht;
	else if(align & VABottom) y = -position[1];
	else y = -position[1] - ht / 2;

	myFontImpl->Render(text.c_str(), text.length(), FTPoint(x, y, 0.0f)); 
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
FontManager::FontManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
FontManager::~FontManager() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FontManager::createFont(omega::String fontName, omega::String filename, int size)
{
	if(getFont(fontName))
	{
		owarn("FontManager::createFont: font '%s' already existing.", fontName);
		return;
	}

	FTFont* fontImpl = new FTTextureFont(filename.c_str());
	if(fontImpl->Error())
	{
		owarn("Font %s failed to open", filename.c_str());
		delete fontImpl;
	}

	if(!fontImpl->FaceSize(size))
	{
		owarn("Font %s failed to set size %i", filename, size);
		delete fontImpl;
	}

	Font* font = new Font(fontImpl);

	myFonts[fontName] = font;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Font* FontManager::getFont(omega::String fontName)
{
	return myFonts[fontName];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FontManager::cleanup()
{
}