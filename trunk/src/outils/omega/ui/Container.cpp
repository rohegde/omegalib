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
#include "omega/ui/Container.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Container::Container(omega::String name):
		Widget(name),
		myPadding(5),
		myMargin(5),
		myHorizontalAlign(AlignCenter),
		myVerticalAlign(AlignMiddle)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
Container::~Container()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::addChild(Widget* child)
{
	requestLayoutRefresh();
	myChildren.push_back(child);
	child->setContainer(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::removeChild(Widget* child)
{
	requestLayoutRefresh();
	myChildren.remove(child);
	child->setContainer(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::setUIManager(UIManager* ui)
{
	Widget::setUIManager(ui);
	// set the ui manager for all children.
	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		w->setUIManager(ui);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::draw()
{
	preDraw();

    // draw myself.
	renderContent();

	// draw children.
	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		w->draw();
	}

	postDraw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateSize()
{
	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		w->updateSize();
	}
	Widget::autosize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Container::expandStep(int availableSpace, Orientation orientation)
{
	// Check space constraints for each child
	int childSpace = availableSpace / getNumChildren();
	int spaceLeft = availableSpace;

	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		int min = w->getMinimumSize()[orientation];
		int max = w->getMaximumSize()[orientation];

		// Compute new width
		int ns = w->getSize()[orientation] + childSpace;
		if(ns < min) ns = min;
		if(ns > max) ns = max;

		spaceLeft -= ns;
		w->setActualSize(ns, orientation);
	}
	return spaceLeft;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateChildrenLayoutPosition(Orientation orientation)
{
	int p = myPadding;
	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		w->setPosition(p, orientation);
		p += w->getSize()[orientation] + myMargin;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateChildrenFreeBounds(Orientation orientation)
{
	// Compute the maximum available size
	int available = getSize()[orientation] - myPadding * 2;

	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		int min = w->getMinimumSize()[orientation];
		int max = w->getMaximumSize()[orientation];

		// Compute new width
		int ns = available;
		if(ns < min) ns = min;
		if(ns > max) ns = max;

		// Set child size.
		w->setActualSize(ns, orientation);

		// Set child position (no alignment for now, always centered
		// into parent container.
		int pos = 0;
		if((orientation == Horizontal && myHorizontalAlign == AlignLeft) ||
			(orientation == Vertical && myVerticalAlign == AlignTop))
		{
			pos = 0;
		}
		else if((orientation == Horizontal && myHorizontalAlign == AlignRight) ||
			(orientation == Vertical && myVerticalAlign == AlignBottom))
		{
			pos = getSize(orientation) - w->getSize(orientation) - myPadding;
		}
		else
		{
			pos = (available - ns) / 2 + myPadding;
		}

		w->setPosition(pos, orientation);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::resetChildrenSize(Orientation orientation)
{
	// Initialize widget width to 0
	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())	
	{ 
		Widget* w = it.getNext();	
		w->setActualSize(0, orientation); 
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::computeLinearLayout(Orientation orientation)
{
	int nc = getNumChildren();
	Orientation oppositeOrientation = (orientation == Vertical) ? Horizontal : Vertical;
	// TODO: Check satisfiability of conditions

	// Compute how much total space is available.
	int availableSpace = getSize()[orientation] - myPadding * 2 - (nc - 1) * myMargin;

	resetChildrenSize(orientation);
	while(availableSpace > 0)
	{
		availableSpace = expandStep(availableSpace, orientation) - 1;
	}
	updateChildrenLayoutPosition(orientation);
	updateChildrenFreeBounds(oppositeOrientation);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::layout()
{
	if(needLayoutRefresh())
	{
		if(myLayout == LayoutHorizontal)
		{
			computeLinearLayout(Horizontal);
		}
		else if(myLayout == LayoutVertical)
		{
			computeLinearLayout(Vertical);
		}

		// Layout children.
		WidgetIterator it(myChildren.begin(), myChildren.end());
		while(it.hasMoreElements())
		{
			Widget* w = it.getNext();
			w->layout();
		}

		Widget::layout();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::update(const omega::UpdateContext& context)
{
	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		w->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::processInputEvent(const InputEvent& evt)
{
	myLastEvent = evt;
	bool processed = false;

	Vector2f point = evt.position;
	
	transformPoint(point);

	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		processed |= w->processInputEvent(evt);
	}
	return processed;
}
