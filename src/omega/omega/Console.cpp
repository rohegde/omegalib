/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/Console.h"
#include "omega/Font.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omega/glheaders.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
Console::Console():
	myLines(16),
	myBackgroundColor(Color(0, 0, 0, 0.6f))
{
	myConsoleColors['!'] = Color(0.8f, 0.8f, 0.1f);
	myConsoleColors['*'] = Color(1.0f, 0.2f, 0.1f);
	myConsoleColors['^'] = Color(0.8f, 0.8f, 0.8f);
	myConsoleColors['&'] = Color(0.2f, 1.0f, 0.2f);
	myConsoleColors['@'] = Color(0.8f, 0.7f, 1.0f);

	myHeadline = SystemManager::instance()->getApplication()->getName();

	omsg("Console: adding log listener");
	ologaddlistener(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Console::~Console()
{
	omsg("~Console: removing log listener");
	ologremlistener(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Console::addLine(const String& line)
{
	myLock.lock();
	if(line[0] == '|')
	{
		myHeadline = line.substr(1);
	}
	else
	{
		myLineBuffer.push_back(line);
		while(myLineBuffer.size() > myLines)
		{
			myLineBuffer.pop_front();
		}
	}
	myLock.unlock();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Console::createRenderable()
{
	return new ConsoleRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ConsoleRenderable::draw(const DrawContext& context)
{
	// We assume the transforms and viewport have already been set correctly.

	FontInfo& fi = myOwner->myFont;
	if(myFont == NULL)
	{
		myFont = getRenderer()->createFont(fi.name, fi.filename, fi.size);
	}

	float x = 0; 
	float y = 0;
	float lineHeight = fi.size + 4;
	float lineWidth = fi.size * 100; //SystemManager::instance()->getDisplaySystem()->getCanvasSize().x(); 

	const DisplayTileConfig* tile = context.tile;
	float cx = tile->offset.x();
	float cy = tile->offset.y();
	
	getRenderer()->drawRect(Vector2f(cx, cy), Vector2f(lineWidth, lineHeight * (myOwner->myLines + 1)), myOwner->myBackgroundColor);

	if(myFont != NULL)
	{
		getRenderer()->drawRectOutline(Vector2f(cx - 1, cy), Vector2f(lineWidth + 2, lineHeight - 2), Color::Gray);
		getRenderer()->drawText(myOwner->myHeadline, myFont, Vector2f(cx + x + 2, cy + y + 2), Font::HALeft | Font::VATop, Color(1.0f, 0.9f, 1.0f, 1.0f));
		y += lineHeight;

		foreach(String& s, myOwner->myLineBuffer)
		{
			if(myOwner->myConsoleColors.find(s[0]) != myOwner->myConsoleColors.end())
			{
				getRenderer()->drawText(s.substr(1), myFont, Vector2f(cx + x + 2, cy + y + 2), Font::HALeft | Font::VATop, myOwner->myConsoleColors[s[0]]);
			}
			else
			{
				getRenderer()->drawText(s, myFont, Vector2f(cx + x + 2, cy + y + 2), Font::HALeft | Font::VATop, Color(1, 1, 0, 1));
			}
			y += lineHeight;
		}
	}
}
