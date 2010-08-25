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
#ifndef __IUI_EVENT_HANDLER_H__
#define __IUI_EVENT_HANDLER_H__

#include "omega/InputEvent.h"

namespace omega
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward class declarations.
class Widget;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UIEvent
{
	enum Type { Click };

	UIEvent(Widget* _source, Type _type, omega::InputEvent* _inputEvent):
	type(_type), source(_source), inputEvent(_inputEvent) {}
		

	Widget* source;
	Type type;
	omega::InputEvent* inputEvent;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IUIEventHandler
{
public:
	virtual void handleUIEvent(const UIEvent& evt) = 0;
};
};
}; // namespace omega

#endif