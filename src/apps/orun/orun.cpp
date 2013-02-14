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
 *-------------------------------------------------------------------------------------------------
 * orun
 *	A python script launcher and interpreter for omegalib applications.
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

#ifdef OMEGA_BUILD_OSG_LIB
using namespace cyclops;
#endif

// The name of the script to launch automatically at startup
String sDefaultScript = "";
// When set to true, add the script containing directory to the data search paths.
bool sAddScriptDirectoryToData = true;


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
	~OmegaViewer()
	{}

	virtual void initialize();
	void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);
	virtual void handleCommand(const String& cmd);

	AppDrawer* getAppDrawer() { return myAppDrawer; }

	void setAppDrawerToggleButton(Event::Flags value) { myAppDrawerToggleButton = value; }
	Event::Flags getAppDrawerToggleButton() { return myAppDrawerToggleButton; }

public:
	//! Start the selected application
	void run(const String& appName);
	//! Unload all running applications
	void reset();

private:
	Ref<UiModule> myUiModule;
	Ref<AppDrawer> myAppDrawer;
	Event::Flags myAppDrawerToggleButton;
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
		PYAPI_METHOD(OmegaViewer, getAppDrawerToggleButton)
		PYAPI_METHOD(OmegaViewer, setAppDrawerToggleButton)
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
		PYAPI_REF_GETTER(AppDrawer, getContainer)
		;

	// AppInfo
	PYAPI_REF_BASE_CLASS_WITH_CTOR(AppInfo)
		PYAPI_PROPERTY(AppInfo, name)
		PYAPI_PROPERTY(AppInfo, label)
		PYAPI_PROPERTY(AppInfo, iconFile)
		;

	def("getViewer", getViewer, PYAPI_RETURN_REF);
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

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
	myAppDrawerToggleButton = Event::Button3;

	gViewerInstance = this;

	myUiModule = NULL;
	// If I create t here, UiModule will be registered as a core module and won't be 
	// deallocated between application switches.
	//myUiModule = new UiModule();
	//ModuleServices::addModule(myUiModule);

	//myAppDrawer = new AppDrawer();
	myAppDrawer = NULL;
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

	// If a default script has been passed to orun, queue it's execution through the python
	// interpreter. Queuing it will make sure the script is launched on all nodes when running
	// in a cluster environment.
	if(sDefaultScript != "")
	{
		// Substitute the OMEGA_DATA_ROOT and OMEGA_APP_ROOT macros in the path.
		sDefaultScript = StringUtils::replaceAll(sDefaultScript, "OMEGA_DATA_ROOT", OMEGA_DATA_PATH);
#ifdef OMEGA_APPROOT_DIRECTORY
		sDefaultScript = StringUtils::replaceAll(sDefaultScript, "OMEGA_APP_ROOT", OMEGA_APPROOT_DIRECTORY);
#endif

		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		// If this flag is active, set the script containing folder as the default application data path.
		if(sAddScriptDirectoryToData)
		{
			String scriptPath;
			String baseScriptFilename;
			StringUtils::splitFilename(sDefaultScript, baseScriptFilename, scriptPath);

			interp->queueCommand(ostr("addDataPath(\"%1%\")", %scriptPath));
			interp->queueCommand(ostr("orun(\"%1%\")", %baseScriptFilename));
		}
		else
		{
			interp->queueCommand(ostr("orun(\"%1%\")", %sDefaultScript));
		}
	}

	if(myAppDrawer != NULL)	myAppDrawer->initialize();

	omsg("---------------------------------------------------------------------");
	omsg("Welcome to orun!");
	omsg("\tomegalib version " OMEGA_VERSION);
	omsg("\tTo get a list of quick commands type :?");
	omsg("\tType :? . to list all global symbols");
	omsg("\tType :? C to list all members of class or variable `C`");
	omsg("\t\texample :? SceneNode");
	omsg("\tType :? ./C [prefix] to list global symbols or object members starting with `prefix`");
	omsg("\t\texample :? . si");
	omsg("\t\texample :? SceneNode set");


	//Setting& base = SystemManager::instance()->getAppConfig()->lookup("config");
	//if(Config::getBoolValue("cyclopsEnabled", base, false))
	//{
	//	SceneManager* sm = SceneManager::createAndInitialize();
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::update(const UpdateContext& context)
{
	if(myAppDrawer != NULL) 
	{
		myAppDrawer->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::handleEvent(const Event& evt)
{
	// '`' key toggles app drawer.
	if(evt.isKeyDown(KC_HOME) || 
			evt.isButtonDown(myAppDrawerToggleButton))
	{
		if(myAppDrawer != NULL) 
		{
			if(myAppDrawer->isVisible()) myAppDrawer->hide();
			else  myAppDrawer->show();
		}
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
	String baseName;
	// If appName is a path, treat it differently than just an application name.
	if(appName.find('/') != String::npos)
	{
		baseName = appName;
	}
	else
	{
		baseName = appName + "/" + appName;
	}

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
		// NOTE: Instead of running the script immediately through PythonInterpreter::runFile, we queue a local orun command.
		// We do this to give the system a chance to finish reset, if this script is loading through a :r! command.
		// Also note how we explicitly import module omega, since all global symbols may have been unloaded by the previously mentioned reset command.
		sys->getScriptInterpreter()->queueCommand(ostr("from omega import *; orun('%1%')", %scriptName), true);
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
		SystemManager* sys = SystemManager::instance();
		PythonInterpreter* interp = sys->getScriptInterpreter();
		// ?: command help
		if(args.size() == 2)
		{
			// Print members of specified object
			if(args[1] == ".")	interp->eval("for m in [x for x in dir() if x[0] != \"_\"]: print(m)");
			else interp->eval(ostr("for m in [x for x in dir(%1%) if x[0] != \"_\"]: print(m)", %args[1]));
		}
		else if(args.size() == 3)
		{
			// Print members of specified object starting with a prefix
			if(args[1] == ".") interp->eval(ostr("for m in [x for x in dir() if x[0] != \"_\" and x.startswith('%1%')]: print(m)", %args[2]));
			else interp->eval(ostr("for m in [x for x in dir(%1%) if x[0] != \"_\" and x.startswith('%2%')]: print(m)", %args[1] %args[2]));
		}
		else
		{
			omsg("OmegaViewer");
			omsg("\t r <appName> - run the specified script application");
			omsg("\t lo          - list live objects");
			omsg("\t ln          - print the scene node tree");
			omsg("\t u           - unload all running applications");
			omsg("\t c           - toggle console");
			omsg("\t s		     - print statistics");
			omsg("\t w		     - toggle wand");
			omsg("\t f		     - toggle draw fps");
			omsg("\t porthole    - (experimental) enable porthole");
		}
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
	else if(args[0] == "c")
	{
		// c: toggle console
		bool isConsoleEnabled = getEngine()->isConsoleEnabled();
		getEngine()->setConsoleEnabled(!isConsoleEnabled);
	}
	else if(args[0] == "s")
	{
		// s: print statistics
		SystemManager::instance()->getStatsManager()->printStats();
	}
	else if(args[0] == "f")
	{
		// f: toggle raw fps
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		bool dfps = ds->isDrawFpsEnabled();
		ds->drawFps(!dfps);
	}
	else if(args[0] == "porthole")
	{
	
		// porthole: start the porthole server
		String xmlFile = "porthole/porthello.xml";
		String cssFile = "porthole/porthello.css";
		if(args.size() == 3)
		{
			xmlFile = args[1];
			cssFile = args[2];
		}
		PortholeService* service = PortholeService::createAndInitialize(4080,xmlFile, cssFile);
	}
	else if(args[0] == "q")
	{
		// q: quit
		SystemManager::instance()->postExitRequest();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	String applicationName = "orun";
	
	// Legacy default script (new apps should use launch script instead)
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);

	// If a start script is specified, use it to change the application name. This in turn allows for
	// loading of per-application config files
	// (i.e. orun -s apps/test.py will name the application apps/test and look for apps/test.cfg as the 
	// default configuration file)
	// if(sDefaultScript != "")
	// {
		// String extension;
		// StringUtils::splitBaseFilename(sDefaultScript, applicationName, extension);
	// }

	Application<OmegaViewer> app(applicationName);
	app.setExecutableName(argv[0]);

	// If a start script is specified, use it to change the application name. This in turn allows for
	// loading of per-application config files
	// (i.e. orun -s apps/test.py will name the application apps/test and look for apps/test.cfg as the 
	// default configuration file)
	//if(sDefaultScript != "")
	//{
	//	String extension;
	//	StringUtils::splitBaseFilename(sDefaultScript, applicationName, extension);
	//}

	return omain(app, argc, argv);
}
