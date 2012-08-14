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
#ifndef __OTK_MENU_MANAGER__
#define __OTK_MENU_MANAGER__

#include "omegaToolkit/omegaToolkitConfig.h"
#include "omegaToolkit/ui/Widget.h"
#include "omegaToolkit/ui/Button.h"
#include "omegaToolkit/ui/Container.h"
#include "omegaToolkit/UiScriptCommand.h"

namespace omegaToolkit { namespace ui {
	using namespace omega;

	class Menu;
	class MenuItem;
	class MenuManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class IMenuItemListener
	{
	public:
		virtual void onMenuItemEvent(MenuItem* mi) = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API MenuItem: public ReferenceType, public IEventListener
	{
	friend class Menu;
	public:
		enum Type { Button, Checkbox, Slider, SubMenu };

	public:
		MenuItem(Type type, Menu* owner);

		virtual void handleEvent(const Event& evt);
		Type getType() { return myType; }

		void setListener(IMenuItemListener* value);
		IMenuItemListener* getListener() { return myListener; }

		const String& getText() { return myText; }
		void setText(const String& value);
		const String& getDescription() { return myDescription; }
		void setDescription(const String& value);

		void setCommand(const String& command);
		String getCommand();

		//! Checkbox methods
		//@{
		void setChecked(bool value);
		bool isChecked();
		//@}

		//! User data management
		//@{
		void setUserTag(const String& value) { myUserTag = value; }
		String getUserTag() { return myUserTag; }
		void* getUserData() { return myUserData; }
		void setUserData(void* value) { myUserData = value; }
		//@}

		//! Submenu methods.
		//@{
		MenuItem* addItem(MenuItem::Type type);
		//@}

		//omegaToolkit::ui::Container* getContainerWidget();

		omegaToolkit::ui::Widget* getWidget() { return myWidget; }

		Menu* getSubMenu() { return mySubMenu; }

	private:
		Menu* myMenu;
		MenuItem* myParent;
		Type myType;

		Ref<UiScriptCommand> myCommand;
		IMenuItemListener* myListener;


		String myText;
		String myDescription;
		int myValue;

		String myUserTag;
		void* myUserData;
		Menu* mySubMenu;

		omegaToolkit::ui::Widget* myWidget;
		omegaToolkit::ui::Button* myButton;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API Menu: public ReferenceType
	{
	friend class MenuItem;
	friend class MenuManager;
	public:
		MenuManager* getManager() { return myManager; }

		MenuItem* addItem(MenuItem::Type type);

		void focus();
		void show();
		void hide();
		void toggle();
		bool isVisible();

		void setActiveSubMenu(Menu* submenu);
		Menu* getTopActiveSubMenu();
		Menu* getParent() { return myParent; }

		void update(const UpdateContext& context);

		omegaToolkit::ui::Container* getContainer() { return myContainer; }
		omegaToolkit::ui::Container3dSettings& get3dSettings() { return my3dSettings; }

	private:
		Menu(const String& name, MenuManager* manager);
		void onPopMenuStack();
		void onPushMenuStack();

	private:
		MenuManager* myManager;
		MenuItem* myRootItem;
		String myName;

		// Menu placement
		Vector3f menuPosition;

		List<MenuItem*> myMenuItems;
		Menu* myActiveSubMenu;
		Menu* myParent;

		bool myVisible;

		omegaToolkit::ui::Container* myContainer;
		omegaToolkit::ui::Container3dSettings my3dSettings;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API MenuManager: public EngineModule
	{
	public:
		static MenuManager* createAndInitialize();
		static MenuManager* instance();

		UiModule* getUiModule() { return myUiModule; }

		virtual void initialize();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt);

		Menu* createMenu(const String& name);

		void setMainMenu(Menu* menu) { myMainMenu = menu; }
		Menu* getMainMenu() { return myMainMenu; }

		void autoPlaceMenu(Menu* menu, const Event& evt);

		float getDefaultMenuScale() { return myDefaultMenuScale; }

		// Specifies whether camera navigation should be disabled when inside a menu
		void setNavigationSuspended(bool value) { myNavigationSuspended = value; }
		//! see setNavigationSuspended
		bool getNavigationSuspended() { return myNavigationSuspended; }

	private:
		MenuManager();

		//void autoPlaceMenu(Menu* menu, const Event& evt);

	private:
		static MenuManager* mysInstance;

		UiModule* myUiModule;
		List<Menu*> myMenuList;
		Menu* myMainMenu;

		bool myRayPlaceEnabled;
		Vector3f myDefaultMenuPosition;
		float myDefaultMenuScale;

		// options
		bool myNavigationSuspended;
		bool myNavigationState;
		bool myUseMenuToggleButton;
		Event::Flags myMenuToggleButton;

		//bool myMenu3dEnabled;
		//bool myAutoPlaceEnabled;
		//float myAutoPlaceDistance;
		//float myMenu3dScale;
	};
}; };

#endif
