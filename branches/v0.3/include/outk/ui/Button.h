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
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "omega.h"
#include "outk/ui/AbstractButton.h"
#include "outk/ui/Label.h"

namespace outk
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button: public AbstractButton
{
public:
	OUTK_API Button(omega::String name);
	OUTK_API virtual ~Button();

	omega::String getText() { return myLabel.getText(); }
	void setText(omega::String value) { myLabel.setText(value); }

	// Gets the label subobject used by the button.
	Label* getLabel() { return &myLabel; }

protected:
		virtual bool processInputEvent(const omega::InputEvent& evt);
		virtual void draw();

private:
	Label myLabel;
};
}; // namespace gfx
}; // namespace outk

#endif