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
#include "omegaToolkit/ui/Label.h"
#include "omega/DrawInterface.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Label::Label(ServerEngine* srv):
	Widget(srv),
	myColor(255, 255, 255),
	myVerticalAlign(AlignMiddle),
	myHorizontalAlign(AlignCenter),
	myAutosizeHorizontalPadding(6),
	myAutosizeVerticalPadding(6)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
Label::~Label()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Label::autosize()
{
	// Kindof hack. To compute the label size we need a Font pointer. Fonts are stored inside LabelRenderable
	// objects. Since we expect all those objects to share the same properties, just get the first one
	// and use its font to compute the label size.
	LabelRenderable* lr = (LabelRenderable*)getFirstRenderable();
	Font* font = lr->myFont;

	if(font != NULL)
	{
		Vector2f size = font->computeSize(myText);
		size += Vector2f(myAutosizeHorizontalPadding, myAutosizeVerticalPadding);
		setSize(size);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Label::getFontAlignFlags()
{
	unsigned int alignFlags = 0;
	switch(myHorizontalAlign)
	{
	case AlignRight: alignFlags |= Font::HARight; break;
	case AlignLeft: alignFlags |= Font::HALeft; break;
	case AlignCenter: alignFlags |= Font::HACenter; break;
	}
	switch(myVerticalAlign)
	{
	case AlignTop: alignFlags |= Font::VATop; break;
	case AlignBottom: alignFlags |= Font::VABottom; break;
	case AlignMiddle: alignFlags |= Font::VAMiddle; break;
	}
	return alignFlags;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Label::createRenderable()
{
	return new LabelRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void LabelRenderable::refresh()
{
	myFont = getRenderer()->getFont(myOwner->getFont());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void LabelRenderable::drawContent()
{
	WidgetRenderable::drawContent();

	// If not font has been set, use default ui font.
	if(!myFont) myFont = getRenderer()->getDefaultFont();

	if(myFont)
	{
		unsigned int alignFlags = myOwner->getFontAlignFlags();
		Vector2f textPos = Vector2f::Zero();

		if(alignFlags & Font::HARight) textPos[0] += (float)myOwner->getWidth() - 1;
		else if(alignFlags & Font::HACenter) textPos[0] += (float)myOwner->getWidth() / 2 - 1;

		if(alignFlags & Font::VABottom) textPos[1] += (float)myOwner->getHeight() - 1;
		else if(alignFlags & Font::VAMiddle) textPos[1] += (float)myOwner->getHeight() / 2 - 1;

		glColor4fv(myOwner->myColor.data());

		getRenderer()->drawText(myOwner->myText, myFont, textPos, alignFlags);
	}
}
