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
#include "meshviewer.h"
#include "omega/ImageUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(const String& name, SceneManager* sm, Mesh* m, Texture* leftImage, Texture* rightImage):
	myName(name),
	myMesh(m),
	myLeftImage(leftImage),
	myRightImage(rightImage),
	myVisible(false)
{
	mySelectionSphere = onew(BoundingSphere)();
	mySelectionSphere->setVisible(false);
	mySelectionSphere->setDrawOnSelected(true);

	mySceneNode = onew(SceneNode)(sm);
	mySceneNode->addRenderable(myMesh);
	mySceneNode->addRenderable(mySelectionSphere);
	mySceneNode->setPosition(Vector3f::Zero());
	mySceneNode->setSelectable(true);
	mySceneNode->setVisible(false);
	sm->getRootNode()->addChild(mySceneNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setVisible(bool value)
{
	myVisible = value;
	mySceneNode->setVisible(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::resetTransform()
{
	mySceneNode->setPosition(0, 0, 0.0f);
	mySceneNode->resetOrientation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initialize()
{
	EngineClient::initialize();

	Config* cfg = getSystemManager()->getAppConfig();

	// Setup the system font.
	if(cfg->exists("config/defaultFont"))
	{
		Setting& fontSetting = cfg->lookup("config/defaultFont");
		getFontManager()->createFont("default", fontSetting["filename"], fontSetting["size"]);
	}

	// Load meshes specified in config file.
	MeshManager* mm = getMeshManager();
	if(cfg->exists("config/entities"))
	{
		Setting& meshes = cfg->lookup("config/entities");
		for(int i = 0; i < meshes.getLength(); i++)
		{
			Setting& entitySetting = meshes[i];
			String name = entitySetting.getName();
			String label = entitySetting["label"];

			Mesh* mesh = NULL;
			if(entitySetting.exists("mesh"))
			{
				String meshFilename = entitySetting["mesh"];
				mm->loadMesh(name, meshFilename, MeshManager::MeshFormatPly, 0.8f);
				mesh = mm->getMesh(name);
			}

			Texture* leftImage = NULL;
			Texture* rightImage = NULL;
			if(entitySetting.exists("leftImage") && entitySetting.exists("rightImage"))
			{
				String leftImageFilename = entitySetting["leftImage"];
				String rightImageFilename = entitySetting["rightImage"];

				leftImage = ImageUtils::createTexture(getTextureManager(), name, leftImageFilename);
				rightImage = ImageUtils::createTexture(getTextureManager(), name, rightImageFilename);
			}

			Entity* e = new Entity(label, getSceneManager(), mesh, leftImage, rightImage);
			myEntities.push_back(e);
		}
	}

	// Create and initialize meshviewer UI
	initUI();

	// Create a reference box around the scene.
	if(cfg->exists("config/referenceBox"))
	{
		myReferenceBox = new ReferenceBox();
		getSceneManager()->getRootNode()->addRenderable(myReferenceBox);
		myReferenceBox->setSize(Vector3f(4.0f, 4.0f, 4.0f));
	}

	// Set the interactor style used to manipulate meshes.
	String interactorStyle = cfg->lookup("config/interactorStyle");
	if(interactorStyle == "Mouse")
	{
		DefaultMouseInteractor* interactor = onew(DefaultMouseInteractor)();
		myCurrentInteractor = interactor;
	}
	else
	{
		DefaultTwoHandsInteractor* interactor = onew(DefaultTwoHandsInteractor)();
		interactor->initialize("ObserverUpdateService");
		myCurrentInteractor = interactor;
	}
	getSceneManager()->addActor(myCurrentInteractor);
	
	Texture* leftImage = ImageUtils::createTexture(getTextureManager(), "leftNut", "data/images/SET_Chart_LEFT_1280X1024_Dispersion.jpg");
	Texture* rightImage = ImageUtils::createTexture(getTextureManager(), "rightNut", "data/images/SET_Chart_RIGHT_1280X1024_Dispersion.jpg");
	setTextureBackgroundEnabled( true );
	setTextureBackground( DrawContext::EyeLeft , leftImage );
	setTextureBackground( DrawContext::EyeRight , rightImage );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initUI()
{
	UIManager* ui = getUIManager();
	ui->setEventHandler(this);

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
	for(int i = 0; i < myEntities.size(); i++)
	{
		Entity* e = myEntities[i];
		Button* btn = wf->createButton(e->getName(), entityButtons);
		myEntityButtons.push_back(btn);
	}

	// If openNI service is available, add User manager panel to UI layer two (mapped to omegadesk control window)
	if(getServiceManager()->findService<Service>("OpenNIService") != NULL)
	{
		root = ui->getRootContainer(1);
		root->setLayout(Container::LayoutVertical);
		UserManagerPanel* ump = new UserManagerPanel("userManagerPanel");
		ump->initialize(root, "OpenNIService", "ObserverUpdateService");
	}

	getSceneManager()->setAmbientLightColor(Color::Black);

	Light* light = getSceneManager()->getLight(0);
	light->setEnabled(true);
	light->setColor(Color(0.5f, 0.5f, 0.5f));
	light->setPosition(Vector3f(0, 3, 3));

	light = getSceneManager()->getLight(1);
	light->setEnabled(true);
	light->setColor(Color(0.3f, 0.35f, 0.3f));
	light->setPosition(Vector3f(-3, 0, 0));

	light = getSceneManager()->getLight(2);
	light->setEnabled(true);
	light->setColor(Color(0.3f, 0.35f, 0.3f));
	light->setPosition(Vector3f(3, 0, 0));

	light = getSceneManager()->getLight(3);
	light->setEnabled(true);
	light->setColor(Color(0.3f, 0.3f, 0.35f));
	light->setPosition(Vector3f(0, -3, 0));

	light = getSceneManager()->getLight(4);
	light->setEnabled(true);
	light->setColor(Color(0.35f, 0.3f, 0.3f));
	light->setPosition(Vector3f(0, 0, -3));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::handleUIEvent(const UIEvent& evt)
{
	for(int i = 0; i < myEntities.size(); i++)
	{
		if(myEntityButtons[i] == evt.source)
		{
			setVisibleEntity(i);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::setVisibleEntity(int entityId)
{
	if(myVisibleEntity != NULL)
	{
		myVisibleEntity->setVisible(false);
		myVisibleEntity = NULL;
	}

	Entity* e = myEntities[entityId];
	myVisibleEntity = e;
	myVisibleEntity->resetTransform();
	myVisibleEntity->setVisible(true);

	// Tell the interactor what is the currently active scene node
	myCurrentInteractor->setSceneNode(e->getSceneNode());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	MeshViewerApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "meshviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "meshviewer.log", new FilesystemDataSource("./../../data/"));

	return 0;
}
