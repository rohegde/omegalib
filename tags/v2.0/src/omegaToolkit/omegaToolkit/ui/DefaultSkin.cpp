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
#include "omegaToolkit/ui/DefaultSkin.h"
#include "omega/DrawInterface.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultButtonRenderable::drawContent()
{
	WidgetRenderable::drawContent();

	DrawInterface* painter = getRenderer();

	Color col = Color::White;
	if(myOwner->isActive())
	{
		col = Color::Lime;
	}
	//painter->drawRect(Vector2f::Zero(), myOwner->getSize(), col);
	//painter->drawRectOutline(Vector2f::Zero(), myOwner->getSize(), Color::White);

	Vector2f size = myOwner->getSize();

	// If button is checkable, draw check box.
	if(myOwner->isCheckable())
	{
		size[0] -= 18;
		Vector2f checkBoxSize = Vector2f(14, 14);
		Vector2f checkBoxPosition = Vector2f(size[0], 4);
		painter->drawRectOutline(checkBoxPosition, checkBoxSize, Color::White);

		if(myOwner->isChecked())
		{
			checkBoxSize -= Vector2f(5, 5);
			checkBoxPosition += Vector2f(2, 2);
			painter->drawRect(checkBoxPosition, checkBoxSize, Color::Lime);
		}
	}

	myOwner->getLabel()->setPosition(Vector2f::Zero());
	myOwner->getLabel()->setSize(size);

	myOwner->getLabel()->setColor(col);
	LabelRenderable* lr = (LabelRenderable*)myOwner->getLabel()->getRenderable(getClient());
	if(lr)
	{
		lr->drawContent();
	}

	myAnim *= 0.8f;
	if(myOwner->isPressed()) myAnim = 1.0f;

	//GfxUtils::drawVGradient(myPosition, mySize, Color(80, 80, 100, 100 + myAnim * 100), Color(80, 80, 100, 100 + myAnim * 100));

	//glColor4ub(255, 255, 255, 255);
	//GfxUtils::drawDRect(myPosition, mySize, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultSliderRenderable::drawContent()
{
	WidgetRenderable::drawContent();

	DrawInterface* painter = getRenderer();

	Vector2f sliderPos = myOwner->getSliderPosition();
	Vector2f sliderSize = myOwner->getSliderSize();

	painter->drawRectOutline(Vector2f::Zero(), myOwner->getSize(), Color::White);
	painter->drawRect(sliderPos, sliderSize, Color::Gray);
	painter->drawRectOutline(sliderPos, sliderSize, Color::White);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void DefaultPanelRenderable::drawContent()
{
	ContainerRenderable::drawContent();

	DrawInterface* painter = getRenderer();

	//Vector2f sliderPos = myOwner->getSliderPosition();
	//Vector2f sliderSize = myOwner->getSliderSize();

	Vector2f borderSize = Vector2f(4.0f, 4.0f);
	Vector2f size = myOwner->getSize();

	//painter->drawRectOutline(Vector2f::Zero(), myOwner->getSize(), Color::White);

	painter->drawRect(Vector2f::Zero(), size, Color::White);
	painter->drawRect(borderSize / 2, size - borderSize, Color(0, 0, 0, 0.9f));

	//painter->drawRectOutline(sliderPos, sliderSize, Color::White);
}
