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
#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include "omega.h"

#include "FTGL/ftgl.h"

#include "boost/unordered_map.hpp"

namespace outk
{
namespace gfx
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Font
	{
	public:
		enum Align {HALeft = 1 << 0, HARight = 1 << 1, HACenter = 1 << 2,
					VATop = 1 << 3, VABottom = 1 << 4, VAMiddle = 1 << 5};
	public:
		Font(FTFont* fontImpl): myFontImpl(fontImpl) {}

		OUTK_API void render(const omega::String& text, omega::Vector2f position, unsigned int align = HALeft | VATop);
	private:
		FTFont* myFontImpl;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef boost::unordered_map<omega::String, Font*> FontDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class FontManager
	{
	public:
		OUTK_API FontManager();
		OUTK_API ~FontManager();

		OUTK_API void createFont(omega::String fontName, omega::String filename, int size);
		OUTK_API Font* getFont(omega::String fontName);

	private:
		FontDictionary myFonts;
	};
};
}; // namespace omega

#endif