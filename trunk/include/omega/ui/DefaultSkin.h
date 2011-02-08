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
#ifndef __DEFAULT_SKIN_H__
#define __DEFAULT_SKIN_H__

#include "omega/ui/WidgetFactory.h"

namespace omega
{
namespace ui
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OUTILS_API DefaultButton: public Button
{
public:
	DefaultButton(String name): Button(name) {}

protected:
	void renderContent();

private:
	float myAnim;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OUTILS_API DefaultSlider: public Slider
{
public:
	DefaultSlider(String name): Slider(name) {}
protected:
	void draw();
private:
	float myAnim;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DefaultWidgetFactory: public WidgetFactory
{
public:
	virtual Button* createButton(String name, Container* container)
	{
		Button* button = new DefaultButton(name);
		container->addChild(button);
		return button;
	}

	virtual Slider* createSlider(String name, Container* container)
	{
		Slider* slider = new DefaultSlider(name);
		container->addChild(slider);
		return slider;
	}
};


}; // namespace ui
}; // namespace omega

#endif