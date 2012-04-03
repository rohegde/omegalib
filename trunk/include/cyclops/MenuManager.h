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
#ifndef __CY_MENU_MANAGER__
#define __CY_MENU_MANAGER__

#include "cyclopsConfig.h"

#include <osg/Texture2D>
#include <osg/Light>
#include <osg/Group>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

#include "omega/PortholeTabletService.h"


namespace cyclops {
	using namespace omega;
	using namespace omegaToolkit;
	using namespace omegaOsg;

	class Menu;
	class MenuManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class IMenuListener
	{
	public:
		virtual void execute() = 0;
		virtual void execute(int value) = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API MenuItem
	{
	public:
		enum Type { Button, Checkbox, Slider, SubMenu };

	public:
		MenuItem(Type type, Menu* owner, MenuItem* parent);

		Type getType() { return myType; }

		void setListener(IMenuListener* value) { myListener = value; }
		IMenuListener* setListener() { return myListener; }

		const String& getText() { return myText; }
		void setText(const String& value) { myText = value; }
		const String& getDescription() { return myDescription; }
		void setDescription(const String& value) { myDescription = value; }

		void setCommand(const String& command);
		String getCommand();
		

		//! Submenu methods.
		//@{
		MenuItem* addItem(MenuItem::Type type);
		//@}

		omegaToolkit::ui::Container* getContainerWidget();

	private:
		Menu* myMenu;
		MenuItem* myParent;
		Type myType;

		UiScriptCommand* myCommand;

		List<MenuItem*> mySubMenuItems;

		IMenuListener* myListener;
		String myText;
		String myDescription;
		int myValue;

		omegaToolkit::ui::Widget* myWidget;
		omegaToolkit::ui::Container* myContainer;
		omegaToolkit::ui::Button* myButton;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API Menu
	{
	public:
		Menu(const String& name, MenuManager* manager);

		MenuManager* getManager() { return myManager; }
		MenuItem* getRoot() { return myRootItem; }

		void show();
		void hide();
		void toggle();
		bool isVisible();

	private:
		MenuManager* myManager;
		MenuItem* myRootItem;
		String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API MenuManager: public ServerModule
	{
	public:
		static MenuManager* instance();
		MenuManager();

		ServerEngine* getEngine() { return myEngine; }
		UiModule* getUiModule() { return myUiModule; }

		virtual void initialize();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt);

		Menu* createMenu(const String& name);
		Menu* getMenu(const String& name);

		void setMainMenu(Menu* menu) { myMainMenu = menu; }
		Menu* getMainMenu() { return myMainMenu; }

	private:
		static MenuManager* mysInstance;

		UiModule* myUiModule;
		Dictionary<String, Menu*> myMenuDictionary;
		Menu* myMainMenu;

		ServerEngine* myEngine;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline MenuManager* MenuManager::instance() 
	{ return mysInstance; }
};

#endif
