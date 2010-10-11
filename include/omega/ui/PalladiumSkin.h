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

#include "omega/ui/WidgetFactory.h"

namespace omega
{
namespace ui
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PalladiumButton: public Button
{
public:
	PalladiumButton(omega::String name): Button(name) {}
protected:
	OUTILS_API void draw();
private:
	float myAnim;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PalladiumSlider: public Slider
{
public:
	PalladiumSlider(omega::String name): Slider(name) {}
protected:
	OUTILS_API void draw();
private:
	float myAnim;
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

	virtual Slider* createSlider(omega::String name, Widget* parent)
	{
		Slider* slider = new PalladiumSlider(name);
		parent->addChild(slider);
		return slider;
	}
};


}; // namespace ui
}; // namespace omega

#endif