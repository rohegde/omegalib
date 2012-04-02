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
	myWidget(NULL)
{
	UiModule* ui = owner->getManager()->getUiModule();
	WidgetFactory* wf = ui->getWidgetFactory();

	if(type == MenuItem::SubMenu)
	{
		myContainer = wf->createContainer("container", ui->getUi(), Container::LayoutVertical);
		myContainer->setWidth(200);
		myContainer->setHeight(200);
		myContainer->setPosition(Vector2f(10, 10));

		myWidget = myContainer;
	}
	else if(parent != NULL)
	{
		if(type == MenuItem::Button)
		{
			myButton = wf->createButton("button", parent->myContainer);
			myButton->setText("Hello World");
			myWidget = myButton;
		}
		if(type == MenuItem::Checkbox)
		{
			myButton = wf->createButton("button", parent->myContainer);
			myButton->setText("Hello Checkbox");
			myButton->setCheckable(true);
			myWidget = myButton;
		}
	}
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
const String& MenuItem::getCommand()
{
	if(myCommand != NULL)
	{
		return myCommand->getCommand();
	}
	return "";
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
	myMainMenu(NULL)
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
			}
		}
	}
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
