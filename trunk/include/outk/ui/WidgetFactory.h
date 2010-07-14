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
#ifndef __WIDGET_FACTORY_H__
#define __WIDGET_FACTORY_H__

#include "outk/ui/Box.h"
#include "outk/ui/Button.h"
#include "outk/ui/Widget.h"

namespace outk
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class WidgetFactory
{
public:
	virtual Widget* createWidget(omega::String name, Widget* parent)
	{
		Widget* widget = new Widget(name);
		parent->addChild(widget);
		return widget;
	}

	virtual Box* createBox(omega::String name, Widget* parent, Box::Layout layout)
	{
		Box* box = new Box(name, layout);
		parent->addChild(box);
		return box;
	}

	virtual Button* createButton(omega::String name, Widget* parent)
	{
		Button* button = new Button(name);
		parent->addChild(button);
		return button;
	}
};
}; // namespace ui
}; // namespace outk

#endif