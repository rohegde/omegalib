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
#include "omegaToolkit/ui/MenuManager.h"
#include "omega/DisplaySystem.h"

using namespace omegaToolkit::ui;

MenuManager* MenuManager::mysInstance = NULL;

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
	myNavigationSuspended(false),
	myRayPlaceEnabled(true),
	myMenuToggleButton(Event::User),
	myUseMenuToggleButton(false),
	myHideSoundMenu(NULL),
	myShowMenuSound(NULL)
{
	setPriority(EngineModule::PriorityLow);
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
	myUiModule = UiModule::createAndInitialize();

	if(SystemManager::settingExists("config/ui"))
	{
		Setting& sUi = SystemManager::settingLookup("config/ui");
		myRayPlaceEnabled = Config::getBoolValue("menuRayPlaceEnabled", sUi, myRayPlaceEnabled);
		myDefaultMenuPosition = Config::getVector3fValue("menuDefaultPosition", sUi, myDefaultMenuPosition);
		myDefaultMenuScale = Config::getFloatValue("menuDefaultScale", sUi, myDefaultMenuScale);
		my3dMenuEnabled = Config::getBoolValue("menu3dEnabled", sUi, true);
		myNavigationSuspended = Config::getBoolValue("menuSuspendNavigation", sUi, myNavigationSuspended);

		// Parse menu toggle button name (if present)
		String toggleButtonName = Config::getStringValue("menuToggleButton", sUi, "");
		if(toggleButtonName != "")
		{
			myMenuToggleButton = Event::parseButtonName(toggleButtonName);
			myUseMenuToggleButton = true;
		}

		myDefaultMenuScale *= 0.001f;
	}

	// See if we have a sound environment available.
	// If we don't it means sounds are disabled OR we are not the master node.
	SoundEnvironment* se = getEngine()->getSoundEnvironment();
	if(se != NULL)
	{
		if(SystemManager::settingExists("config/sound"))
		{
			Setting& sUi = SystemManager::settingLookup("config/sound");

			float volume = Config::getFloatValue("menuSoundVolume", sUi, 1.0);
			float width = Config::getFloatValue("menuSoundWidth", sUi, 1.0);
			float mix = Config::getFloatValue("menuSoundMix", sUi, 1.0);
			float reverb = Config::getFloatValue("menuSoundReverb", sUi, 1.0);

			if( sUi.exists("showMenuSound") ){
				myShowMenuSound = se->createSound("showMenuSound");
				myShowMenuSound->loadFromFile( Config::getStringValue("showMenuSound", sUi ) );
				myShowMenuSound->setDefaultParameters(volume, width, mix, reverb, false, false);
			}

			if( sUi.exists("hideMenuSound") ){
				myHideSoundMenu = se->createSound("hideMenuSound");
				myHideSoundMenu->loadFromFile( Config::getStringValue("hideMenuSound", sUi ) );
				myHideSoundMenu->setDefaultParameters(volume, width, mix, reverb, false, false);
			}

			if( sUi.exists("selectMenuSound") ){
				selectMenuSound = se->createSound("selectMenuSound");
				selectMenuSound->loadFromFile( Config::getStringValue("selectMenuSound", sUi ) );
				selectMenuSound->setDefaultParameters(volume, width, mix, reverb, false, false);
			}

			if( sUi.exists("scrollMenuSound") ){
				scrollMenuSound = se->createSound("scrollMenuSound");
				scrollMenuSound->loadFromFile( Config::getStringValue("scrollMenuSound", sUi ) );
				scrollMenuSound->setDefaultParameters(volume, width, mix, reverb, false, false);
			}
			
		}
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
					evt.isButtonDown(UiModule::getCancelButton());

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
	
	if(myHideSoundMenu != NULL)
	{
		// Mutes menu sound for default hide
		myHideSoundMenu->setVolumeScale(0);
	}
	
	// Set not visible by default.
	menu->hide();
	
	if(myHideSoundMenu != NULL)
	{
		// Reenables menu sound
		myHideSoundMenu->setVolumeScale(1);
	}
	
	return menu;
}
