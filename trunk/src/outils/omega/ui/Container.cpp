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
		myVerticalAlign(AlignMiddle),
		myGridRows(1),
		myGridColumns(1)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
Container::~Container()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::load(Setting& setting)
{
	if(setting.exists("layout"))
	{
		String layout = setting["layout"];
		if(layout == "LayoutFree") myLayout = LayoutFree;
		if(layout == "LayoutVertical") myLayout = LayoutVertical;
		if(layout == "LayoutHorizontal") myLayout = LayoutHorizontal;
		if(layout == "LayoutGridHorizontal") myLayout = LayoutGridHorizontal;
		if(layout == "LayoutGridVertical") myLayout = LayoutGridVertical;
	}
	if(setting.exists("position"))
	{
		String position = setting["position"];
		setPosition(Vector2i(position[0], position[1]));
	}
	if(setting.exists("horizontalAlign"))
	{
		String align = setting["horizontalAlign"];
		if(align == "AlignLeft") myHorizontalAlign = AlignLeft;
		if(align == "AlignCenter") myHorizontalAlign = AlignCenter;
		if(align == "AlignRight") myHorizontalAlign = AlignRight;
	}
	if(setting.exists("verticalAlign"))
	{
		String align = setting["verticalAlign"];
		if(align == "AlignTop") myVerticalAlign = AlignTop;
		if(align == "AlignBottom") myVerticalAlign = AlignBottom;
		if(align == "AlignMiddle") myVerticalAlign = AlignMiddle;
	}
	if(setting.exists("size"))
	{
		Setting& size = setting["size"];
		setSize(Vector2i(size[0], size[1]));
	}
	if(setting.exists("width"))
	{
		setWidth((int)setting["width"]);
	}
	if(setting.exists("height"))
	{
		setHeight((int)setting["height"]);
	}
	if(setting.exists("padding"))
	{
		myPadding = setting["padding"];
	}
	if(setting.exists("margin"))
	{
		myMargin = setting["margin"];
	}
	if(setting.exists("gridRows"))
	{
		myGridRows = setting["gridRows"];
	}
	if(setting.exists("gridColumns"))
	{
		myGridRows = setting["gridColumns"];
	}
	if(setting.exists("debugMode"))
	{
		myDebugModeEnabled = setting["debugMode"];
	}
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
		int size = w->getSize()[orientation] + childSpace;
		w->setActualSize(size, orientation);
		spaceLeft -= w->getSize(orientation);
	}
	return spaceLeft;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateChildrenLayoutPosition(Orientation orientation)
{
	int p = 0;

	if((orientation == Horizontal && myHorizontalAlign == AlignLeft) ||
		(orientation == Vertical && myVerticalAlign == AlignTop))
	{
		p = myPadding;
	}
	else if((orientation == Horizontal && myHorizontalAlign == AlignRight) ||
		(orientation == Vertical && myVerticalAlign == AlignBottom))
	{
		p = getSize(orientation) - myPadding;
		WidgetIterator it(myChildren.begin(), myChildren.end());
		while(it.hasMoreElements())
		{
			Widget* w = it.getNext();
			p -= (w->getSize(orientation) + myMargin);
		}
	}
	else
	{
		// TODO: Compute center align start position
		p = myPadding;
	}
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

		// Set child size.
		w->setActualSize(available, orientation);

		// Set child position
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
			pos = (available - w->getSize(orientation)) / 2 + myPadding;
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
void Container::computeGridLayout(Orientation orientation)
{
	int nc = getNumChildren();
	Orientation oppositeOrientation = (orientation == Vertical) ? Horizontal : Vertical;
	// TODO: Check satisfiability of conditions

	// Compute how much total space is available.
	int availableWidth = getSize()[Horizontal] - myPadding * 2 - (myGridColumns - 1) * myMargin;
	int availableHeight = getSize()[Vertical] - myPadding * 2 - (myGridRows - 1) * myMargin;

	int cellWidth = availableWidth / myGridColumns;
	int cellHeight = availableHeight / myGridRows;

	int cellX = myPadding;
	int cellY = myPadding;

	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		w->setActualSize(cellWidth, Horizontal);
		w->setActualSize(cellHeight, Vertical);

		w->setPosition(Vector2f(cellX, cellY));

		// Compute next cell position.
		if(orientation == Horizontal)
		{
			cellX += cellWidth + myMargin;
			if(cellX > availableWidth)
			{
				cellX = myPadding;
				cellY += cellHeight + myMargin;
			}
		}
		else
		{
			cellY += cellHeight + myMargin;
			if(cellY > availableHeight)
			{
				cellY = myPadding;
				cellX += cellWidth + myMargin;
			}
		}
	}
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
		else if(myLayout == LayoutGridHorizontal)
		{
			computeGridLayout(Horizontal);
		}
		if(myLayout == LayoutGridVertical)
		{
			computeGridLayout(Vertical);
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
bool Container::processInputEvent(const Event& evt)
{
	myLastEvent = evt;
	bool processed = false;

	Vector2f point = Vector2f(evt.position[0], evt.position[1]);
	
	transformPoint(point);

	WidgetIterator it(myChildren.begin(), myChildren.end());
	while(it.hasMoreElements())
	{
		Widget* w = it.getNext();
		processed |= w->processInputEvent(evt);
	}
	return processed;
}
