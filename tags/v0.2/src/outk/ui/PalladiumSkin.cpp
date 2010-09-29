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
#include "outk/gfx/GfxUtils.h"
#include "outk/ui/PalladiumSkin.h"

using namespace omega;
using namespace outk::gfx;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PalladiumButton::draw()
{
	Button::draw();

	myAnim *= 0.8f;
	if(myPressed) myAnim = 1.0f;

	GfxUtils::drawVGradient(myPosition, mySize, Color(80, 80, 100, 100 + myAnim * 100), Color(80, 80, 100, 100 + myAnim * 100));

	glColor4ub(255, 255, 255, 255);
	GfxUtils::drawDRect(myPosition, mySize, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PalladiumSlider::draw()
{
	//float d = 0.5f;
	//if(myPressed) d = 1.0f;

	Vector2f sliderPos, sliderSize;
	getSliderCoords(&sliderPos, &sliderSize);

	Label* lbl = getLabel();
	if(!getProgressBarMode())
	{
		lbl->setPosition(sliderPos);
		lbl->setSize(sliderSize);
		lbl->setColor(Color(0, 0, 0,  255));
	}
	else
	{
		lbl->setPosition(myPosition + Vector2f(mySize[0], 0));
		lbl->setSize(mySize);
		lbl->setHorizontalAlign(Label::AlignLeft);
		lbl->setColor(Color(255, 255, 255,  255));
	}

	myAnim *= 0.8f;
	if(myPressed) myAnim = 1.0f;

	GfxUtils::drawVGradient(sliderPos, sliderSize, Color(200, 200, 250, 200 + myAnim * 50 ), Color(200, 200, 250, 200 + myAnim * 50), 0);

	glColor4ub(255, 255, 255, 255);
	GfxUtils::drawDRect(myPosition, mySize, 0);

	Slider::draw();
}
