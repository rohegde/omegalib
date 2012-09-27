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
#ifndef __BAREBONE_SKIN_H__
#define __BAREBONE_SKIN_H__

#include "omegaToolkit/ui/WidgetFactory.h"

namespace omegaToolkit { namespace ui {
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API BareboneButtonRenderable: public WidgetRenderable
	{
	public:
		BareboneButtonRenderable(Button* owner): WidgetRenderable(owner), myOwner(owner) {}
		virtual ~BareboneButtonRenderable() {}
	protected:
		void drawContent();
	private:
		Button* myOwner;
		float myAnim;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API BareboneSliderRenderable: public WidgetRenderable
	{
	public:
		BareboneSliderRenderable(Slider* owner): WidgetRenderable(owner), myOwner(owner) {}
		virtual ~BareboneSliderRenderable() {}
	protected:
		void drawContent();
	private:
		Slider* myOwner;
		float myAnim;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class BareboneButton: public Button
	{
	friend class BareboneButtonRenderable;
	public:
		BareboneButton(Engine* srv): Button(srv) {}
		virtual ~BareboneButton() {}
		virtual Renderable* createRenderable()  { return new BareboneButtonRenderable(this); }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class BareboneSlider: public Slider
	{
	friend class BareboneSliderRenderable;
	public:
		BareboneSlider(Engine* srv): Slider(srv) {}
		virtual ~BareboneSlider() {}
		virtual Renderable* createRenderable() { return new BareboneSliderRenderable(this); }
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class BareboneWidgetFactory: public WidgetFactory
	{
	public:
		BareboneWidgetFactory(Engine* srv): WidgetFactory(srv) {}

		virtual Button* createButton(const String& name, Container* container)
		{
			Button* button = new BareboneButton(getEngine());
			button->setName(name);
			button->getLabel()->setText(name);
			container->addChild(button);
			return button;
		}

		virtual Slider* createSlider(const String& name, Container* container)
		{
			Slider* slider = new BareboneSlider(getEngine());
			container->addChild(slider);
			return slider;
		}
	};
};};
#endif