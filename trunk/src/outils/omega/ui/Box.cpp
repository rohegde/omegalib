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
#include "omega/ui/Box.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Box::Box(omega::String name, Layout layout):
	Widget(name),
	myLayout(layout),
	myPadding(5)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Box::~Box()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Box::layoutChildren()
{
	if(myVisible)
	{
		float x = myPosition.x();
		float y = myPosition.y();
		float width = mySize.x();
		float height = mySize.y();

		if(myLayout == LayoutHorizontal)
		{
			float cx = x;
			float ix = width / myChildren.size();
			boost_foreach(Widget* child, myChildren)
			{
	            
				child->setPosition(Vector2f(cx, y));
				child->setSize(Vector2f(ix - myPadding, height));
				cx += ix;    
			}
		}
		else if (myLayout == LayoutVertical)
		{
			int cy = y;
			int iy = height / myChildren.size();
			boost_foreach(Widget* child, myChildren)
			{
				child->setPosition(Vector2f(x, cy));
				child->setSize(Vector2f(width, iy - myPadding));
				cy += iy;    
			}
		}
	}
	Widget::layoutChildren();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Box::draw()
{
	Widget::draw();
}
