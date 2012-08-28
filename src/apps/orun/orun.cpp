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
#include <omegaToolkit.h>

#ifdef OMEGA_BUILD_VTK_LIB
#include <omegaVtk.h>
#endif

#ifdef OMEGA_BUILD_OSG_LIB
#include <cyclops.h>
#endif

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

#ifdef OMEGA_BUILD_VTK_LIB
using namespace omegaVtk;
#endif

using namespace cyclops;

// The name of the script to launch automatically at startup
String sDefaultScript = "";

class AppDrawer;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct AppInfo: public ReferenceType
{
	String name;
	String label;
	String iconFile;

	void initialize(AppDrawer* drawer);

	AppDrawer* myDrawer;

	// ui stuff.
	Ref<UiModule> myUi;
	Ref<Container> myContainer;
	Ref<Button> myButton;
	Ref<Image> myImage;
	Ref<UiScriptCommand> myCommand;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class AppDrawer: public ReferenceType
{
public:

	void initialize();
	Container* getContainer() { return myContainer; }
	UiModule* getUi() { return myUi; }
	void update(const UpdateContext& context);
	void show();
	void hide();
	bool isVisible() { return myVisible; }
	void addApp(AppInfo* app);
	float getDrawerScale() { return myDrawerScale; }
	void setDrawerScale(float value) { myDrawerScale = value; }
	int getIconSize() { return myIconSize; }
	void setIconSize(int value) { myIconSize = value; }

private:
	Ref<UiModule> myUi;
	List < Ref<AppInfo> > myAppList;
	Ref<Container> myContainer;
	Container3dSettings my3dSettings;
	bool myVisible;
	float myDrawerScale;
	int myIconSize;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
class OmegaViewer: public EngineModule
{
public:
	OmegaViewer();

	virtual void initialize();
	void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);
	virtual void handleCommand(const String& cmd);

	AppDrawer* getAppDrawer() { return myAppDrawer; }

public:
	//! Start the selected application
	void run(const String& appName);
	//! Unload all running applications
	void reset();

private:
	Ref<UiModule> myUiModule;
	Ref<AppDrawer> myAppDrawer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer* gViewerInstance = NULL;
OmegaViewer* getViewer() { return gViewerInstance; }

///////////////////////////////////////////////////////////////////////////////////////////////////
// Python wrapper code.
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(omegaViewer)
{
	// SceneLoader
	PYAPI_REF_BASE_CLASS(OmegaViewer)
		PYAPI_METHOD(OmegaViewer, run)
		PYAPI_METHOD(OmegaViewer, reset)
		PYAPI_REF_GETTER(OmegaViewer, getAppDrawer)
		;

	// App drawer
	PYAPI_REF_BASE_CLASS(AppDrawer)
		PYAPI_METHOD(AppDrawer, show)
		PYAPI_METHOD(AppDrawer, hide)
		PYAPI_METHOD(AppDrawer, isVisible)
		PYAPI_METHOD(AppDrawer, addApp)
		PYAPI_METHOD(AppDrawer, setDrawerScale)
		PYAPI_METHOD(AppDrawer, getDrawerScale)
		PYAPI_METHOD(AppDrawer, getIconSize)
		PYAPI_METHOD(AppDrawer, setIconSize)
		;

	// AppInfo
	PYAPI_REF_BASE_CLASS_WITH_CTOR(AppInfo)
		PYAPI_PROPERTY(AppInfo, name)
		PYAPI_PROPERTY(AppInfo, label)
		PYAPI_PROPERTY(AppInfo, iconFile)
		;

	def("getViewer", getViewer, PYAPI_RETURN_POINTER);
}

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
	my3dSettings.alpha = 1.0f;

	myContainer->get3dSettings().scale = myDrawerScale / 2000;
	my3dSettings.scale = myDrawerScale / 1000;

	Camera* cam = Engine::instance()->getDefaultCamera();
	Vector3f pos = cam->getPosition() + cam->getHeadOffset();
	pos.z() -= 2.5;
	my3dSettings.position = pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AppDrawer::hide()
{
	//omsg("Menu hide");

	myVisible = false;
	myContainer->setEnabled(false);

	UiModule::instance()->activateWidget(NULL);

	my3dSettings.alpha = 0.0f;
	my3dSettings.scale = myDrawerScale / 1000;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
	gViewerInstance = this;

	myUiModule = new UiModule();
	ModuleServices::addModule(myUiModule);

	myAppDrawer = new AppDrawer();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::initialize()
{
#ifdef OMEGA_BUILD_VTK_LIB
	omegaVtkPythonApiInit();
#endif

	omegaToolkitPythonApiInit();

#ifdef OMEGA_BUILD_OSG_LIB
	cyclopsPythonApiInit();
#endif

	// Initialize the python wrapper module for this class.
	initomegaViewer();

	if(sDefaultScript != "")
	{
		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->queueInteractiveCommand(ostr("orun(\"%1%\")", %sDefaultScript));
	}

	myAppDrawer->initialize();


	//Setting& base = SystemManager::instance()->getAppConfig()->lookup("config");
	//if(Config::getBoolValue("cyclopsEnabled", base, false))
	//{
	//	SceneManager* sm = SceneManager::createAndInitialize();
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::update(const UpdateContext& context)
{
	myAppDrawer->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::handleEvent(const Event& evt)
{
	// '`' key toggles app drawer.
	if(evt.isKeyDown(KC_HOME) || 
			evt.isButtonDown(Event::Button3))
	{
		if(myAppDrawer->isVisible()) myAppDrawer->hide();
		else  myAppDrawer->show();
	}
	//if(evt.getServiceType() == Service::UI) 
	//{
	//	handleUIEvent(evt);
	//}
	//else
	//{
	//	EngineClient::handleEvent(evt);
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::run(const String& appName)
{
	String baseName = appName + "/" + appName;
	String scriptName = baseName + ".py";
	String cfgName = baseName + ".cfg";

	ofmsg("Looking for script file %1%", %scriptName);
	String scriptPath;
	if(DataManager::findFile(scriptName, scriptPath))
	{
		ofmsg(":: found at %1%", %scriptPath);
		ofmsg("Looking for config file %1%", %cfgName);

		SystemManager* sys = SystemManager::instance();

		String cfgPath;
		if(DataManager::findFile(cfgName, cfgPath))
		{
			ofmsg(":: found at %1%", %cfgPath);

			// Load and set the new app config.
			Config* cfg = new Config(cfgName);
			cfg->load();
			sys->setAppConfig(cfg);
		}

		// Run the application main script.
		sys->getScriptInterpreter()->runFile(scriptName);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::reset()
{
	// u: unload the current application.
	SystemManager* sys = SystemManager::instance();
	PythonInterpreter* interp = sys->getScriptInterpreter();

	// unregister callbacks
	interp->unregisterAllCallbacks();

	// dispose non-core modules
	ModuleServices::disposeNonCoreModules();

	// destroy all global variables
	interp->eval("for uniquevar in [var for var in globals().copy() if var[0] != \"_\" and var != 'clearall']: del globals()[uniquevar]");

	// Use this line instead of the previous to get debugging info on variable deletion. Useful in 
	// case of crashes to know which variable is currently being deleted.
	//interp->eval("for uniquevar in [var for var in globals().copy() if var[0] != \"_\" and var != 'clearall']: print(\"deleting \" + uniquevar); del globals()[uniquevar]");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::handleCommand(const String& cmd)
{
	Vector<String> args = StringUtils::split(cmd);
	if(args[0] == "?")
	{
		// ?: command help
		omsg("OmegaViewer");
		omsg("\t r <appName> - run the specified script application");
		omsg("\t lo          - list live objects");
		omsg("\t ln          - print the scene node tree");
		omsg("\t q           - quit");
	}
	else if(args[0] == "r" && args.size() > 1)
	{
		// r: run application.
		run(args[1]);
	}
	else if(args[0] == "r!" && args.size() > 1)
	{
		// r!: reset state and run application.
		reset();
		run(args[1]);
	}
	else if(args[0] == "u")
	{
		// u: unload all running applications.
		reset();
	}
	else if(args[0] == "lo")
	{
		// lo: list objects
		ReferenceType::printObjCounts();
	}
	else if(args[0] == "ln")
	{
		// ln: list nodes

		// ls is really just a shortcut for printChildren(getEngine().getScene(), <tree depth>)
		SystemManager* sys = SystemManager::instance();
		sys->getScriptInterpreter()->eval("printChildren(getEngine().getScene(), 10");
	}
	else if(args[0] == "q")
	{
		// q: quit
		oexit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<OmegaViewer> app("orun");
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);
	return omain(app, argc, argv);
}
