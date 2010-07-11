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
#include "outk/ui/UIManager.h"

using namespace omega;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UIManager::UIManager():
	myRootWidget(new Widget("")),
	myDefaultFont(NULL)
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
void UIManager::draw()
{
	myRootWidget->layoutChildren();
	myRootWidget->draw();
}
