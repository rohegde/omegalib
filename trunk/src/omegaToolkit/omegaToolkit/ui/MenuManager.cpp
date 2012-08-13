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
String MenuItem::getCommand()
{
	if(myCommand != NULL)
	{
		return myCommand->getCommand();
	}
	return "";
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

	my3dSettings.enable3d = true; //MenuManager::instance()->isMenu3dEnabled();
	myContainer->setAutosize(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuItem* Menu::addItem(MenuItem::Type type)
{
	MenuItem* item = new MenuItem(type, this);
	myMenuItems.push_back(item);
	return item;
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

	focus();

	myContainer->get3dSettings().alpha = 0.0f;
	my3dSettings.alpha = 1.0f;

	myContainer->get3dSettings().scale = myManager->getDefaultMenuScale() / 2;
	my3dSettings.scale = myManager->getDefaultMenuScale();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::hide()
{
	//omsg("Menu hide");

	myVisible = false;

	UiModule::instance()->activateWidget(NULL);

	my3dSettings.alpha = 0.0f;
	my3dSettings.scale = myManager->getDefaultMenuScale() / 2;

	if(myActiveSubMenu != NULL)
	{
		myActiveSubMenu->hide();
		myActiveSubMenu = NULL;
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
	myRayPlaceEnabled(true)
{
	mysInstance = this;
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
		myDefaultMenuScale *= 0.001f;
	}

	// Read configuration parameters from system config
	//Setting& sSysCfg = SystemManager::instance()->getSystemConfig()->lookup("config");
	//myMenu3dEnabled = Config::getBoolValue("menu3dEnabled", sSysCfg, false);
	//myMenu3dEnabled = true;
	//if(myMenu3dEnabled)
	//{
	//	myAutoPlaceEnabled = true;
	//	myAutoPlaceDistance = Config::getFloatValue("menu3dDistance", sSysCfg, myAutoPlaceDistance);
	//	myMenu3dScale = Config::getFloatValue("menu3dScale", sSysCfg, myMenu3dScale);
	//}
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
				if(evt.isButtonDown(UiModule::getConfirmButton()))
				{
					myMainMenu->show();
					autoPlaceMenu(myMainMenu, evt);
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
				if(evt.isButtonDown(UiModule::getCancelButton()))
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
		Ray ray;
		if(SystemManager::instance()->getDisplaySystem()->getViewRayFromEvent(evt, ray))
		{
			Vector3f pos = ray.getPoint(myDefaultMenuPosition[2]);

			Container3dSettings& c3ds = menu->get3dSettings();
			Widget* menuWidget = menu->getContainer();
			Vector3f offset = Vector3f(0, menuWidget->getHeight() * c3ds.scale, 0);
			c3ds.position = pos - offset;
			//c3ds.normal = -ray.getDirection();
			c3ds.normal = Vector3f(0, 0, 1);
			c3ds.scale = myDefaultMenuScale;
		}
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
	// Set not visible by default.
	menu->hide();
	return menu;
}
