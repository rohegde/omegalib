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
#include "cyclops/MenuManager.h"

using namespace omegaToolkit::ui;
using namespace cyclops;

MenuManager* MenuManager::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuItem::MenuItem(Type type, Menu* owner, MenuItem* parent):
	myMenu(owner),
	myType(type),
	myParent(parent),
	myContainer(NULL),
	myButton(NULL),
	myCommand(NULL),
	myWidget(NULL),
	myUserData(NULL)
{
	UiModule* ui = owner->getManager()->getUiModule();
	WidgetFactory* wf = ui->getWidgetFactory();

	if(type == MenuItem::SubMenu)
	{
		myContainer = wf->createPanel("container", ui->getUi());
		myContainer->setWidth(400);
		myContainer->setHeight(SystemManager::instance()->getDisplaySystem()->getCanvasSize().y());
		//myContainer->setAutosize(true);
		myContainer->setPosition(Vector2f(10, 10));

		myContainer->get3dSettings().enable3d = MenuManager::instance()->isMenu3dEnabled();

		myWidget = myContainer;
	}
	else if(parent != NULL)
	{
		if(type == MenuItem::Button)
		{
			myButton = wf->createButton("button", parent->myContainer);
			myButton->setText("Button");
			myWidget = myButton;
		}
		if(type == MenuItem::Checkbox)
		{
			myButton = wf->createButton("button", parent->myContainer);
			myButton->setText("Checkbox");
			myButton->setCheckable(true);
			myWidget = myButton;
		}
	}
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
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuItem* MenuItem::addItem(MenuItem::Type type)
{
	if(myType == SubMenu)
	{
		MenuItem* item = new MenuItem(type, myMenu, this);
		mySubMenuItems.push_back(item);
		return item;
	}
	owarn("MenuItem::addItem: menu items can be added only to sub menus");
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Container* MenuItem::getContainerWidget()
{
	if(myType == SubMenu) return myContainer;
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu::Menu(const String& name, MenuManager* manager): 
	myName(name),
	myManager(manager)
{
	myRootItem = new MenuItem(MenuItem::SubMenu, this, NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::show()
{
	omsg("Menu show");
	Container* c = myRootItem->getContainerWidget();
	c->setVisible(true);
	UiModule::instance()->activateWidget(c);
	UiModule::instance()->setGamepadInteractionEnabled(true);
	ServerEngine::instance()->getDefaultCamera()->setControllerEnabled(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::hide()
{
	omsg("Menu hide");
	Container* c = myRootItem->getContainerWidget();
	c->setVisible(false);
	UiModule::instance()->activateWidget(NULL);
	UiModule::instance()->setGamepadInteractionEnabled(false);
	ServerEngine::instance()->getDefaultCamera()->setControllerEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::toggle()
{
	Container* c = myRootItem->getContainerWidget();
	if(c->isVisible()) hide();
	else show();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Menu::isVisible()
{
	return myRootItem->getContainerWidget()->isVisible();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MenuManager::MenuManager():
	myMainMenu(NULL),
	myAutoPlaceDistance(0.5f),
	myAutoPlaceEnabled(true)
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
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MenuManager::handleEvent(const Event& evt)
{
	if(!evt.isProcessed())
	{
		if(evt.getType() == Event::Down && evt.isFlagSet(Event::Button2))
		{
			if(myMainMenu != NULL)
			{
				myMainMenu->toggle();

				// If menu autoplace is enabled, place menu along the pointer or wand ray, at
				// the distance specified by myAutoPlaceDistance.
				if(myAutoPlaceEnabled)
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
	// Extract the pointer / wand ray from the event.
	// NOTE: if the event does not contain this information, the default is
	// a ray starting at the origin and pointing in the negative Z direction.
	Ray ray(Vector3f::Zero(), -Vector3f::UnitZ());
	if(evt.getServiceType() == Service::Pointer && 
		evt.getExtraDataType() == Event::ExtraDataVector3Array &&
		evt.getExtraDataItems() == 2)
	{
		// Pointer event: ray information is in the extra data section of the event.
		ray.setOrigin(evt.getExtraDataVector3(0));
		ray.setDirection(evt.getExtraDataVector3(1));
	}
	else if(evt.getServiceType() == Service::Wand)
	{
		// Wand event: generate a ray using the wand position and orientation.
		ray.setOrigin(evt.getPosition());
		ray.setDirection(evt.getOrientation() * -Vector3f::UnitZ());
	}

	Vector3f pos = ray.getPoint(myAutoPlaceDistance);

	Container3dSettings& c3ds = menu->get3dSettings();
	Widget* menuWidget = menu->getRoot()->getWidget();
	Vector3f offset = Vector3f(0, menuWidget->getHeight() * c3ds.scale, 0);
	c3ds.position = pos - offset;
	c3ds.normal = -ray.getDirection();

	//c3ds.yaw = q.getYaw() * Math::RadToDeg;
	//c3ds.pitch = q.getPitch() * Math::RadToDeg;
	//c3ds.roll = q.getRoll() * Math::RadToDeg;

	//ofmsg("MenuManager::autoPlaceMenu: pos=%1% pitch=%2% yaw=%3% roll=%4%", %c3ds.position %c3ds.pitch %c3ds.yaw %c3ds.roll);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu* MenuManager::createMenu(const String& name)
{
	Menu* menu = new Menu(name, this);
	myMenuDictionary[name] = menu;
	// Set not visible by default.
	menu->hide();
	return menu;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Menu* MenuManager::getMenu(const String& name)
{
	return myMenuDictionary[name];
}
