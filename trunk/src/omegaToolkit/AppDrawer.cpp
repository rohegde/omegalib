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
#include "omegaToolkit/AppDrawer.h"
#include "omegaToolkit/UiScriptCommand.h"

using namespace omegaToolkit;
using namespace omegaToolkit::ui;
using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppInfo::initialize(AppDrawer* drawer)
{
	myDrawer = drawer;

	myUi = myDrawer->getUi();
	WidgetFactory* wf = myUi->getWidgetFactory();

	myContainer = wf->createContainer(name, drawer->getContainer(), Container::LayoutVertical);

	myImage = wf->createImage(name + "Icon", myContainer);
	myImage->setData(ImageUtils::loadImage(iconFile));
	myImage->setSize(Vector2f(myDrawer->getIconSize(), myDrawer->getIconSize()));

	myButton = wf->createButton(name + "Button", myContainer);
	myButton->setText(label);

	// Add an event handler to the button that will run the specified application.
	// the ':r!' command is handled by the OmegaViewer::handleCommand method.
	myCommand = new UiScriptCommand(ostr(":r! %1%", %name));
	myButton->setUIEventHandler(myCommand.get());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppDrawer::initialize()
{
	myDrawerScale = 1.0f;
	myIconSize = 128;
	myUi = UiModule::instance();
	myContainer = myUi->getWidgetFactory()->createContainer("appDrawer", myUi->getUi(), Container::LayoutHorizontal);
	myContainer->get3dSettings().enable3d = true;
	myContainer->get3dSettings().center = true;
	hide();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppDrawer::addApp(AppInfo* info)
{
	info->initialize(this);
	myAppList.push_back(info);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppDrawer::update(const UpdateContext& context)
{
	float speed = context.dt * 10;

	ui::Container3dSettings& c3ds = myContainer->get3dSettings();
	//c3ds.enable3d = my3dSettings.enable3d;
	//c3ds.normal = my3dSettings.normal;
	//c3ds.up = my3dSettings.up;
	c3ds.position += (my3dSettings.position - c3ds.position) * speed;
	c3ds.scale += (my3dSettings.scale - c3ds.scale) * speed;
	c3ds.alpha += (my3dSettings.alpha - c3ds.alpha) * speed;

	if(myContainer->isVisible())
	{
		if(c3ds.alpha <= 0.1f)
		{
			myContainer->setVisible(false);
			myVisible = false;
		}
	}
	else
	{
		if(c3ds.alpha > 0.1f)
		{
			myContainer->setVisible(true);
			myVisible = true;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppDrawer::show()
{
	myVisible = true;
	myContainer->setEnabled(true);

	myContainer->get3dSettings().alpha = 0.0f;
	my3dSettings.enable3d = true;
	my3dSettings.alpha = 1.0f;
	my3dSettings.up = Vector3f(0, 1, 0);
	my3dSettings.normal = Vector3f(0, 0, 1);

	myContainer->get3dSettings().scale = myDrawerScale / 2000;
	my3dSettings.scale = myDrawerScale / 1000;

	Camera* cam = Engine::instance()->getDefaultCamera();
	Vector3f pos = cam->getPosition() + cam->getHeadOffset();
	pos.z() -= 2.5;
	ofmsg("App drawer position: %1%", %pos);
	my3dSettings.position = pos;
	
	UiModule::instance()->activateWidget(myContainer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppDrawer::hide()
{
	//omsg("Menu hide");

	myVisible = false;
	myContainer->setEnabled(false);
	//myContainer->setDebugModeEnabled(true);

	UiModule::instance()->activateWidget(NULL);

	my3dSettings.alpha = 0.0f;
	my3dSettings.scale = myDrawerScale / 1000;
}
