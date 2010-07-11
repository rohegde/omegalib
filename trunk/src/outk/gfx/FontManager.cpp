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
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "outk/gfx/FontManager.h"

using namespace omega;
using namespace outk::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Font::render(const omega::String& text, omega::Vector2f position) 
{ 
	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f);
	myFontImpl->Render(text.c_str(), text.length(), FTPoint(position[0], -position[1], 0.0f)); 
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FontManager::FontManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FontManager::~FontManager() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Font* FontManager::getFont(omega::String fontName)
{
	return myFonts[fontName];
}
