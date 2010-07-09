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
#include "outk/ui/Widget.h"

using namespace omega;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Widget::Widget(omega::String name):
	myName(name),
	myParent(NULL),
	myVisible(true),
	myDebugModeColor(Color(1.0f, 0.0f, 1.0f)),
	myDebugModeEnabled(true)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Widget::~Widget()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::addChild(Widget* child)
{
	if(child->myParent != NULL)
	{
		child->myParent->removeChild(child);
	}
	myChildren.push_back(child);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::removeChild(Widget* child)
{
	myChildren.remove(child);
	child->myParent = NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::draw()
{
	if(myDebugModeEnabled)
	{
		float x = myPosition.x();
		float y = myPosition.y();

		float width = mySize.x();
		float height = mySize.y();

		glColor4fv(myDebugModeColor.begin());

		glBegin(GL_LINES);

		glVertex2f(x, y);
		glVertex2f(x + width, y);

		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);

		glVertex2f(x, y);
		glVertex2f(x, y + height);

		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);

		glEnd();
	}

	boost_foreach(Widget* child, myChildren)
	{
		child->draw();
	}
}