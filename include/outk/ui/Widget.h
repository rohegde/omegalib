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
#include "outk/ui/UIManager.h"

namespace outk
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Widget
	{
	friend class UIManager;
	public:
		OUTK_API Widget(omega::String name);
		virtual ~Widget();

		//! Gets the color used when widget debug mode is enabled.
		omega::Color getDebugColor() { return myDebugModeColor; }

		//! Sets the color used when widget debug mode is enabled.
		void setDebugColor( omega::Color value ) { myDebugModeColor = value; }

		//! Returns true if debug mode is enabled for this widget.
		bool isDebugModeEnabled() { return myDebugModeEnabled; }

		//! Enabled or disabled debug mode for this widget.
		//! When debug mode is enabled, the widget bounding box will be displayed.
		void setDebugModeEnabled(bool value) { myDebugModeEnabled = value; }

		//! Gets the widget unique name.
		omega::String getName() { return myName; }

		//! Gets the UIManager handling this widget.
		UIManager* getUI() { return myUI; }
		
		//! Gets the widget position.
		omega::Vector2f getPosition() { return myPosition; }

		//! Gets the widget position.
		//! @param index - the element of the position vector that is getting returned (0 for x, 1 for y)
		float getPosition(int index) { return myPosition[index]; }

		//! Sets the widget position
		void setPosition(omega::Vector2f& value) { myPosition = value; }

		//! Sets the widget position
		void setPosition(float x, float y) { myPosition[0] = x; myPosition[1] = y; }

		//! Sets the widget rotation
		//! @param value - the widget rotation in degrees
		void setRotation(float value) { myRotation = value; }

		//! Gets the widget position.
		float getRotation() { return myRotation; }

		//! sets the widget scale.
		//void setScale(float value) { myScale = value; }

		//! Gets the widget scale.
		//float getScale() { return myScale; }

		//! Gets the widget size
		omega::Vector2f getSize() { return mySize; }

		//! Gets the widget size
		//! @param index - the element of the size vector that is getting returned (0 for width, 1 for height)
		float getSize(int index) { return mySize[index]; }
		
		//! Sets the widget size.
		void setSize(omega::Vector2f& value) { mySize = value; }

		//! Sets the widget size.
		void SetSize(float width, float height) { mySize[0] = width; mySize[1] = height; }

		bool isVisible() { return myVisible; }
		void setVisible(bool value) { myVisible = value; }

		bool isUserMoveEnabled() { return myUserMoveEnabled; }
		void setUserMoveEnabled(bool value) { myUserMoveEnabled = value; }


		OUTK_API bool hitTest(const omega::Vector2f& point);
		OUTK_API static bool hitTest(const omega::Vector2f& point, const omega::Vector2f& pos, const omega::Vector2f& size);

		// Add a child to this widget.
		OUTK_API void addChild(Widget* child);

		// Remove a child from this widget.
		OUTK_API  void removeChild(Widget* child);

	protected:
		void render();

		void transformPoint(omega::Vector2f& point);

		virtual void update(const omega::UpdateContext& context);

		// Drawing function for this widget.
		virtual void layoutChildren();

		virtual void draw();

		virtual bool processInputEvent(const omega::InputEvent& evt);

		virtual void dispatchUIEvent(UIEvent& evt);

	protected:
		UIManager* myUI;

		omega::String myName;
		std::list<Widget*> myChildren;
		Widget* myParent;

		// Debug mode.
		bool myDebugModeEnabled;
		omega::Color myDebugModeColor;

		bool myUserMoveEnabled;
		bool myMoving;
		omega::Vector2f myUserMovePosition;

		bool myVisible;

		omega::Vector2f myPosition;
		omega::Vector2f mySize;
		float myRotation;
		//float myScale;

		// Last handled input event.
		omega::InputEvent myLastEvent;

	private:
		void setUI(UIManager* ui);
	};
};
}; // namespace omega

#endif