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
#ifndef __BASE_BUTTON_H__
#define __BASE_BUTTON_H__

#include "omega.h"
#include "outk/ui/Widget.h"

namespace outk
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AbstractButton: public Widget
{
public:
	OMEGA_API AbstractButton(omega::String name);
	OMEGA_API virtual ~AbstractButton();

	bool isCheckable() { return myCheckable; }
	void setCheckable(bool value) { myCheckable = value; }

	bool isChecked() { return myChecked; }
	void setChecked(bool value) { myChecked = value; }

protected:
	virtual void update(const omega::UpdateContext& context);

protected:
	bool myCheckable;
	bool myChecked;
	bool myPressed;
	bool myPressedStateChanged;
};
}; // namespace gfx
}; // namespace outk

#endif