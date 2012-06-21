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
		myWidget->setUIEventHandler(myCommand);
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
			mySubMenu->get3dSettings() = myMenu->get3dSettings();

			// Compute the submenu position (place it at the side of the main menu).
			Vector3f normal = mySubMenu->get3dSettings().normal;
			Vector3f up = -mySubMenu->get3dSettings().up;
			Vector3f size = normal.cross(up);
			Vector3f menuPos = mySubMenu->get3dSettings().position;
			menuPos += size * myWidget->getWidth() * mySubMenu->get3dSettings().scale;
			mySubMenu->get3dSettings().position = menuPos;

			mySubMenu->show();

			if(myMenu->myActiveSubMenu != NULL)
			{
				myMenu->myActiveSubMenu->hide();
				myMenu->myActiveSubMenu = NULL;
			}
			myMenu->myActiveSubMenu = mySubMenu;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu::Menu(const String& name, MenuManager* manager): 
	myName(name),
	myManager(manager),
	myActiveSubMenu(NULL)
{
	UiModule* ui = UiModule::instance();
	WidgetFactory* wf = ui->getWidgetFactory();
	myContainer = wf->createPanel("container", ui->getUi());
	myContainer->setPosition(Vector2f(10, 10));

	my3dSettings.enable3d = MenuManager::instance()->isMenu3dEnabled();
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
void Menu::show()
{
	//omsg("Menu show");

	myVisible = true;

	//myContainer->setVisible(true);
	UiModule::instance()->activateWidget(myContainer);
	UiModule::instance()->setGamepadInteractionEnabled(true);
	ServerEngine::instance()->getDefaultCamera()->setControllerEnabled(false);

	myContainer->get3dSettings().alpha = 0.0f;
	my3dSettings.alpha = 1.0f;

	myContainer->get3dSettings().scale = myManager->getMenu3dScale() / 2;
	my3dSettings.scale = myManager->getMenu3dScale();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::hide()
{
	//omsg("Menu hide");

	myVisible = false;

	UiModule::instance()->activateWidget(NULL);
	UiModule::instance()->setGamepadInteractionEnabled(false);
	ServerEngine::instance()->getDefaultCamera()->setControllerEnabled(true);

	my3dSettings.alpha = 0.0f;
	my3dSettings.scale = myManager->getMenu3dScale() / 2;

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
		mysInstance->doInitialize(ServerEngine::instance());
	}
	return mysInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuManager::MenuManager():
	myMainMenu(NULL),
	myAutoPlaceDistance(0.5f),
	myAutoPlaceEnabled(true),
	myMenu3dScale(0.002f)
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
		myUiModule->doInitialize(getServer());
	}

	// Read configuration parameters from system config
	Setting& sSysCfg = SystemManager::instance()->getSystemConfig()->lookup("config");
	myMenu3dEnabled = Config::getBoolValue("menu3dEnabled", sSysCfg, false);
	if(myMenu3dEnabled)
	{
		myAutoPlaceEnabled = true;
		myAutoPlaceDistance = Config::getFloatValue("menu3dDistance", sSysCfg, myAutoPlaceDistance);
		myMenu3dScale = Config::getFloatValue("menu3dScale", sSysCfg, myMenu3dScale);
	}
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
		if(evt.getType() == Event::Down && evt.isFlagSet(Event::Button3))
		{
			if(myMainMenu != NULL)
			{
				myMainMenu->toggle();

				// If menu autoplace is enabled, place menu along the pointer or wand ray, at
				// the distance specified by myAutoPlaceDistance.
				if(myMainMenu->isVisible() && myAutoPlaceEnabled)
				{
					autoPlaceMenu(myMainMenu, evt);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::autoPlaceMenu(Menu* menu, const Event& evt)
{
	Ray ray;
	if(SystemManager::instance()->getDisplaySystem()->getViewRayFromEvent(evt, ray))
	{
		Vector3f pos = ray.getPoint(myAutoPlaceDistance);

		Container3dSettings& c3ds = menu->get3dSettings();
		Widget* menuWidget = menu->getContainer();
		Vector3f offset = Vector3f(0, menuWidget->getHeight() * c3ds.scale, 0);
		c3ds.position = pos - offset;
		c3ds.normal = -ray.getDirection();
		c3ds.scale = myMenu3dScale;
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
