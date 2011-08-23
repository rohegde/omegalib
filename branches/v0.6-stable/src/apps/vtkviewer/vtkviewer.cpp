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
#include "vtkviewer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkViewerServer::VtkViewerServer(Application* app):
	ApplicationServer(app),
	myViewManager(NULL)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerServer::initialize()
{
	myViewManager = new ViewManager();
	myVtkModule = new VtkModule();

	myVtkModule->initialize(myViewManager);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerServer::setVisibleModel(int modelId)
{
	//VtkModel* ei = myEntityLibrary[entityId];

	//if(myVisibleEntity != NULL)
	//{
	//	myVtkModule->destroyEntity(myVisibleEntity);
	//	myVisibleEntity = NULL;
	//}

	//myVisibleEntity = myVtkModule->createEntity();
	//myVisibleEntity->loadScript(ei->getSource());

	//myCurrentInteractor->setSceneNode(myVisibleEntity->getSceneNode());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerClient::initialize()
{
	EngineClient::initialize();

	// Register this client with the view manager.
	VtkViewerServer* srv = (VtkViewerServer*)getServer();
	srv->getViewManager()->addClient(this);
	srv->getVtkModule()->initialize(this);

	Config* cfg = getSystemManager()->getAppConfig();

	// Setup the system font.
	if(cfg->exists("config/defaultFont"))
	{
		Setting& fontSetting = cfg->lookup("config/defaultFont");
		getFontManager()->createFont("default", fontSetting["filename"], fontSetting["size"]);
	}

	// Load entity library
	MeshManager* mm = getMeshManager();
	if(cfg->exists("config/entities"))
	{
		Setting& entities = cfg->lookup("config/entities");
		for(int i = 0; i < entities.getLength(); i++)
		{
			VtkModel* e = onew(VtkModel)();
			Setting& entitySetting = entities[i];
			e->setName(entitySetting.getName());
			e->setSource((String)((const char*)entitySetting["script"]));
			e->setLabel((String)((const char*)entitySetting["label"]));

			myEntityLibrary.push_back(e);
		}
	}

	initUI();

	// Create a reference box around the scene.
	myReferenceBox = new ReferenceBox();
	getSceneManager()->getRootNode()->addRenderable(myReferenceBox);
	myReferenceBox->setSize(Vector3f(4.0f, 4.0f, 4.0f));

	// Set the interactor style used to manipulate meshes.
	String interactorStyle = cfg->lookup("config/interactorStyle");
	if(interactorStyle == "Mouse")
	{
		DefaultMouseInteractor* interactor = new DefaultMouseInteractor();
		myCurrentInteractor = interactor;
	}
	else
	{
		DefaultTwoHandsInteractor* interactor = new DefaultTwoHandsInteractor();
		interactor->initialize("ObserverUpdateService");
		myCurrentInteractor = interactor;
	}
	getSceneManager()->addActor(myCurrentInteractor);

	Light* light = getSceneManager()->getLight(0);
	light->setEnabled(true);
	light->setPosition(Vector3f(0, 3, 0));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerClient::initUI()
{
	UIManager* ui = getUIManager();
	ui->setUIEventHandler(this);

	//! Load and set default font.
	FontManager* fm = getFontManager();
	Font* defaultFont = fm->getFont("default");
	ui->setDefaultFont(defaultFont);

	WidgetFactory* wf = ui->getWidgetFactory();
	Container* root = ui->getRootContainer(0);
	root->setLayout(Container::LayoutVertical);

	Container* entityButtons = wf->createContainer("entities", root, Container::LayoutHorizontal);

	// Setup ui layout using from config file sections.
	Config* cfg = SystemManager::instance()->getAppConfig();
	if(cfg->exists("config/ui/entityButtons"))
	{
		entityButtons->load(cfg->lookup("config/ui/entityButtons"));
	}
	if(cfg->exists("config/ui/root"))
	{
		root->load(cfg->lookup("config/ui/root"));
	}

	// Add buttons for each entity
	for(int i = 0; i < myEntityLibrary.size(); i++)
	{
		VtkModel* e = myEntityLibrary[i];
		Button* btn = wf->createButton(e->getLabel(), entityButtons);
		myEntityButtons.push_back(btn);
	}

	// If openNI service is available, add User manager panel to UI layer two (mapped to omegadesk control window)
	if(getServiceManager()->findService<Service>("OpenNIService") != NULL)
	{
		root = ui->getRootContainer(1);
		root->setLayout(Container::LayoutVertical);
		UserManagerPanel* ump = new UserManagerPanel(ui);
		ump->initialize(root, "OpenNIService", "ObserverUpdateService");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerClient::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::UI) 
	{
		handleUIEvent(evt);
	}
	else
	{
		EngineClient::handleEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerClient::handleUIEvent(const Event& evt)
{
	for(int i = 0; i < myEntityLibrary.size(); i++)
	{
		if(myEntityButtons[i]->getId() == evt.getSourceId())
		{
			VtkViewerServer* srv = (VtkViewerServer*)getServer();
			srv->setVisibleModel(i);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	VtkViewerApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "vtkviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "vtkviewer.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
