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


///////////////////////////////////////////////////////////////////////////////////////////////////
class OmegaViewer: public EngineModule, IAppDrawerListener
{
public:
	OmegaViewer();
	~OmegaViewer()
	{}

	virtual void initialize();
	void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);
	virtual bool handleCommand(const String& cmd);

	String getAppStartCommand() { return myAppStartFunctionCall; }
	void setAppStartCommand(const String cmd) { myAppStartFunctionCall = cmd; }


	AppDrawer* getAppDrawer() { return myAppDrawer; }
	virtual void startApp(AppInfo* app);

	void setAppDrawerToggleButton(Event::Flags value) { myAppDrawerToggleButton = value; }
	Event::Flags getAppDrawerToggleButton() { return myAppDrawerToggleButton; }

private:
	Ref<AppDrawer> myAppDrawer;
	Ref<UiModule> myUi;
	Event::Flags myAppDrawerToggleButton;
	String myAppStartFunctionCall;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer* gViewerInstance = NULL;
OmegaViewer* getViewer() { return gViewerInstance; }

///////////////////////////////////////////////////////////////////////////////////////////////////
// Python wrapper code.
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(omegaViewer)
{
	// OmegaViewer
	PYAPI_REF_BASE_CLASS(OmegaViewer)
		PYAPI_METHOD(OmegaViewer, getAppDrawerToggleButton)
		PYAPI_METHOD(OmegaViewer, setAppDrawerToggleButton)
		PYAPI_METHOD(OmegaViewer, getAppStartCommand)
		PYAPI_METHOD(OmegaViewer, setAppStartCommand)
		PYAPI_REF_GETTER(OmegaViewer, getAppDrawer)
		;

	def("getViewer", getViewer, PYAPI_RETURN_REF);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
	myAppDrawerToggleButton = Event::Button3;

	gViewerInstance = this;

	// If I create t here, UiModule will be registered as a core module and won't be 
	// deallocated between application switches.
	myUi = new UiModule();
	ModuleServices::addModule(myUi);

	myAppDrawer = new AppDrawer(SystemManager::instance()->getScriptInterpreter(), myUi);
	myAppDrawer->setListener(this);
	//myAppDrawer = NULL;
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

	//
	String orunInitScriptName = "orun_init.py";
	myAppStartFunctionCall = "from omegaToolkit import *; _onAppStart()";

	Config* cfg = SystemManager::instance()->getAppConfig();
    if(cfg->exists("config/orun"))
    {
        Setting& s = cfg->lookup("config/orun");
		orunInitScriptName = Config::getStringValue("initScript", s, orunInitScriptName);
		myAppStartFunctionCall = Config::getStringValue("appStartFunction", s, myAppStartFunctionCall);
    }

	cfg = SystemManager::instance()->getSystemConfig();
    if(cfg->exists("config/appDrawer"))
    {
        Setting& s = cfg->lookup("config/appDrawer");
		myAppDrawerToggleButton = Event::parseButtonName(Config::getStringValue("appDrawerToggleButton", s, "Button3"));
    }

	// Initialize the python wrapper module for this class.
	initomegaViewer();

	// Run the init script.
	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->runFile(orunInitScriptName);
	interp->eval(myAppStartFunctionCall);

	// If a default script has been passed to orun, queue it's execution through the python
	// interpreter. Queuing it will make sure the script is launched on all nodes when running
	// in a cluster environment.
	if(sDefaultScript != "")
	{
		interp->queueCommand(ostr(":r %1%", %sDefaultScript));
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::startApp(AppInfo* app)
{
	SystemManager* sys = SystemManager::instance();
	PythonInterpreter* interp = sys->getScriptInterpreter();
	interp->queueCommand(myAppStartFunctionCall, true);
	interp->cleanRun(app->name);
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
	if(evt.isButtonDown(myAppDrawerToggleButton))
	{
		if(myAppDrawer != NULL) 
		{
			if(myAppDrawer->isVisible()) myAppDrawer->hide();
			else  myAppDrawer->show();
		}
	}
	if(myAppDrawer->isVisible())
	{
		myAppDrawer->handleEvent(evt);
		// When the app drawer is open, it consumes all events.
		evt.setProcessed();
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
bool OmegaViewer::handleCommand(const String& cmd)
{
	Vector<String> args = StringUtils::split(cmd);
	SystemManager* sys = SystemManager::instance();
	PythonInterpreter* interp = sys->getScriptInterpreter();
	if(args[0] == "?")
	{
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
		interp->runFile(args[1]);
		return true;
	}
	else if(args[0] == "r!" && args.size() > 1)
	{
		// r!: reset state and run application.
		interp->queueCommand(myAppStartFunctionCall, true);
		interp->cleanRun(args[1]);
		return true;
	}
	else if(args[0] == "u")
	{
		// u: unload all running applications.
		interp->queueCommand(myAppStartFunctionCall, true);
		interp->clean();
		return true;
	}
	else if(args[0] == "lo")
	{
		// lo: list objects
		ReferenceType::printObjCounts();
		return true;
	}
	else if(args[0] == "ln")
	{
		// ln: list nodes

		// ls is really just a shortcut for printChildren(getEngine().getScene(), <tree depth>)
		interp->eval("printChildren(getEngine().getScene(), 10");
		return true;
	}
	else if(args[0] == "c")
	{
		// c: toggle console
		bool isConsoleEnabled = getEngine()->isConsoleEnabled();
		getEngine()->setConsoleEnabled(!isConsoleEnabled);
		return true;
	}
	else if(args[0] == "s")
	{
		// s: print statistics
		SystemManager::instance()->getStatsManager()->printStats();
		return true;
	}
	else if(args[0] == "f")
	{
		// f: toggle raw fps
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		bool dfps = ds->isDrawFpsEnabled();
		ds->drawFps(!dfps);
		return true;
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
		return true;
	}
	else if(args[0] == "q")
	{
		// q: quit
		SystemManager::instance()->postExitRequest();
		return true;
	}
	else if(args[0] == "hint")
	{
		if(args[1] == "displayWand")
		{
			interp->queueCommand("getSceneManager().displayWand(0, 1)", true);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	String applicationName = "orun";
	
	// Legacy default script (new apps should use launch script instead)
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);

	Application<OmegaViewer> app(applicationName);
	app.setExecutableName(argv[0]);

	return omain(app, argc, argv);
}
