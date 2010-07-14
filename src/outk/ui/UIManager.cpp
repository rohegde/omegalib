/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "outk/ui/PalladiumSkin.h"
#include "outk/ui/UIManager.h"
#include "outk/ui/Widget.h"

using namespace omega;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::UIManager():
	myRootWidget(new Widget("")),
	myWidgetFactory(new PalladiumWidgetFactory()),
	myDefaultFont(NULL),
	myEventHandler(NULL)
{
	myRootWidget->myUI = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::~UIManager() 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::update(const UpdateContext& context)
{
	myRootWidget->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::draw(const DrawContext& context)
{
	myRootWidget->setPosition(Vector2f(context.viewportX, context.viewportY));
	myRootWidget->setSize(Vector2f(context.viewportWidth, context.viewportHeight));

	myRootWidget->layoutChildren();
	myRootWidget->render();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::processInputEvent(const InputEvent& evt)
{
	// UI widgets only manage touch and pointer events.
	if(evt.serviceType == InputService::Touch || evt.serviceType == InputService::Pointer)
	{
		myRootWidget->processInputEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UIManager::dispatchUIEvent(const UIEvent& evt)
{
	if(myEventHandler)
	{
		myEventHandler->handleUIEvent(evt);
	}
}
