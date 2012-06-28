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
#include <omega.h>
#include <cyclops.h>
#include <omegaToolkit.h>

using namespace omega;
using namespace omegaToolkit;
using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
class StructureViewer: public ServerModule, ui::IMenuItemListener
{
public:
	enum ControlMode { ControlEntity, ControlCamera, ControlLight };

public:
	StructureViewer();

	virtual void initialize();
	virtual void handleEvent(const Event& evt);
	virtual void onMenuItemEvent(ui::MenuItem* mi);

private:
	SceneManager* mySceneManager;
	ui::MenuManager* myMenuManager;

	SceneEditorModule* myEditor;

	ControlMode myControlMode;

	cyclops::Light* myLight;
	float myLightDistance;

	// User interface stuff.
	ui::MenuItem* myControlEntityButton;
	ui::MenuItem* myControlLightButton;
	ui::MenuItem* myControlCameraButton;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
StructureViewer::StructureViewer():
	mySceneManager(NULL),
	myMenuManager(NULL),
	myEditor(NULL),
	myLight(NULL),
	myLightDistance(5.0f)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void StructureViewer::initialize()
{
	// Create a scene editor to manipulate entities.
	myEditor = SceneEditorModule::createAndInitialize();
	myEditor->setEnabled(false);

	// Create and initialize the cyclops scene manager.
	// If a scene file is specified in the application config file, the scene manager will
	// load it during initialization.
	mySceneManager = SceneManager::createAndInitialize();

	// Create the menu manager and a main menu.
	myMenuManager = ui::MenuManager::createAndInitialize();
	ui::Menu* menu = myMenuManager->createMenu("menu");
	myMenuManager->setMainMenu(menu);

	ui::MenuItem* viewMenu = menu->addItem(ui::MenuItem::SubMenu);
	viewMenu->setText("View");

	// For each entity in the scene, add a checkbox to the menu, that will be used
	// to toggle the entity visibility.
	foreach(DrawableObject* dobj, mySceneManager->getObjects())
	{
		Entity* e = Entity::fromDrawableObject(dobj);
		if(e != NULL)
		{
			ui::MenuItem* mi = viewMenu->getSubMenu()->addItem(ui::MenuItem::Checkbox);
			mi->setChecked(false);
			e->setVisible(false);
			mi->setText(e->getTag());
			mi->setUserData(e);
			mi->setListener(this);
			mi->setUserTag("visibilityToggle");

			// Register the entity to the node editor, so it can be manipulated by the user.
			myEditor->addNode(e);
		}
	}

	// Create the control submenu
	ui::MenuItem* optionsMenu = menu->addItem(ui::MenuItem::SubMenu);
	optionsMenu->setText("Control");
	// control camera button
	myControlCameraButton = optionsMenu->getSubMenu()->addItem(ui::MenuItem::Checkbox);
	myControlCameraButton->setText("Control Camera");
	myControlCameraButton->setChecked(true);
	myControlCameraButton->setListener(this);
	myControlCameraButton->setUserTag("control");
	// control entity button
	myControlEntityButton = optionsMenu->getSubMenu()->addItem(ui::MenuItem::Checkbox);
	myControlEntityButton->setText("Control Entity");
	myControlEntityButton->setChecked(false);
	myControlEntityButton->setListener(this);
	myControlEntityButton->setUserTag("control");
	// control light button
	myControlLightButton = optionsMenu->getSubMenu()->addItem(ui::MenuItem::Checkbox);
	myControlLightButton->setText("Control Light");
	myControlLightButton->setChecked(false);
	myControlLightButton->setListener(this);
	myControlLightButton->setUserTag("control");

	myControlMode = ControlCamera;

	// Add the 'quit' menu item.
	ui::MenuItem* quitMenuItem = menu->addItem(ui::MenuItem::Button);
	quitMenuItem->setText("Quit Structure");
	quitMenuItem->setCommand("oexit()");

	myLight = new Light(mySceneManager);
	myLight->setEnabled(true);
	myLight->setPosition(Vector3f(0, 50, 0));
	myLight->setColor(Color(1.0f, 1.0f, 0.7f));
	myLight->setAmbient(Color(0.1f, 0.1f, 0.1f));
	mySceneManager->setMainLight(myLight);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void StructureViewer::onMenuItemEvent(ui::MenuItem* mi)
{
	if(mi->getUserTag() == "visibilityToggle")
	{
		// Get the entity associated with the button and toggle its visibility.
		Entity* e = (Entity*)mi->getUserData();
		e->setVisible(mi->isChecked());
	}
	else if(mi->getUserTag() == "control")
	{
		if(mi->isChecked())
		{
			if(mi == myControlCameraButton)
			{
				myControlLightButton->setChecked(false);
				myControlEntityButton->setChecked(false);
				myControlMode = ControlCamera;
			}
			else if(mi == myControlLightButton)
			{
				myControlCameraButton->setChecked(false);
				myControlEntityButton->setChecked(false);
				myControlMode = ControlLight;
			}
			else if(mi == myControlEntityButton)
			{
				myControlLightButton->setChecked(false);
				myControlCameraButton->setChecked(false);
				myControlMode = ControlEntity;
				myEditor->setEnabled(true);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void StructureViewer::handleEvent(const Event& evt)
{
	if(myControlMode == ControlLight)
	{
		if(evt.getServiceType() == Service::Pointer ||
			evt.getServiceType() == Service::Controller ||
			evt.getServiceType() == Service::Wand)
		{
			if(evt.isFlagSet(Event::Button1))
			{
				Ray r;
				if(getServer()->getDisplaySystem()->getViewRayFromEvent(evt, r))
				{
					myLight->setPosition(r.getPoint(myLightDistance));
				}
				evt.setProcessed();
			}
			// If the event has a floating point extra data field (usualy mapped to a controller axis)
			// use it to change the light distance.
			if(evt.getExtraDataType() == Event::ExtraDataFloatArray && evt.getExtraDataItems() >= 1)
			{
				float v = evt.getExtraDataFloat(0) / 10;
				if(abs(v) > 0.01f)
				{
					myLightDistance += v;
					evt.setProcessed();
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<StructureViewer> app("structure");
	return omain(app, argc, argv);
}

