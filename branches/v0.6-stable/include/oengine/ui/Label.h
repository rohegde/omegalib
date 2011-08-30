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
#ifndef __LABEL_H__
#define __LABEL_H__

#include "oengine/FontManager.h"
#include "oengine/ui/Widget.h"

namespace oengine { namespace ui {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API Label: public Widget
	{
	public:
		enum HorizontalAlign { AlignRight, AlignLeft, AlignCenter};
		enum VerticalAlign { AlignTop, AlignMiddle, AlignBottom};

	public:
		Label(UiManager* mng);
		virtual ~Label();

		String getText();
		void setText(const String& value);

		Font* getFont();
		void setFont(Font* value);

		Color getColor();
		void setColor(const Color& value);

		HorizontalAlign getHorizontalAlign();
		void setHorizontalAlign(HorizontalAlign value);

		VerticalAlign getVerticalAlign();
		void setVerticalAlign(VerticalAlign value);

		void setAutosizeVerticalPadding(int value);
		void setAutosizeHorizontalPadding(int value);
		int getAutosizeVerticalPadding();
		int getAutosizeHorizontalPadding();
		void setAutosizePadding(int value);

		virtual void autosize();

	protected:
		unsigned int getFontAlignFlags();
		virtual void renderContent();

	protected:
		String myText;
		Font* myFont;
		Color myColor;

		HorizontalAlign myHorizontalAlign;
		VerticalAlign myVerticalAlign;

	private:
        static NameGenerator mysNameGenerator;

		int myAutosizeHorizontalPadding;
		int myAutosizeVerticalPadding;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline String Label::getText() 
	{ return myText; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setText(const String& value) 
	{ myText = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Font* Label::getFont() 
	{ return myFont; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setFont(Font* value)
	{ myFont = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color Label::getColor()
	{ return myColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setColor(const Color& value)
	{ myColor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Label::HorizontalAlign Label::getHorizontalAlign()
	{ return myHorizontalAlign; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setHorizontalAlign(HorizontalAlign value) 
	{ myHorizontalAlign = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Label::VerticalAlign Label::getVerticalAlign() 
	{ return myVerticalAlign; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setVerticalAlign(VerticalAlign value) 
	{ myVerticalAlign = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setAutosizeVerticalPadding(int value) 
	{ myAutosizeVerticalPadding = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setAutosizeHorizontalPadding(int value) 
	{ myAutosizeHorizontalPadding = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Label::getAutosizeVerticalPadding() 
	{ return myAutosizeVerticalPadding; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Label::getAutosizeHorizontalPadding() 
	{ return myAutosizeHorizontalPadding; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Label::setAutosizePadding(int value)
	{
		myAutosizeHorizontalPadding = value;
		myAutosizeVerticalPadding = value;
	}

}; };
#endif