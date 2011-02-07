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
#include "omega/ui/DefaultSkin.h"

using namespace omega;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultButton::draw()
{
	Button::draw();

	myAnim *= 0.8f;
	if(myPressed) myAnim = 1.0f;

	//GfxUtils::drawVGradient(myPosition, mySize, Color(80, 80, 100, 100 + myAnim * 100), Color(80, 80, 100, 100 + myAnim * 100));

	glColor4ub(255, 255, 255, 255);
	//GfxUtils::drawDRect(myPosition, mySize, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultSlider::draw()
{
	////float d = 0.5f;
	////if(myPressed) d = 1.0f;

	//Vector2f sliderPos, sliderSize;
	//getSliderCoords(&sliderPos, &sliderSize);

	//Label* lbl = getLabel();
	//if(!getProgressBarMode())
	//{
	//	lbl->setPosition(sliderPos);
	//	lbl->setSize(sliderSize);
	//	lbl->setColor(Color(0, 0, 0,  255));
	//}
	//else
	//{
	//	lbl->setPosition(myPosition + Vector2f(mySize[0], 0));
	//	lbl->setSize(mySize);
	//	lbl->setHorizontalAlign(Label::AlignLeft);
	//	lbl->setColor(Color(255, 255, 255,  255));
	//}

	//myAnim *= 0.8f;
	//if(myPressed) myAnim = 1.0f;

	////GfxUtils::drawVGradient(sliderPos, sliderSize, Color(200, 200, 250, 200 + myAnim * 50 ), Color(200, 200, 250, 200 + myAnim * 50), 0);

	//glColor4ub(255, 255, 255, 255);
	////GfxUtils::drawDRect(myPosition, mySize, 0);

	//Slider::draw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultWidgetFactory::gradientRect(Vector2i pos, Vector2i size, Widget::Orientation orientation, 
	Color startColor, Color endColor, float pc)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	float s = 0;

	glColor4fv(startColor);
	if(orientation == Widget::Horizontal)
	{
		// draw full color portion
		s = int(height * pc);
		glRecti(x, y, x + width, y + s);
		y += s;
		height -= s;
		// draw gradient portion
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + width, y);
		glColor4fv(endColor);
		glVertex2i(x + width, y + height);
		glVertex2i(x, y + height);
		glEnd(); 
	}
	else
	{
		// draw full color portion
		s = int(width * pc);
		glRecti(x, y, x + s, y + height);
		x += s;
		width -= s;
		// draw gradient portion
		glBegin(GL_QUADS);
		glVertex2i(x, y + height);
		glVertex2i(x, y);
		glColor4fv(endColor.begin());
		glVertex2i(x + width, y);
		glVertex2i(x + width, y + height);
		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultWidgetFactory::fillRect(Vector2i pos, Vector2i size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4fv(color);
	glRecti(x, y, x + width, y + height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultWidgetFactory::outlineRect(Vector2i pos, Vector2i size, Color color)
{
	int x = pos[0];
	int y = pos[1];
	int width = size[0];
	int height = size[1];

	glColor4fv(color);

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

