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

	float d = 0.5f;
	if(myPressed) d = 1.0f;

	GfxUtils::drawVGradient(myPosition, mySize, Color(50, 50, 100, 255), Color(100, 100, 150, 255), d);

	glColor4ub(100, 100, 100, 255);
	GfxUtils::drawDRect(myPosition, mySize, 0);
	glColor4ub(255, 255, 255, 255);
	GfxUtils::drawDRect(myPosition, mySize, 1);
	glColor4ub(255, 255, 255, 128);
	GfxUtils::drawDRect(myPosition, mySize, 2);
}
