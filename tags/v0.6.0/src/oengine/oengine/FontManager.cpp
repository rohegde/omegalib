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
#include "oengine/FontManager.h"

#include "omega/DataManager.h"
#include "omega/Lock.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"
#include "omega/StringUtils.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f Font::computeSize(const omega::String& text) 
{ 
	FTBBox bbox = myFontImpl->BBox(text.c_str());
	Vector2f size = Vector2f((int)bbox.Upper().Xf(), (int)bbox.Upper().Yf());
	return size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Font::render(const omega::String& text, float x, float y) 
{ 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f);

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
		ofwarn("FontManager::createFont: font '%1%' already existing.", %fontName);
		return;
	}

	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo info = dm->getInfo(filename);
	oassert(!info.isNull());
	oassert(info.local);

	myLock.lock();

	FTFont* fontImpl = new FTTextureFont(info.path.c_str());

	myLock.unlock();

	//delete data;

	if(fontImpl->Error())
	{
		ofwarn("Font %1% failed to open", %filename);
		delete fontImpl;
	}

	if(!fontImpl->FaceSize(size))
	{
		ofwarn("Font %1% failed to set size %2%", %filename %size);
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