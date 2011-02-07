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
#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "omega/ui/Widget.h"

namespace omega
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	typedef VectorIterator<std::list<Widget*> > WidgetIterator;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API Container: public Widget
	{
	friend class UIManager;
	public:
		enum Layout {LayoutFree, LayoutHorizontal, LayoutVertical};
		enum HorizontalAlign { AlignRight, AlignLeft, AlignCenter};
		enum VerticalAlign { AlignTop, AlignMiddle, AlignBottom};

	public:
		Container(omega::String name);
		virtual ~Container();

		//! Child management
		//@{
		//! Add a child to this widget.
		void addChild(Widget* child);
		//! Remove a child from this widget.
		void removeChild(Widget* child);
		//! Return the number of children
		int getNumChildren();
		//@}


		//!Layout options
		//@{
		void setLayout(Layout layout);
		Layout getLayout();
		float getPadding();
		float getMargin();
		void setPadding(float value);
		void setMargin(float value);
		HorizontalAlign getHorizontalAlign();
		void setHorizontalAlign(HorizontalAlign value);
		VerticalAlign getVerticalAlign();
		void setVerticalAlign(VerticalAlign value);
		//@}

		virtual void draw();
		virtual void setUIManager(UIManager* ui);
		virtual void updateSize();

	protected:
		virtual bool processInputEvent(const omega::InputEvent& evt);
		virtual void update(const omega::UpdateContext& context);
		virtual void layout();

	private:
		int expandStep(int childSpace, Orientation orientation);
		void updateChildrenLayoutPosition(Orientation orientation);
		void updateChildrenFreeBounds(Orientation orientation);
		void resetChildrenSize(Orientation orientation);
		void computeLinearLayout(Orientation orientation);

	private:
		std::list<Widget*> myChildren;
		Layout myLayout;
		float myPadding;
		float myMargin;
		HorizontalAlign myHorizontalAlign;
		VerticalAlign myVerticalAlign;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Container::getNumChildren() 
	{ return myChildren.size(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Container::setLayout(Layout layout) 
	{ myLayout = layout; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Container::Layout Container::getLayout() 
	{ return myLayout; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Container::getPadding()
	{ return myPadding; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Container::getMargin() 
	{ return myMargin; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Container::setPadding(float value) 
	{ myPadding = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Container::setMargin(float value) 
	{ myMargin = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Container::HorizontalAlign Container::getHorizontalAlign()
	{ return myHorizontalAlign; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Container::setHorizontalAlign(HorizontalAlign value) 
	{ myHorizontalAlign = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Container::VerticalAlign Container::getVerticalAlign() 
	{ return myVerticalAlign; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Container::setVerticalAlign(VerticalAlign value) 
	{ myVerticalAlign = value; }

};
}; // namespace omega

#endif