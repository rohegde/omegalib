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
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "omega.h"

namespace outk
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Widget
	{
	friend class UIManager;
	public:
		Widget(omega::String name);
		virtual ~Widget();

		omega::String getName() { return myName; }
		UIManager* getUI() { return myUI; }
		
		// Gets the widget position
		omega::Vector2f getPosition() { return myPosition; }
		float getPosition(int index) { return myPosition[index]; }
		// Sets the widget position
		void setPosition(omega::Vector2f& value) { myPosition = value; }
		void setPosition(float x, float y) { myPosition[0] = x; myPosition[1] = y; }

		// Gets the widget size
		omega::Vector2f getSize() { return mySize; }
		float getSize(int index) { return mySize[index]; }
		
		// Sets the widget size.
		void setSize(omega::Vector2f& value) { mySize = value; }
		void SetSize(float width, float height) { mySize[0] = width; mySize[1] = height; }

		bool isVisible() { return myVisible; }
		void setVisible(bool value) { myVisible = value; }

		omega::Color getDebugColor() { return myDebugModeColor; }
		void setDebugColor( omega::Color value ) { myDebugModeColor = value; }

		bool isDebugModeEnabled() { return myDebugModeEnabled; }
		void setDebugModeEnabled(bool value) { myDebugModeEnabled = value; }

		// Add a child to this widget.
		OUTK_API void addChild(Widget* child);

		// Remove a child from this widget.
		OUTK_API  void removeChild(Widget* child);

	public:
		virtual void update(const omega::UpdateContext& context) {};

		// Drawing function for this widget.
		virtual void layoutChildren();

		virtual void draw();

	protected:
		UIManager* myUI;

		omega::String myName;
		std::list<Widget*> myChildren;
		Widget* myParent;

		// Debug mode.
		bool myDebugModeEnabled;
		omega::Color myDebugModeColor;

		bool myVisible;

		omega::Vector2f myPosition;
		omega::Vector2f mySize;
	};
};
}; // namespace omega

#endif