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
#include <omegaVtk.h>

using namespace omega;
using namespace omegaToolkit;
using namespace omegaVtk;
using namespace cyclops;

// The name of the script to launch automatically at startup
String sDefaultScript = "";

///////////////////////////////////////////////////////////////////////////////////////////////////
class OmegaViewer: public ServerModule
{
public:
	OmegaViewer();

	virtual void initialize();
	virtual void handleEvent(const Event& evt);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::initialize()
{
#ifdef OMEGA_USE_PYTHON
	omegaVtkPythonApiInit();
	omegaToolkitPythonApiInit();
	cyclopsPythonApiInit();
#endif

	if(sDefaultScript != "")
	{
		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->queueInteractiveCommand(ostr("orun(\"%1%\")", %sDefaultScript));
	}

	Setting& base = SystemManager::instance()->getAppConfig()->lookup("config");
	if(Config::getBoolValue("cyclopsEnabled", base, false))
	{
		SceneManager* sm = new SceneManager();
		ModuleServices::addModule(sm);
		sm->doInitialize(getServer());
	}


	MenuManager* menuMng = new MenuManager();
	ModuleServices::addModule(menuMng);
	menuMng->doInitialize(getServer());
	Menu* menu = MenuManager::instance()->createMenu("menu");
	MenuItem* btn = menu->getRoot()->addItem(MenuItem::Button);
	btn = menu->getRoot()->addItem(MenuItem::Button);

	menuMng->setMainMenu(menu);
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
// Application entry point
int main(int argc, char** argv)
{
	Application<OmegaViewer> app("orun");
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);
	return omain(app, argc, argv);
}
