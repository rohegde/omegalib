/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Test executable
 *********************************************************************************************************************/
#ifndef __NIGHTFIELD_UI_H__
#define __NIGHTFIELD_UI_H__

#include "omega.h"
#include "outk.h"

// Using namespace declarations in a header file make emokitteh cry
// but we'll accept them here since it's application-level code, and we can have
// declarations shorter and more readable for the sake of clarity.
using namespace omega;
using namespace outk::gfx;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NightfieldUI: public IUIEventHandler
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	NightfieldUI(TextureManager* texMng, FontManager* fontMng);
	void initialize();
	void draw(const DrawContext& context);
	void update(const UpdateContext& context);
	bool handleEvent(const InputEvent& evt);
	void handleUIEvent(const UIEvent& evt);

private:
	// Managers
	TextureManager* myTexMng;
	FontManager* myFontMng;

	// User interface stuff.
	UIManager myUI;
	Font* myFont;

	float myMouseX;
	float myMouseY;
};

#endif