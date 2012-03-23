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

#include "FTGL/ftgl.h"

using namespace omega;
using namespace outk::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Widget::Widget(omega::String name):
	myUI(NULL),
	myName(name),
	myParent(NULL),
	myVisible(true),
	myDebugModeColor(255, 0, 255),
	myDebugModeEnabled(false),
	myRotation(0),
	//myScale(1.0f),
	myUserMoveEnabled(false),
	myMoving(false)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Widget::~Widget()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::setUI(UIManager* ui)
{
	myUI = ui;
	boost_foreach(Widget* child, myChildren)
	{
		child->setUI(ui);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::addChild(Widget* child)
{
	if(child->myParent != NULL)
	{
		child->myParent->removeChild(child);
	}
	myChildren.push_back(child);
	child->myParent = this;
	child->setUI(myUI);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::removeChild(Widget* child)
{
	myChildren.remove(child);
	child->myParent = NULL;
	child->setUI(NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::render()
{
	glPushMatrix();

	// Setup transformation.
	Vector2f center = myPosition + (mySize / 2);
	Vector2f mcenter = -center;

	glTranslatef(center[0], center[1], 0.0f);

	glRotatef(myRotation, 0, 0, 1);

	glTranslatef(mcenter[0], mcenter[1], 0.0f);

    // draw myself.
	draw();

	// render children.
	boost_foreach(Widget* child, myChildren)
	{
		child->render();
	}

	// reset transform.
	glPopMatrix();
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

		glColor4ubv(myDebugModeColor.begin());

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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::layoutChildren()
{
	boost_foreach(Widget* child, myChildren)
	{
		child->layoutChildren();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::update(const omega::UpdateContext& context)
{
	boost_foreach(Widget* child, myChildren)
	{
		child->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::hitTest(const Vector2f& point)
{
	float x1 = myPosition[0];
	float y1 = myPosition[1];
	float x2 = myPosition[0] + mySize[0];
	float y2 = myPosition[1] + mySize[1];

	if(point[0] >= x1 && point[1] >= y1 && point[0] < x2 && point[1] < y2) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::hitTest(const Vector2f& point, const Vector2f& pos, const Vector2f& size)
{
	float x1 = pos[0];
	float y1 = pos[1];
	float x2 = pos[0] + size[0];
	float y2 = pos[1] + size[1];

	if(point[0] >= x1 && point[1] >= y1 && point[0] < x2 && point[1] < y2) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::transformPoint(Vector2f& point)
{
	if(myParent)
	{
		myParent->transformPoint(point);
	}

	if(myRotation != 0)
	{
		Vector2f center = myPosition + (mySize / 2);
		point -= center;
		float s = sin(-myRotation * omega::Math::DegToRad);
		float c = cos(-myRotation * omega::Math::DegToRad);
		float x = point[0];
		float y = point[1];
		point[0] = x * c - y * s;;
		point[1] = y * c + x * s;
		point += center;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Widget::processInputEvent(const InputEvent& evt)
{
	myLastEvent = evt;
	bool processed = false;

	Vector2f point = evt.position;
	
	transformPoint(point);

	boost_foreach(Widget* child, myChildren)
	{
		processed |= child->processInputEvent(evt);
	}
	if(hitTest(point))
	{
		if(!processed)
		{
			if(myUserMoveEnabled)
			{
				if(evt.type == InputEvent::Down)
				{
					myUserMovePosition = evt.position;
					processed = true;
					myMoving = true;
				}
				else if(evt.type == InputEvent::Move && myMoving)
				{
					Vector2f cur = evt.position;
					Vector2f delta = cur - myUserMovePosition;
					myUserMovePosition = cur;

					myPosition += delta;
					processed = true;
				}
				else if(myMoving && evt.type == InputEvent::Up)
				{
					myMoving = false;
					processed = true;
				}
				if(evt.type == InputEvent::Rotate)
				{
					myRotation += evt.rotation[0];
					processed = true;
				}
			}
		}
	}
	return processed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::dispatchUIEvent(UIEvent& evt)
{
	if(myParent) myParent->dispatchUIEvent(evt);
	else myUI->dispatchUIEvent(evt);
}