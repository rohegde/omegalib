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
#include "oengine/Console.h"
#include "oengine/Font.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace oengine;

OMEGA_DEFINE_TYPE(Console, RenderableFactory);
OMEGA_DEFINE_TYPE(ConsoleRenderable, OverlayRenderable);

///////////////////////////////////////////////////////////////////////////////////////////////////
Console::Console():
	myLines(8),
	myBackgroundColor(Color(0, 0, 0, 0.6f))
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Console::addLine(const String& line)
{
	myLineBuffer.push_back(line);
	while(myLineBuffer.size() > myLines)
	{
		myLineBuffer.pop_front();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Console::createRenderable()
{
	return new ConsoleRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ConsoleRenderable::draw(RenderState* state)
{
	// We assume the transforms and viewport have already been set correctly.

	FontInfo& fi = myOwner->myFont;
	if(myFont == NULL)
	{
		myFont = getRenderer()->createFont(fi.name, fi.filename, fi.size);
	}

	float x = 0; 
	float y = 0;
	float lineHeight = fi.size + 2;
	float lineWidth = state->context->channel->canvasSize->x(); 

	getRenderer()->drawRect(Vector2f(0, 0), Vector2f(lineWidth, lineHeight * myOwner->myLines), myOwner->myBackgroundColor);

	if(myFont != NULL)
	{
		foreach(String& s, myOwner->myLineBuffer)
		{
			glColor4f(1, 1, 1, 1);
			getRenderer()->drawText(s, myFont, Vector2f(x + 2, y + 2), Font::HALeft | Font::VATop);
			y += lineHeight;
		}
	}
}
