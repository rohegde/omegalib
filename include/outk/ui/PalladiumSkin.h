/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Implements the Palladium UI skin.
 *********************************************************************************************************************/
#ifndef __PALLADIUM_SKIN_H__
#define __PALLADIUM_SKIN_H__

#include "outk/ui/WidgetFactory.h"

namespace outk
{
namespace ui
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class PalladiumButton: public Button
{
public:
	PalladiumButton(omega::String name): Button(name) {}
protected:
	OUTK_API void draw();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PalladiumWidgetFactory: public WidgetFactory
{
public:
	virtual Button* createButton(omega::String name, Widget* parent)
	{
		Button* button = new PalladiumButton(name);
		parent->addChild(button);
		return button;
	}
};


}; // namespace ui
}; // namespace outk

#endif