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
#include "outk/ui/Label.h"
#include "outk/ui/UIManager.h"

using namespace omega;
using namespace outk::ui;
using namespace outk::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Label::Label(omega::String name):
	Widget(name),
	myText(name),
	myFont(NULL),
	myColor(255, 255, 255),
	myVerticalAlign(AlignMiddle),
	myHorizontalAlign(AlignCenter)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Label::~Label()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Label::getFontAlignFlags()
{
	unsigned int alignFlags = 0;
	switch(myHorizontalAlign)
	{
	case AlignRight: alignFlags |= Font::HARight; break;
	case AlignLeft: alignFlags |= Font::HALeft; break;
	case AlignCenter: alignFlags |= Font::HACenter; break;
	}
	switch(myVerticalAlign)
	{
	case AlignTop: alignFlags |= Font::VATop; break;
	case AlignBottom: alignFlags |= Font::VABottom; break;
	case AlignMiddle: alignFlags |= Font::VAMiddle; break;
	}
	return alignFlags;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Label::draw()
{
	Widget::draw();

	// If not font has been set, use default ui font.
	if(!myFont) myFont = myUI->getDefaultFont();

	if(myFont)
	{
		if(myFont)
		{
			unsigned int alignFlags = getFontAlignFlags();
			Vector2f textPos = myPosition;

			if(alignFlags & Font::HARight) textPos[0] += mySize[0];
			else if(alignFlags & Font::HACenter) textPos[0] += mySize[0] / 2;

			if(alignFlags & Font::VABottom) textPos[1] += mySize[1];
			else if(alignFlags & Font::VAMiddle) textPos[1] += mySize[1] / 2;

			glColor4ubv(myColor);
			myFont->render(myText, textPos, alignFlags);
		}
	}
}
