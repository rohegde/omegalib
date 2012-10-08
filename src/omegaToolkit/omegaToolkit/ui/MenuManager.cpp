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
#include "omegaToolkit/ui/MenuManager.h"
#include "omega/DisplaySystem.h"

using namespace omegaToolkit::ui;

MenuManager* MenuManager::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuItem::MenuItem(Type type, Menu* owner):
	myMenu(owner),
	myType(type),
	myListener(NULL),
	myButton(NULL),
	myCommand(NULL),
	myWidget(NULL),
	myUserData(NULL),
	mySubMenu(NULL)
{
	UiModule* ui = owner->getManager()->getUiModule();
	WidgetFactory* wf = ui->getWidgetFactory();

	if(type == MenuItem::SubMenu)
	{
		myButton = wf->createButton("subMenu_button", myMenu->myContainer);
		myButton->setText("Button");
		myWidget = myButton;
		myWidget->setUIEventHandler(this);

		mySubMenu = myMenu->getManager()->createMenu("Submenu");
	}
	else if(type == MenuItem::Button)
	{
		myButton = wf->createButton("button", myMenu->myContainer);
		myButton->setText("Button");
		myWidget = myButton;
	}
	else if(type == MenuItem::Checkbox)
	{
		myButton = wf->createButton("button", myMenu->myContainer);
		myButton->setText("Checkbox");
		myButton->setCheckable(true);
		myWidget = myButton;
	}

	myWidget->setAutosize(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuItem::setText(const String& value) 
{ 
	myText = value; 
	switch(myType)
	{
	case MenuItem::Button:
	case MenuItem::Checkbox:
		myButton->setText(myText);
		break;
	case MenuItem::SubMenu:
		myButton->setText(myText + "  >");
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuItem::setDescription(const String& value) 
{ 
	myDescription = value; 
	switch(myType)
	{
	case MenuItem::Button:
	case MenuItem::Checkbox:
		// Do nothing.
		break;
	}
}
		
///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuItem::setChecked(bool value)
{
	if(myType == MenuItem::Checkbox)
	{
		myButton->setChecked(value);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MenuItem::isChecked()
{
	if(myType == MenuItem::Checkbox)
	{
		return myButton->isChecked();
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuItem::setCommand(const String& command)
{
	if(myCommand == NULL)
	{
		myCommand = new UiScriptCommand(command);
		myWidget->setUIEventHandler(myCommand.get());
	}
	else
	{
		myCommand->setCommand(command);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const String& MenuItem::getCommand()
{
	static String emptyString("");

	if(myCommand != NULL)
	{
		return myCommand->getCommand();
	}
	return emptyString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuItem::setListener(IMenuItemListener* value) 
{
	myListener = value; 
	if(myListener != NULL)
	{
		myWidget->setUIEventHandler(this);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuItem::handleEvent(const Event& evt)
{
	if(evt.isFrom(Service::Ui, myWidget->getId()))
	{
		if(myListener != NULL) myListener->onMenuItemEvent(this);
		if(myType == SubMenu && evt.getType() == Event::Click)
		{
			myMenu->setActiveSubMenu(mySubMenu);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu::Menu(const String& name, MenuManager* manager): 
	myName(name),
	myManager(manager),
	myActiveSubMenu(NULL),
	myParent(NULL)
{
	UiModule* ui = UiModule::instance();
	WidgetFactory* wf = ui->getWidgetFactory();
	myContainer = wf->createPanel("container", ui->getUi());
	myContainer->setPosition(Vector2f(10, 10));

	myLabelWidget = wf->createLabel("menuLabel", myContainer, "Menu");
	myLabelWidget->setStyle("border-bottom: 2 white");

	my3dSettings.enable3d = MenuManager::instance()->is3dMenuEnabled();
	myContainer->setAutosize(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::setLabel(const String& label)
{
	myLabelWidget->setText(label);
	if(label != "") myLabelWidget->setVisible(true);
	else myLabelWidget->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
String Menu::getLabel() 
{ 
	return 	myLabelWidget->getText();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuItem* Menu::addItem(MenuItem::Type type)
{
	MenuItem* item = new MenuItem(type, this);
	myMenuItems.push_back(item);
	return item;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuItem* Menu::addButton(const String& label, const String& command)
{
	MenuItem* item = addItem(MenuItem::Button);
	item->setText(label);
	item->setCommand(command);
	return item;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu* Menu::addSubMenu(const String& label)
{
	MenuItem* item = addItem(MenuItem::SubMenu);
	item->setText(label);
	return item->getSubMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::update(const UpdateContext& context)
{
	float speed = context.dt * 10;

	ui::Container3dSettings& c3ds = myContainer->get3dSettings();
	c3ds.enable3d = my3dSettings.enable3d;
	c3ds.normal = my3dSettings.normal;
	c3ds.up = my3dSettings.up;
	c3ds.position += (my3dSettings.position - c3ds.position) * speed;
	c3ds.scale += (my3dSettings.scale - c3ds.scale) * speed;
	//c3ds.position = my3dSettings.position;
	//c3ds.scale = my3dSettings.scale;
	c3ds.alpha += (my3dSettings.alpha - c3ds.alpha) * speed;
	//c3ds.alpha = my3dSettings.alpha;

	if(myContainer->isVisible())
	{
		if(c3ds.alpha <= 0.1f)
		{
			myContainer->setVisible(false);
		}
	}
	else
	{
		if(c3ds.alpha > 0.1f)
		{
			myContainer->setVisible(true);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::setActiveSubMenu(Menu* submenu)
{
	if(submenu == NULL)
	{
		// Hide current active submenu
		if(myActiveSubMenu != NULL)
		{
			onPopMenuStack();
			myActiveSubMenu->hide();
			myActiveSubMenu->myParent = NULL;
		}
		myActiveSubMenu = NULL;
		myContainer->setEnabled(true);
	}
	else
	{
		submenu->get3dSettings() = my3dSettings;
		submenu->show();

		// Hide current active submenu
		if(myActiveSubMenu != NULL)
		{
			myActiveSubMenu->hide();
			myActiveSubMenu->myParent = NULL;
			myActiveSubMenu = submenu;
			myActiveSubMenu->myParent = this;
		}
		else
		{
			myActiveSubMenu = submenu;
			myActiveSubMenu->myParent = this;
			onPushMenuStack();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::onPopMenuStack()
{
	if(myActiveSubMenu != NULL)
	{
		if(myParent != NULL)
		{
			myParent->onPopMenuStack();
		}
		my3dSettings = myActiveSubMenu->get3dSettings();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::onPushMenuStack()
{
	if(myActiveSubMenu != NULL)
	{
		// I have an active submenu: I am disabled.
		myContainer->setEnabled(false);

		if(myParent != NULL)
		{
			myParent->onPushMenuStack();
		}

		const Container3dSettings& subc3ds = myActiveSubMenu->get3dSettings();

		my3dSettings.position += subc3ds.normal * (-0.3f);
		my3dSettings.alpha = myActiveSubMenu->get3dSettings().alpha / 3;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu* Menu::getTopActiveSubMenu()
{
	if(myActiveSubMenu != NULL) return myActiveSubMenu->getTopActiveSubMenu();
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::focus()
{
	UiModule::instance()->activateWidget(myContainer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::show()
{
	myVisible = true;
	myContainer->setEnabled(true);

	focus();

	myContainer->get3dSettings().alpha = 0.0f;
	my3dSettings.alpha = 1.0f;

	myContainer->get3dSettings().scale = myManager->getDefaultMenuScale() / 2;
	my3dSettings.scale = myManager->getDefaultMenuScale();

	if(SystemManager::settingExists("config/sound"))
	{
		// Set the listener position
		Vector3f cameraPosition = myManager->getEngine()->getDefaultCamera()->getPosition();
		myManager->getEngine()->getSoundManager()->setListenerPosition( cameraPosition );

		// Set the sound position and play
		SoundInstance* showSound = new SoundInstance(myManager->getShowMenuSound());
		ofmsg("Menu::show() about to play sound %1%", %myManager->getHideMenuSound()->getFilePath() );
		ofmsg("Menu pos: %1%", %myContainer->get3dSettings().position );
		showSound->setPosition( myContainer->get3dSettings().position );
		showSound->setVolume(1.0);
		showSound->play();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::hide()
{
	//omsg("Menu hide");

	myVisible = false;
	myContainer->setEnabled(false);

	UiModule::instance()->activateWidget(NULL);

	my3dSettings.alpha = 0.0f;
	my3dSettings.scale = myManager->getDefaultMenuScale() / 2;

	if(myActiveSubMenu != NULL)
	{
		myActiveSubMenu->hide();
		myActiveSubMenu = NULL;
	}

	if(SystemManager::settingExists("config/sound"))
	{
		// Set the listener position
		Vector3f cameraPosition = myManager->getEngine()->getDefaultCamera()->getPosition();
		myManager->getEngine()->getSoundManager()->setListenerPosition( cameraPosition );

		SoundInstance* hideSound = new SoundInstance(myManager->getHideMenuSound());
		ofmsg("Menu::hide() about to play sound %1%", %myManager->getHideMenuSound()->getFilePath() );
		ofmsg("Menu pos: %1%", %myContainer->get3dSettings().position );
		ofmsg("Listener pos: %1%", %myManager->getEngine()->getSoundManager()->getListenerPosition() );
		hideSound->play( myContainer->get3dSettings().position, 1.0f, 2.0f, 1.0f, 1.0f, false);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::placeOnWand(const Event& evt)
{
	MenuManager* mm = MenuManager::instance();
	float distance = mm->getDefaultMenuDistance();
	float scale = mm->getDefaultMenuScale();
	Ray ray;
	if(SystemManager::instance()->getDisplaySystem()->getViewRayFromEvent(evt, ray))
	{
		Vector3f pos = ray.getPoint(distance);
		//ofmsg("menu position: %1%", %pos);
		Container3dSettings& c3ds = get3dSettings();
		Widget* menuWidget = myContainer;
		Vector3f offset = Vector3f(0, menuWidget->getHeight() * c3ds.scale, 0);
		c3ds.position = pos - offset;
		c3ds.normal = -ray.getDirection();
		
		// Find up vector.
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		Camera* cam = Engine::instance()->getDefaultCamera();			
		Vector3f up = Vector3f::UnitY();
		c3ds.up = cam->getOrientation() * up;
		
		//c3ds.normal = Vector3f(0, 0, 1);
		c3ds.scale = scale;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::toggle()
{
	if(myContainer->isVisible()) hide();
	else show();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Menu::isVisible()
{
	return myVisible;
}

///////////////////////////////////////////////////////////////////////////////////////////////
MenuManager* MenuManager::instance() 
{ 
	if(mysInstance == NULL)
	{
		createAndInitialize();
	}
	return mysInstance; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuManager* MenuManager::createAndInitialize()
{
	if(mysInstance == NULL)
	{
		mysInstance = new MenuManager();
		ModuleServices::addModule(mysInstance);
		mysInstance->doInitialize(Engine::instance());
	}
	return mysInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuManager::MenuManager():
	myMainMenu(NULL),
	myDefaultMenuPosition(-1.0, -0.5, -2.0),
	myDefaultMenuScale(1.0f),
	myNavigationSuspended(true),
	myRayPlaceEnabled(true),
	myMenuToggleButton(Event::User),
	myUseMenuToggleButton(false)
{
	mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuManager::~MenuManager()
{
	omsg("~MenuManager");
	mysInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::initialize()
{
	// Create the ui module if one is not available already
	if(UiModule::instance() != NULL)
	{
		myUiModule = UiModule::instance();
	}
	else
	{
		myUiModule = new UiModule();
		ModuleServices::addModule(myUiModule);
		// Force uimodule init.
		myUiModule->doInitialize(getEngine());
	}

	if(SystemManager::settingExists("config/ui"))
	{
		Setting& sUi = SystemManager::settingLookup("config/ui");
		myRayPlaceEnabled = Config::getBoolValue("menuRayPlaceEnabled", sUi, myRayPlaceEnabled);
		myDefaultMenuPosition = Config::getVector3fValue("menuDefaultPosition", sUi, myDefaultMenuPosition);
		myDefaultMenuScale = Config::getFloatValue("menuDefaultScale", sUi, myDefaultMenuScale);
		my3dMenuEnabled = Config::getBoolValue("menu3dEnabled", sUi, myDefaultMenuScale);

		// Parse menu toggle button name (if present)
		String toggleButtonName = Config::getStringValue("menuToggleButton", sUi, "");
		if(toggleButtonName != "")
		{
			myMenuToggleButton = Event::parseButtonName(toggleButtonName);
			myUseMenuToggleButton = true;
		}

		myDefaultMenuScale *= 0.001f;
	}

	if(SystemManager::settingExists("config/sound"))
	{
		Setting& sUi = SystemManager::settingLookup("config/sound");

		showMenuSound = getEngine()->getSoundEnvironment()->createSound("showMenuSound");
		showMenuSound->loadFromFile( Config::getStringValue("showMenuSound", sUi, "showMenu.wav").c_str() );

		hideMenuSound = getEngine()->getSoundEnvironment()->createSound("hideMenuSound");
		hideMenuSound->loadFromFile( Config::getStringValue("hideMenuSound", sUi, "hideMenu.wav").c_str() );
		
		selectMenuSound = getEngine()->getSoundEnvironment()->createSound("selectMenuSound");
		selectMenuSound->loadFromFile( Config::getStringValue("selectMenuSound", sUi, "selectMenu.wav").c_str() );
		
		scrollMenuSound = getEngine()->getSoundEnvironment()->createSound("scrollMenuSound");
		scrollMenuSound->loadFromFile( Config::getStringValue("scrollMenuSound", sUi, "scrollMenu.wav").c_str() );
		
		float volume = Config::getFloatValue("menuSoundVolume", sUi, 1.0);
		float width = Config::getFloatValue("menuSoundWidth", sUi, 1.0);
		float mix = Config::getFloatValue("menuSoundMix", sUi, 1.0);
		float reverb = Config::getFloatValue("menuSoundReverb", sUi, 1.0);
		
		showMenuSound->setDefaultParameters(volume, width, mix, reverb, false);
		hideMenuSound->setDefaultParameters(volume, width, mix, reverb, false);
		selectMenuSound->setDefaultParameters(volume, width, mix, reverb, false);
		scrollMenuSound->setDefaultParameters(volume, width, mix, reverb, false);
	}

	// Read configuration parameters from system config
	//Setting& sSysCfg = SystemManager::instance()->getSystemConfig()->lookup("config");
	//myMenu3dEnabled = Config::getBoolValue("menu3dEnabled", sSysCfg, false);nMenu
	//myMenu3dEnabled = true;
	//if(myMenu3dEnabled)
	//{
	//	myAutoPlaceEnabled = true;
	//	myAutoPlaceDistance = Config::getFloatValue("menu3dDistance", sSysCfg, myAutoPlaceDistance);
	//	myMenu3dScale = Config::getFloatValue("menu3dScale", sSysCfg, myMenu3dScale);
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::dispose()
{
	ofmsg("MenuManager::dispose: cleaning up %1% menus", %myMenuList.size());
	myMainMenu = NULL;
	myMenuList.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::update(const UpdateContext& context)
{
	foreach(Menu* m, myMenuList)
	{
		m->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::handleEvent(const Event& evt)
{
	if(!evt.isProcessed())
	{
		if(myMainMenu != NULL)
		{
			if(!myMainMenu->isVisible())
			{
				bool showMenuButtonPressed = 
					(myUseMenuToggleButton && evt.isButtonDown(myMenuToggleButton)) ||
					(!myUseMenuToggleButton && evt.isButtonDown(UiModule::getConfirmButton()));

				if(showMenuButtonPressed)
				{
					autoPlaceMenu(myMainMenu, evt);
					myMainMenu->show();
					if(myNavigationSuspended)
					{
						Camera* cam = getEngine()->getDefaultCamera();
						myNavigationState = cam->isControllerEnabled();
						cam->setControllerEnabled(false);
					}
				}
			}
			else
			{
				bool hideMenuButtonPressed = 
					(myUseMenuToggleButton && evt.isButtonDown(myMenuToggleButton)) ||
					(!myUseMenuToggleButton && evt.isButtonDown(UiModule::getCancelButton()));

				if(hideMenuButtonPressed)
				{
					Menu* topMostMenu = myMainMenu->getTopActiveSubMenu();
					if(topMostMenu == myMainMenu) 
					{
						myMainMenu->hide();
						if(myNavigationSuspended)
						{
							Camera* cam = getEngine()->getDefaultCamera();
							cam->setControllerEnabled(myNavigationState);
						}
					}
					else
					{
						Menu* parent = topMostMenu->getParent();
						parent->setActiveSubMenu(NULL);
						parent->focus();
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::autoPlaceMenu(Menu* menu, const Event& evt)
{
	if(myRayPlaceEnabled)
	{
		menu->placeOnWand(evt);
	}
	else
	{
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		Camera* cam = Engine::instance()->getDefaultCamera();
		
		//ofmsg("MENU World head position: %1%", %obs->getWorldHeadPosition());

		Vector3f obsForward = cam->getOrientation() * Vector3f(0, 0, -1);
		Vector3f headWorldPos = cam->getPosition() + cam->getHeadOffset();
		Vector3f menuPosition = headWorldPos + cam->getHeadOrientation() * myDefaultMenuPosition;
		//menuPosition = Vector3f(0, 1, -3);

		ofmsg("MENU Menu position, offset: %1% %2%", %menuPosition %myDefaultMenuPosition);
		
		Container3dSettings& c3ds = menu->get3dSettings();
		Widget* menuWidget = menu->getContainer();

		c3ds.scale = myDefaultMenuScale;
		Vector3f offset = Vector3f(0, menuWidget->getHeight() * c3ds.scale, 0);
		c3ds.position = menuPosition - offset;

		c3ds.normal = -obsForward;
		c3ds.scale = myDefaultMenuScale;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu* MenuManager::createMenu(const String& name)
{
	Menu* menu = new Menu(name, this);
	myMenuList.push_back(menu);
	
	if(hideMenuSound != NULL)
	{
		// Mutes menu sound for default hide
		hideMenuSound->setVolumeScale(0);
	}
	
	// Set not visible by default.
	menu->hide();
	
	if(hideMenuSound != NULL)
	{
		// Reenables menu sound
		hideMenuSound->setVolumeScale(1);
	}
	
	return menu;
}
