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

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(MeshData* data, EngineServer* server, Actor* interactor, const String& name, const String& label):
	myMeshData(data),
	myServer(server),
	myName(name),
	myLabel(label),
	myInteractor(interactor)
{
	myMesh = new Mesh();
	mySceneNode = new SceneNode(server);
	mySceneNode->setSelectable(true);
	server->getScene(0)->addChild(mySceneNode);
	mySceneNode->addObject(myMesh);
	myMesh->setData(myMeshData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity::~Entity()
{
	myServer->getScene(0)->removeChild(mySceneNode);
	delete mySceneNode;
	mySceneNode = NULL;

	delete myMesh;
	myMesh = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::show()
{
	mySceneNode->setPosition(0, 0, 0.0f);
	mySceneNode->setScale( 1.0 , 1.0 , 1.0 );
	mySceneNode->resetOrientation();
	mySceneNode->setVisible(true);
	myInteractor->setSceneNode(mySceneNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::hide()
{
	mySceneNode->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::loadEntityLibrary()
{
	Config* cfg = SystemManager::instance()->getAppConfig();
	if(cfg->exists("config/entities"))
	{
		Setting& entities = cfg->lookup("config/entities");
		for(int i = 0; i < entities.getLength(); i++)
		{
			Setting& entitySetting = entities[i];

			if(entitySetting.exists("mesh"))
			{
				String meshFilename = String((const char*)entitySetting["mesh"]);
				MeshData* mesh = MeshUtils::load(meshFilename);
				
				if(mesh != NULL)
				{
					mesh->scale(0.8f);	
					String name = entitySetting.getName();
					String label = String((const char*)entitySetting["label"]);
					Entity* e = new Entity(mesh, this, myInteractor, name, label);
					myEntities.push_back(e);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::initialize()
{
	EngineServer::initialize();

	// Setup lighting
	Light* light = getLight(0);
	light->setEnabled(true);
	light->setColor(Color(0.5f, 0.5f, 0.5f));
	light->setPosition(Vector3f(0, 3, 3));
	setAmbientLightColor(Color::Black);
	
	// Create a reference box around the scene.
	Config* cfg = getSystemManager()->getAppConfig();
	if(cfg->exists("config/referenceBox"))
	{
		myReferenceBox = new ReferenceBox();
		getScene(0)->addObject(myReferenceBox);
		// Default box size for meshviewer, if left unspecified by config
		myReferenceBox->setSize(Vector3f(4.0f, 4.0f, 4.0f));

		// Read config for reference box.
		myReferenceBox->setup(cfg->lookup("config/referenceBox"));
	}
	
	// Set the interactor style used to manipulate meshes.
	String interactorStyle = cfg->lookup("config/interactorStyle");
	if(interactorStyle == "Mouse")
	{
		DefaultMouseInteractor* interactor = new DefaultMouseInteractor();
		interactor->setMoveButtonFlag(Event::Left);
		interactor->setRotateButtonFlag(Event::Right);
		myInteractor = interactor;
	}
	else if(interactorStyle == "Controller")
	{
		ControllerManipulator* interactor = new ControllerManipulator();
		myInteractor = interactor;
	}
	else
	{
		DefaultTwoHandsInteractor* interactor =  new DefaultTwoHandsInteractor();
		interactor->initialize("ObserverUpdateService");
		myInteractor = interactor;
	}
	addActor(myInteractor);
	
	// Load the entities specified in the configuration file.
	loadEntityLibrary();

	// Create and initialize the gui
	initUi();

    myShowUI = true;
    autoRotate = true;
	deltaScale = 0;
	
	// Display the first entity in the entity library.
	mySelectedEntity = myEntities[0];
	mySelectedEntity->show();

	// Get the default camera and focus in on the scene root (do we need this call?)
	Camera* cam = getDefaultCamera();
	cam->focusOn(getScene(0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::initUi()
{
	WidgetFactory* wf = getWidgetFactory();

	int canvasWidth = getCanvasWidth();
	int canvasHeight = getCanvasHeight();

	Container* root = getUi(0);

	Container* entityButtons = wf->createContainer("entities", root, Container::LayoutHorizontal);
	//entityButtons->setDebugModeEnabled(true);

	entityButtons->setPosition(Vector2f(5, 5));
	entityButtons->setSize(Vector2f(300, canvasHeight - 10));

	// Add buttons for each entity
	for(int i = 0; i < myEntities.size(); i++)
	{
		Entity* e = myEntities[i];
		Button* btn = wf->createButton(e->getName(), entityButtons);
		btn->setAutosize(true);
		myEntityButtons.push_back(btn);
	}

	Image* img = wf->createImage("img", root);
	mySecondaryViewData = new PixelData(PixelData::FormatRgba, 420, 240);
	img->setData(mySecondaryViewData);
	img->setAutoRefresh(true);
	img->setPosition(Vector2f(canvasWidth - 325, 0));
	img->setSize(Vector2f(420, 240));

	Camera* cam = createCamera(Camera::ForceMono | Camera::Offscreen | Camera::DrawScene);
	cam->setProjection(30, 1, 0.1f, 100);
	cam->setAutoAspect(true);
	cam->setPosition(Vector3f(0, 1, 0));
	Quaternion o = AngleAxis(-Math::HalfPi, Vector3f::UnitX());
	cam->setOrientation(o);
	cam->getOutput(0)->setReadbackTarget(mySecondaryViewData);
	cam->getOutput(0)->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::handleEvent(const Event& evt)
{
    EngineServer::handleEvent(evt);
	if(evt.getServiceType() == Service::Ui) 
	{
		handleUiEvent(evt);
	}
	else if( evt.getServiceType() == Service::Keyboard )
    {
		if(evt.isKeyDown('s')) 
        {
			if(myTablet != NULL)
			{
				myTablet->sendImage(mySecondaryViewData);
			}
        }
        if(evt.isKeyDown('r')) 
        {
            autoRotate = !autoRotate;
        }
    }
	else if( evt.getServiceType() == Service::Generic )
    {
		if(evt.getType() == Event::Connect)
		{
			TabletService* tsvc = getServiceManager()->getService<TabletService>(evt.getServiceId());
			myTablet = new TabletInterface(tsvc, evt.getSourceId());
			myTablet->beginGui();
			myTablet->addButton(0, "Button1", "Hello I'm a Button", "Click me!");
			myTablet->addButton(1, "Button2", "Hello I'm another Button", "Click me!");
			myTablet->addSlider(2, "Slider1", "Hello I'm a Slider! (0, 100, 50)", 0, 100, 50);
			myTablet->addSlider(3, "Slider2", "Hello I'm another Slider! (0, 10, 10)", 0, 10, 10);
			myTablet->addSwitch(4, "Switch1", "Hello I'm a Switch! (on)", true);
			myTablet->addSwitch(5, "Switch2", "Hello I'm another Switch! (off)", false);
			myTablet->finishGui();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::handleUiEvent(const Event& evt)
{
	for(int i = 0; i < myEntityButtons.size(); i++)
	{
		if(myEntityButtons[i]->getId() == evt.getSourceId())
		{
			Entity* e = myEntities[i];
			mySelectedEntity->hide();
			mySelectedEntity = e;
			mySelectedEntity->show();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::update(const UpdateContext& context)
{
	EngineServer::update( context );

	SceneNode* daSceneNode = myInteractor->getSceneNode();
	if(daSceneNode != NULL)
	{
		if ( autoRotate )
		{
			daSceneNode->yaw( 0.01f );
		}
	
		if( deltaScale != 0 )
		{
			// if it is negative 
			Vector3f curScale = daSceneNode->getScale( );
			daSceneNode->setScale( curScale + curScale * deltaScale );	
			deltaScale = 0.0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<MeshViewer> app;

	// Read config file name from command line or use default one.
	const char* cfgName = "meshviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "meshviewer.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
