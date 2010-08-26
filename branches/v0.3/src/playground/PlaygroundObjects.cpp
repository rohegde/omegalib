/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Nightfield is an application that tests most of omegalib & omega features.
 *********************************************************************************************************************/
#include "PlaygroundUI.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NightfieldUI::NightfieldUI(TextureManager* texMng, FontManager* fontMng):
	myTexMng(texMng), myFontMng(fontMng)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldUI::initialize()
{
	myFontMng->createFont("arial10", "../../data/fonts/Arial.ttf", 12);
	myFont = myFontMng->getFont("arial10");

	myUI.setEventHandler(this);
	myUI.setDefaultFont(myFont);

	WidgetFactory* wf = myUI.getWidgetFactory();

	Box* b1 = wf->createBox("box1", myUI.getRootWidget(), Box::LayoutVertical);
	b1->setPosition(Vector2f(10, 50));
	b1->setSize(Vector2f(300, 120));

	Button* btn1 = wf->createButton("L1", b1);
	btn1->setText("Hello Button!");

	Slider* sld1 = wf->createSlider("S1", b1);
	sld1->setProgressBarMode(false);

	Slider* sld2 = wf->createSlider("S2", b1);
	sld2->setProgressBarMode(true);

	myPresetSelector = wf->createSlider("presetSelector", myUI.getRootWidget());
	myPresetSelector->setPosition(10, 10);
	myPresetSelector->setSize(300, 30);
	myPresetSelector->setMinValue(0);
	myPresetSelector->setMaxValue(3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldUI::handleUIEvent(const UIEvent& evt)
{
	printf("click yay!\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldUI::draw(const DrawContext& context)
{
	GfxUtils::beginOverlayMode(context);

	glEnable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myUI.draw(context);

	GfxUtils::endOverlayMode();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NightfieldUI::update(const UpdateContext& context)
{
	myUI.update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool NightfieldUI::handleEvent(const InputEvent& evt)
{
	switch(evt.serviceType)
	{
	case InputService::Touch:
	case InputService::Pointer:
		myUI.processInputEvent(evt);
	break;
	}
	return false;
}



