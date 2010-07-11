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
#ifndef __LABEL_H__
#define __LABEL_H__

#include "omega.h"
#include "outk/gfx/FontManager.h"
#include "outk/ui/Widget.h"

namespace outk
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Label: public Widget
{
public:
	OMEGA_API Label(omega::String name);
	OMEGA_API virtual ~Label();

	omega::String getText() { return myText; }
	void setText(const omega::String& value) { myText = value; }

	outk::gfx::Font* getFont() { return myFont; }
	void setFont(outk::gfx::Font* value) { myFont = value; }

protected:
	virtual void draw();

protected:
	omega::String myText;
	outk::gfx::Font* myFont;
};
}; // namespace gfx
}; // namespace outk

#endif