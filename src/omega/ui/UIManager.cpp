/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/ui/PalladiumSkin.h"
#include "omega/ui/UIManager.h"
#include "omega/ui/Widget.h"

using namespace omega;
using namespace omega::ui;

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
		//if(evt.type == InputEvent::Move) 
		//	printf("move! at %f %f\n", evt.position[0], evt.position[1]);
		//if(evt.type == InputEvent::Rotate) 
		//	printf("rotate! at %f %f, deg:%f\n", evt.position[0], evt.position[1], evt.rotation[0]);
		//if(evt.type == InputEvent::Split) printf("split!\n");
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
