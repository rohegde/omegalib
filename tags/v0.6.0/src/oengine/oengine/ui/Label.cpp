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
#include "oengine/ui/Label.h"
#include "oengine/ui/UiManager.h"
#include "oengine/Renderer.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;
using namespace oengine::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Label::Label(UiManager* mng):
	Widget(mng),
	myFont(NULL),
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
	// If not font has been set, use default ui font.
	if(!myFont) myFont = getManager()->getDefaultFont();
	if(myFont)
	{
		Vector2f size = myFont->computeSize(myText);
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
void Label::renderContent()
{
	Widget::renderContent();

	// If not font has been set, use default ui font.
	if(!myFont) myFont = getManager()->getDefaultFont();

	if(myFont)
	{
		unsigned int alignFlags = getFontAlignFlags();
		Vector2f textPos = Vector2f::Zero();

		if(alignFlags & Font::HARight) textPos[0] += (float)getWidth() - 1;
		else if(alignFlags & Font::HACenter) textPos[0] += (float)getWidth() / 2 - 1;

		if(alignFlags & Font::VABottom) textPos[1] += (float)getHeight() - 1;
		else if(alignFlags & Font::VAMiddle) textPos[1] += (float)getHeight() / 2 - 1;

		glColor4fv(myColor.data());

		getRenderer()->drawText(myText, myFont, textPos, alignFlags);
	}
}
