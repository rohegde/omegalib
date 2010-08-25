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
#ifndef __BOX_H__
#define __BOX_H__

#include "omega/ui/Widget.h"

namespace omega
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Box: public Widget
{
public: 
	enum Layout { LayoutNone, LayoutHorizontal, LayoutVertical };
public:
	OMEGA_API Box(omega::String name, Layout layout);
	OMEGA_API virtual ~Box();

	// Gets the widget size
	Layout getLayout() { return myLayout; }
	// Sets the widget size.
	void setLayout(Layout value) { myLayout = value; }

	float getPadding() { return myPadding; }
	void setPadding(float value) { myPadding = value; }

public:
	// Drawing function for this widget.
	virtual void draw();

	// Drawing function for this widget.
	virtual void layoutChildren();

private:
	Layout myLayout;
	float myPadding;
};
}; // namespace gfx
}; // namespace omega

#endif