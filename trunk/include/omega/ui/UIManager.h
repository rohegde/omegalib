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
#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "omega/Application.h"
#include "omega/FontManager.h"
#include "omega/ui/IUIEventHandler.h"

namespace omega
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward class declarations.
class Widget;
class WidgetFactory;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OUTILS_API UIManager
{
friend class Widget;
public:
	UIManager();
	~UIManager();

	void update(const omega::UpdateContext& context);
	void draw(const omega::DrawContext& context);
	void processInputEvent(const omega::InputEvent& evt);

	Widget* getRootWidget() { return myRootWidget; }

	omega::Font* getDefaultFont() { return myDefaultFont; }
	void setDefaultFont(omega::Font* value) { myDefaultFont = value; }

	IUIEventHandler* getEventHandler() { return myEventHandler; }
	void setEventHandler(IUIEventHandler* value) { myEventHandler = value; }

	WidgetFactory* getWidgetFactory() { return myWidgetFactory; }
	void setWidgetFactory(WidgetFactory* value) { myWidgetFactory = value; }

private:
	void dispatchUIEvent(const UIEvent& evt);

private:
	Widget* myRootWidget;
	omega::Font* myDefaultFont;
	IUIEventHandler* myEventHandler;
	WidgetFactory* myWidgetFactory;
};
};
}; // namespace omega

#endif