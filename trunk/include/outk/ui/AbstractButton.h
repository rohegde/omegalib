/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
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
	bool myCheckable;
	bool myChecked;

};
}; // namespace gfx
}; // namespace outk

#endif