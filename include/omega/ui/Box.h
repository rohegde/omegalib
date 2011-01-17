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
class OUTILS_API Box: public Widget
{
public: 
	enum Layout { LayoutNone, LayoutHorizontal, LayoutVertical };
public:
	Box(omega::String name, Layout layout);
	virtual ~Box();

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