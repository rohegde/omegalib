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
#ifndef __APP_DRAWER__
#define __APP_DRAWER__

#include "omega/Engine.h"
#include "omega/Application.h"
#include "omega/PythonInterpreter.h"
#include "omegaToolkit/UiModule.h"
#include "ui/Container.h"
#include "ui/Button.h"
#include "ui/WidgetFactory.h"

namespace omegaToolkit
{
class AppDrawer;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct OTK_API AppInfo: public ReferenceType
{
	String name;
	String label;
	String iconFile;

	void initialize(AppDrawer* drawer);

	AppDrawer* myDrawer;

	// ui stuff.
	Ref<UiModule> myUi;
	Ref<ui::Container> myContainer;
	Ref<ui::Label> myLabel;
	Ref<ui::Image> myImage;
	Ref<UiScriptCommand> myCommand;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class IAppDrawerListener
{
public:
	virtual void startApp(AppInfo* app) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class OTK_API AppDrawer: public ReferenceType
{
public:
	AppDrawer(PythonInterpreter* interp, UiModule* ui);
	virtual ~AppDrawer() {}

	void initialize();
	ui::Container* getContainer() { return myContainer; }
	UiModule* getUi() { return myUi; }
	void update(const UpdateContext& context);
	void handleEvent(const Event& evt);
	void show();
	void hide();
	bool isVisible() { return myVisible; }
	void addApp(AppInfo* app);
	float getDrawerScale() { return myDrawerScale; }
	void setDrawerScale(float value) { myDrawerScale = value; }
	int getIconSize() { return myIconSize; }
	void setIconSize(int value) { myIconSize = value; }
	void setSelectedApp(AppInfo* i);
	void selectNextApp();
	void selectPrevApp();

	void setListener(IAppDrawerListener* listener) { myListener = listener; }
	IAppDrawerListener* getListener() { return myListener; }

private:
	Ref<UiModule> myUi;
	List < Ref<AppInfo> > myAppList;
	AppInfo* mySelectedApp;
	Ref<ui::Container> myContainer;
	ui::Container3dSettings my3dSettings;
	PythonInterpreter* myInterpreter;
	bool myVisible;
	float myDrawerScale;
	int myIconSize;

	IAppDrawerListener* myListener;
};
};
#endif