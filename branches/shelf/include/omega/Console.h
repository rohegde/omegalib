/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Renderable.h"
#include "DrawInterface.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class Console: public RenderableFactory, public ILogListener
	{
	friend class ConsoleRenderable;
	public:
		Console();
		virtual ~Console();

		virtual Renderable* createRenderable();

		void setFont(const FontInfo& font);

		//! Draw the console. Assume the 
		void draw(const DrawContext& context);

		virtual void addLine(const String& line);

	private:
		int myLines;
		Color myBackgroundColor;
		FontInfo myFont;
		Lock myLock;

		List<String> myLineBuffer;
		String myHeadline;
		Dictionary<char, Color> myConsoleColors;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API ConsoleRenderable: public Renderable
	{
	public:
		ConsoleRenderable(Console* owner): 
		  myOwner(owner), myFont(NULL)
		{}
		void draw(const DrawContext& context);

	private:
		Console* myOwner;
		Font* myFont;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Console::setFont(const FontInfo& font)
	{ myFont = font; }
};
#endif