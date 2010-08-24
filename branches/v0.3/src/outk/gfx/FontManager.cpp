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
void Font::render(const omega::String& text, omega::Vector2f position, unsigned int align) 
{ 
	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f);

	FTBBox bbox = myFontImpl->BBox(text.c_str());

	// string height
	float ht = bbox.Upper().Yf() - bbox.Lower().Yf();

	// string width
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FontManager::cleanup()
{
}