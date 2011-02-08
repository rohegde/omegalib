/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "omega/Color.h"
#include "omega/ui/UIManager.h"

namespace omega
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API Widget
	{
	friend class UIManager;
	friend class Container;
	public:
		enum Orientation {Horizontal = 0, Vertical = 1};

	public:
		Widget(omega::String name);
		virtual ~Widget();

		//! Gets the color used when widget debug mode is enabled.
		Color getDebugColor() { return myDebugModeColor; }

		//! Sets the color used when widget debug mode is enabled.
		void setDebugColor( omega::Color value ) { myDebugModeColor = value; }

		//! Returns true if debug mode is enabled for this widget.
		bool isDebugModeEnabled() { return myDebugModeEnabled; }

		//! Enabled or disabled debug mode for this widget.
		//! When debug mode is enabled, the widget bounding box will be displayed.
		void setDebugModeEnabled(bool value) { myDebugModeEnabled = value; }

		//! Gets the widget unique name.
		String getName() { return myName; }

		//! Position and rotation
		//@{
		//! Gets the widget position.
		Vector2i& getPosition() { return myPosition; }
		//! Sets the widget position
		void setPosition(const omega::Vector2i& value) { myPosition = value; }
		void setPosition(int value, int dimension) { myPosition[dimension] = value; }
		//! Sets the widget rotation
		//! @param value - the widget rotation in degrees
		void setRotation(float value) { myRotation = value; }
		//! Gets the widget position.
		float getRotation() { return myRotation; }
		//@}

		//! sets the widget scale.
		//void setScale(float value) { myScale = value; }
		//! Gets the widget scale.
		//float getScale() { return myScale; }

		//! Size constraints
		//@{
		//! Gets the widget size
		Vector2i& getSize();
		float getSize(Orientation orientation);
		float getWidth();
		float getHeight();
		//! Sets the widget size.
		void setSize(const omega::Vector2i& value);
		void setWidth(float value);
		void setHeight(float value);
		Vector2i& getMinimumSize();
		Vector2i& getMaximumSize();
		void setMinimumSize(const Vector2i& value);
		int getMinimumWidth();
		int getMinimumHeight();
		void setMaximumSize(const Vector2i& value);
		int getMaximumWidth();
		int getMaximumHeight();
		void setMinimumWidth(float value);
		void setMinimumHeight(float value);
		void setMaximumWidth(float value);
		void setMaximumHeight(float value);
		void clearSizeConstaints();
		void setAutosize(bool value);
		bool getAutosize();
		virtual void autosize() {}
		virtual void updateSize();
		//@}

		bool isVisible();
		void setVisible(bool value);

		//bool isUserMoveEnabled() { return myUserMoveEnabled; }
		//void setUserMoveEnabled(bool value) { myUserMoveEnabled = value; }
		
		bool hitTest(const omega::Vector2f& point);
		static bool hitTest(const omega::Vector2f& point, const omega::Vector2f& pos, const omega::Vector2f& size);

		virtual void draw();

		UIManager* getUIManager();
		virtual void setUIManager(UIManager* ui) { myUIMng = ui; }

	protected:
		//! internal layout management
		//@{
		void requestLayoutRefresh();
		bool needLayoutRefresh();
		virtual void layout();
		void setActualSize(int value, Orientation orientation);
		//@}

		void setContainer(Container* value);
		Vector2f transformPoint(const omega::Vector2f& point);
		virtual void update(const omega::UpdateContext& context) {}
		void preDraw();
		void postDraw();
		virtual void renderContent();
		virtual bool processInputEvent(const InputEvent& evt);
		void dispatchUIEvent(UIEvent& evt);

	private:
		omega::String myName;
		Container* myContainer;
		UIManager* myUIMng;

		bool myNeedLayoutRefresh;

		// Debug mode.
		bool myDebugModeEnabled;
		omega::Color myDebugModeColor;

		bool myUserMoveEnabled;
		bool myMoving;
		omega::Vector2f myUserMovePosition;

		bool myVisible;

		omega::Vector2i myPosition;
		omega::Vector2i mySize;
		float myRotation;
		//float myScale;

		// Size constraints.
		omega::Vector2i myMinimumSize;
		omega::Vector2i myMaximumSize;
		bool myAutosize;

		// Last handled input event.
		omega::InputEvent myLastEvent;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline UIManager* Widget::getUIManager() 
	{ oassert(myUIMng != NULL); return myUIMng; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setAutosize(bool value)
	{ 
		myAutosize = value; 
		requestLayoutRefresh();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Widget::getAutosize()
	{ return myAutosize; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2i& Widget::getSize() 
	{ return mySize; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Widget::getSize(Orientation orientation)
	{
		return mySize[orientation];
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Widget::getWidth()
	{ return mySize[0]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Widget::getHeight() 
	{ return mySize[1]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setSize(const omega::Vector2i& value) 
	{ 
		requestLayoutRefresh(); 
		mySize = value; 
		myMinimumSize = value; 
		myMaximumSize = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setWidth(float value) 
	{ 
		requestLayoutRefresh(); 
		mySize[0] = value; 
		myMinimumSize[0] = value; 
		myMaximumSize[0] = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setHeight(float value) 
	{ 
		requestLayoutRefresh(); 
		mySize[1] = value; 
		myMinimumSize[1] = value; 
		myMaximumSize[1] = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2i& Widget::getMinimumSize() 
	{ return myMinimumSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2i& Widget::getMaximumSize() 
	{ return myMaximumSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setMinimumSize(const Vector2i& value) 
	{
		requestLayoutRefresh(); 
		myMinimumSize = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Widget::getMinimumWidth() 
	{ return myMinimumSize[0]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Widget::getMinimumHeight() 
	{ return myMinimumSize[1]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setMaximumSize(const Vector2i& value) 
	{
		requestLayoutRefresh(); 
		myMaximumSize = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Widget::getMaximumWidth() 
	{ return myMaximumSize[0]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Widget::getMaximumHeight() 
	{ return myMaximumSize[1]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setMinimumWidth(float value) 
	{ 
		requestLayoutRefresh(); 
		myMinimumSize[0] = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setMinimumHeight(float value) 
	{
		requestLayoutRefresh(); 
		myMinimumSize[1] = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setMaximumWidth(float value) 
	{ 
		requestLayoutRefresh(); 
		myMaximumSize[0] = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setMaximumHeight(float value) 
	{ 
		requestLayoutRefresh(); 
		myMaximumSize[1] = value; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Widget::isVisible() 
	{ return myVisible; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Widget::setVisible(bool value) 
	{ myVisible = value; }
};
}; // namespace omega

#endif