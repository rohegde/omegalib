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
		const String& getCommand();

		//! Checkbox methods
		//@{
		void setChecked(bool value);
		bool isChecked();
		//@}

		//! User data management
		//@{
		void setUserTag(const String& value) { myUserTag = value; }
		const String& getUserTag() { return myUserTag; }
		void* getUserData() { return myUserData; }
		void setUserData(void* value) { myUserData = value; }
		//@}

		//! Submenu methods.
		//@{
		//MenuItem* addItem(MenuItem::Type type);
		//@}

		//omegaToolkit::ui::Container* getContainerWidget();

		omegaToolkit::ui::Widget* getWidget() { return myWidget; }

		Menu* getSubMenu() { return mySubMenu; }

	private:
		// Weak reference to parent menu
		Menu* myMenu;

		Ref<MenuItem> myParent;
		Type myType;

		Ref<UiScriptCommand> myCommand;
		IMenuItemListener* myListener;


		String myText;
		String myDescription;
		int myValue;

		String myUserTag;
		void* myUserData;
		Ref<Menu> mySubMenu;

		Ref<omegaToolkit::ui::Widget> myWidget;
		Ref<omegaToolkit::ui::Button> myButton;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API Menu: public ReferenceType
	{
	friend class MenuItem;
	friend class MenuManager;
	public:
		MenuManager* getManager() { return myManager; }

		MenuItem* addItem(MenuItem::Type type);
		//! Utility method to create a button
		MenuItem* addButton(const String& label, const String& command);
		//! Utility method to create a sub-menu
		Menu* addSubMenu(const String& label);

		void focus();
		void show();
		void hide();
		void toggle();
		bool isVisible();

		void setActiveSubMenu(Menu* submenu);
		Menu* getTopActiveSubMenu();
		Menu* getParent() { return myParent; }

		void update(const UpdateContext& context);

		void placeOnWand(const Event& evt);
		
		omegaToolkit::ui::Container* getContainer() { return myContainer; }
		omegaToolkit::ui::Container3dSettings& get3dSettings() { return my3dSettings; }

		void setLabel(const String& label);
		String getLabel();

	private:
		Menu(const String& name, MenuManager* manager);
		void onPopMenuStack();
		void onPushMenuStack();

	private:
		Ref<MenuManager> myManager;
		Ref<MenuItem> myRootItem;
		String myName;

		// Menu placement
		Vector3f menuPosition;

		List< Ref<MenuItem> > myMenuItems;
		Ref<Menu> myActiveSubMenu;
		Ref<Menu> myParent;

		bool myVisible;

		Ref<omegaToolkit::ui::Label> myLabelWidget;
		Ref<omegaToolkit::ui::Container> myContainer;
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
		virtual void dispose();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt);

		Menu* createMenu(const String& name);

		void setMainMenu(Menu* menu) { myMainMenu = menu; }
		Menu* getMainMenu() { return myMainMenu; }

		void autoPlaceMenu(Menu* menu, const Event& evt);

		float getDefaultMenuScale() { return myDefaultMenuScale; }
		float getDefaultMenuDistance() { return myDefaultMenuPosition.z(); }

		// Specifies whether camera navigation should be disabled when inside a menu
		void setNavigationSuspended(bool value) { myNavigationSuspended = value; }
		//! see setNavigationSuspended
		bool getNavigationSuspended() { return myNavigationSuspended; }

		bool is3dMenuEnabled() { return my3dMenuEnabled; }
		
		Sound* getShowMenuSound() { return myShowMenuSound; }
		Sound* getHideMenuSound() { return myHideSoundMenu; }
	private:
		MenuManager();
		virtual ~MenuManager();

		//void autoPlaceMenu(Menu* menu, const Event& evt);

	private:
		static MenuManager* mysInstance;

		Ref<UiModule> myUiModule;
		List< Ref<Menu> > myMenuList;
		Ref<Menu> myMainMenu;

		bool myRayPlaceEnabled;
		Vector3f myDefaultMenuPosition;
		float myDefaultMenuScale;

		// options
		bool myNavigationSuspended;
		bool myNavigationState;
		bool myUseMenuToggleButton;
		Event::Flags myMenuToggleButton;

		bool my3dMenuEnabled;
		//bool myAutoPlaceEnabled;
		//float myAutoPlaceDistance;
		//float myMenu3dScale;

		// Menu sounds
		Sound* myShowMenuSound;
		Sound* myHideSoundMenu;
		Sound* selectMenuSound;
		Sound* scrollMenuSound;
	};
}; };

#endif
