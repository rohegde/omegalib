/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __WIDGET_FACTORY_H__
#define __WIDGET_FACTORY_H__

#include "omega/ui/Button.h"
#include "omega/ui/Container.h"
#include "omega/ui/Slider.h"
#include "omega/ui/Widget.h"
#include "omega/ui/Image.h"

namespace omega
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class WidgetFactory
	{
	public:
		virtual Widget* createWidget(String name, Container* container)
		{
			Widget* widget = onew(Widget)(name);
			container->addChild(widget);
			return widget;
		}

		virtual Button* createButton(String name, Container* container)
		{
			Button* button = onew(Button)(name);
			container->addChild(button);
			return button;
		}

		virtual Button* createCheckButton(String name, Container* container)
		{
			Button* button = createButton(name, container);
			button->setCheckable(true);
			return button;
		}

		virtual Slider* createSlider(String name, Container* container)
		{
			Slider* slider = onew(Slider)(name);
			container->addChild(slider);
			return slider;
		}

		virtual Image* createImage(String name, Container* container)
		{
			Image* image = onew(Image)(name);
			container->addChild(image);
			return image;
		}

		virtual Label* createLabel(String name, Container* container, String text = "")
		{
			Label* lbl = onew(Label)(name);
			if(text.size()  == 0) text = name;
			lbl->setText(text);
			container->addChild(lbl);
			return lbl;
		}

		virtual Container* createContainer(String name, Container* container, 
			Container::Layout layout = Container::LayoutHorizontal)
		{
			Container* c = onew(Container)(name);
			c->setLayout(layout);
			container->addChild(c);
			return c;
		}
	};
}; // namespace ui
}; // namespace omega

#endif