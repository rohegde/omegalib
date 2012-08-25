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

///////////////////////////////////////////////////////////////////////////////////////////////////
class OmegaViewer: public EngineModule
{
public:
	OmegaViewer();

	virtual void initialize();
	virtual void handleEvent(const Event& evt);
	virtual void handleCommand(const String& cmd);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
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

	if(sDefaultScript != "")
	{
		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->queueInteractiveCommand(ostr("orun(\"%1%\")", %sDefaultScript));
	}

	//Setting& base = SystemManager::instance()->getAppConfig()->lookup("config");
	//if(Config::getBoolValue("cyclopsEnabled", base, false))
	//{
	//	SceneManager* sm = SceneManager::createAndInitialize();
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::handleEvent(const Event& evt)
{
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
		String baseName = args[1] + "/" + args[1];
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
	else if(args[0] == "u")
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
	//ImageUtils::preallocateBlocks(8192 * 4096 * 3, 2);
	//ImageUtils::preallocateBlocks(8192 * 4096 * 3, 2);
	//Skybox::setPreallocBlock(0, DrawContext::EyeCyclop);
	//Skybox::setPreallocBlock(0, DrawContext::EyeLeft);
	//Skybox::setPreallocBlock(1, DrawContext::EyeRight);

	Application<OmegaViewer> app("orun");
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);
	return omain(app, argc, argv);
}
