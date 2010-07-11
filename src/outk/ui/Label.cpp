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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Label::Label(omega::String name):
	Widget(name),
	myText(name),
	myFont(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Label::~Label()
{

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
			myFont->render(myText, myPosition);
		}
	}
}
