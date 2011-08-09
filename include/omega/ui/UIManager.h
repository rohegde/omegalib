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
#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "omega/Application.h"
#include "omega/FontManager.h"
#include "omega/Service.h"
#include "omega/ui/Widget.h"

namespace omega
{
	class Renderer;
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward class declarations.
	class Container;
	class WidgetFactory;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API UIManager: public Service
	{
	friend class Widget;
	public:
		UIManager();
		~UIManager();

		void update(const UpdateContext& context);
		void draw(const DrawContext& context);
		bool processInputEvent(const Event& evt);

		Container* getRootContainer(int layer);
		Renderer* getDefaultPainter();

		omega::Font* getDefaultFont();
		void setDefaultFont(Font* value);

		WidgetFactory* getWidgetFactory();
		void setWidgetFactory(WidgetFactory* value);

		IUIEventHandler* getEventHandler(); 
		void setEventHandler(IUIEventHandler* value);

		void registerWidget(Widget* w);
		void unregisterWidget(Widget* w);
		template<typename T> T* getWidgetById(int id);

	private:
		void dispatchEvent(const Event& evt);

	private:
		Container* myRootContainer[Application::MaxLayers];

		IUIEventHandler* myEventHandler;

		Font* myDefaultFont;
		Renderer* myDefaultRenderer;
		WidgetFactory* myWidgetFactory;

		Dictionary<int, Widget*> myWidgetDictionary;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Container* UIManager::getRootContainer(int layer) 
	{ return myRootContainer[layer]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Renderer* UIManager::getDefaultPainter() 
	{ return myDefaultRenderer; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline omega::Font* UIManager::getDefaultFont() 
	{ return myDefaultFont; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void UIManager::setDefaultFont(Font* value) 
	{ myDefaultFont = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline IUIEventHandler* UIManager::getEventHandler() 
	{ return myEventHandler; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void UIManager::setEventHandler(IUIEventHandler* value)
	{ myEventHandler = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline WidgetFactory* UIManager::getWidgetFactory() 
	{ return myWidgetFactory; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void UIManager::setWidgetFactory(WidgetFactory* value) 
	{ myWidgetFactory = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void UIManager::registerWidget(Widget* w)
	{ myWidgetDictionary[w->getId()] = w; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void UIManager::unregisterWidget(Widget* w)
	{ myWidgetDictionary[w->getId()] = NULL; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> inline T* UIManager::getWidgetById(int id)
	{ return (T*)myWidgetDictionary[id]; }
};
}; // namespace omega

#endif