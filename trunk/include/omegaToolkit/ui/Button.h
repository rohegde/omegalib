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
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "omegaToolkit/ui/AbstractButton.h"
#include "omegaToolkit/ui/Label.h"

namespace omegaToolkit { namespace ui {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API Button: public AbstractButton
	{
	public:
		Button(Engine* srv);
		virtual ~Button();

		virtual void handleEvent(const omega::Event& evt);
		virtual void update(const omega::UpdateContext& context);

		omega::String getText() { return myLabel.getText(); }
		void setText(omega::String value) { myLabel.setText(value); }

		// Gets the label subobject used by the button.
		Label* getLabel() { return &myLabel; }

		Color getColor();
		void setColor(Color value);

		virtual void autosize(Renderer* r);

	protected:
		Label myLabel;
		Color myColor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color Button::getColor()
	{
		return myColor;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Button::setColor(Color value)
	{
		myColor = value;
	}

};};
#endif