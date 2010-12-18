/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Playground is a simple application designed to test event and gesture handling in a multithreaded / distributed 
 * application.
 *********************************************************************************************************************/
#include "MeshViewerUI.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshViewerUI::MeshViewerUI(TextureManager* texMng, FontManager* fontMng):
	myTexMng(texMng), myFontMng(fontMng)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::initialize()
{
	myFontMng->createFont("arial10", "../../data/fonts/Arial.ttf", 12);
	myFont = myFontMng->getFont("arial10");

	myUI.setEventHandler(this);
	myUI.setDefaultFont(myFont);

	WidgetFactory* wf = myUI.getWidgetFactory();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::handleUIEvent(const UIEvent& evt)
{
	printf("click yay!\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerUI::draw(const DrawContext& context)
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
void MeshViewerUI::update(const UpdateContext& context)
{
	myUI.update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerUI::handleEvent(const InputEvent& evt)
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



