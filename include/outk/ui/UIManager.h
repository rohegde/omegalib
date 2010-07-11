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
#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "omega.h"

#include "outk/gfx/FontManager.h"
#include "outk/ui/Widget.h"

namespace outk
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UIManager
{
public:
	OUTK_API UIManager();
	OUTK_API ~UIManager();

	OUTK_API void update(const omega::UpdateContext& context);
	OUTK_API void draw();

	Widget* getRootWidget() { return myRootWidget; }

	outk::gfx::Font* getDefaultFont() { return myDefaultFont; }
	void setDefaultFont(outk::gfx::Font* value) { myDefaultFont = value; }

private:
	Widget* myRootWidget;
	outk::gfx::Font* myDefaultFont;
};
};
}; // namespace omega

#endif