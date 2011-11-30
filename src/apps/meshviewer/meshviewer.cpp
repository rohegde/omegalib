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
Entity::Entity(EntityData* data, EngineServer* server):
	myData(data),
	myServer(server),
	myVisible(false)
{
	myMesh = new Mesh();
	mySelectionSphere = new BoundingSphere();
	mySelectionSphere->setDrawOnSelected(true);
	mySelectionSphere->setVisible(false);

	mySceneNode = new SceneNode(server);
	mySceneNode->setSelectable(true);
	server->getScene(0)->addChild(mySceneNode);
	mySceneNode->addObject(myMesh);
	mySceneNode->addObject(mySelectionSphere);
	myMesh->setData(myData->meshData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity::~Entity()
{
	myServer->getScene(0)->removeChild(mySceneNode);
	delete mySceneNode;
	mySceneNode = NULL;

	delete mySelectionSphere;
	mySelectionSphere = NULL;

	delete myMesh;
	myMesh = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setVisible(bool value)
{
	myVisible = value;
	if(myMesh != NULL)
	{
		mySceneNode->setVisible(value);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::resetTransform()
{
	mySceneNode->setPosition(0, 0, 0.0f);
	mySceneNode->setScale( 1.0 , 1.0 , 1.0 );
	mySceneNode->resetOrientation();
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

			EntityData* ed = new EntityData();

			ed->name = entitySetting.getName();
			ed->label = String((const char*)entitySetting["label"]);

			if(entitySetting.exists("mesh"))
			{
				String meshFilename = String((const char*)entitySetting["mesh"]);

				if(StringUtils::endsWith(meshFilename, "ply"))
				{
					PlyDataReader* reader = new PlyDataReader();
					if(!reader->readPlyFile(meshFilename))
					{
						ofwarn("Could not load mesh file %1%.", %meshFilename);
					}
					else
					{
						reader->scale(0.8f);
						ed->meshData = reader;
					}
				}
				else if(StringUtils::endsWith(meshFilename, "obj"))
				{
					// ObjDataReader* reader = new ObjDataReader();
					// if(!reader->readFile(meshFilename))
					// {
						// ofwarn("Could not load mesh file %1%.", %meshFilename);
					// }
					// else
					// {
						// reader->scale(0.8f);
						// ed->meshData = reader;
					// }
				}
				else
				{
					ofwarn("%1%: unsupported file format.", %meshFilename);
				}
			}

			myEntityLibrary.push_back(ed);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::initialize()
{
	EngineServer::initialize();

	//myColorIdEffect = new Effect(getEffectManager());
	//myColorIdEffect->setDiffuseColor(Color::getColorByIndex(getId()));
	//myColorIdEffect->setForcedDiffuseColor(true);

	loadEntityLibrary();

	Config* cfg = getSystemManager()->getAppConfig();

	// Setup the system font.
	if(cfg->exists("config/defaultFont"))
	{
		Setting& fontSetting = cfg->lookup("config/defaultFont");
		setDefaultFont(FontInfo("default", fontSetting["filename"], fontSetting["size"]));
	}

	// Create and initialize meshviewer UI
	initUi();

	setAmbientLightColor(Color::Black);

	Light* light = getLight(0);
	light->setEnabled(true);
	light->setColor(Color(0.5f, 0.5f, 0.5f));
	light->setPosition(Vector3f(0, 3, 3));

	light = getLight(1);
	light->setEnabled(true);
	light->setColor(Color(0.3f, 0.35f, 0.3f));
	light->setPosition(Vector3f(-3, 0, 0));

	light = getLight(2);
	light->setEnabled(true);
	light->setColor(Color(0.3f, 0.35f, 0.3f));
	light->setPosition(Vector3f(3, 0, 0));

	light = getLight(3);
	light->setEnabled(true);
	light->setColor(Color(0.3f, 0.3f, 0.35f));
	light->setPosition(Vector3f(0, -3, 0));

	light = getLight(4);
	light->setEnabled(true);
	light->setColor(Color(0.35f, 0.3f, 0.3f));
	light->setPosition(Vector3f(0, 0, -3));

	// Create a reference box around the scene.
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
		interactor->setMoveButtonFlag(Event::Right);
		interactor->setRotateButtonFlag(Event::Middle);
		myInteractor = interactor;
	}
	else if(interactorStyle == "Controller")
	{
		ControllerManipulator* interactor = new ControllerManipulator();
		//interactor->setMoveButtonFlag(Event::Right);
		//interactor->setRotateButtonFlag(Event::Middle);
		myInteractor = interactor;
	}
	else
	{
		DefaultTwoHandsInteractor* interactor =  new DefaultTwoHandsInteractor();
		interactor->initialize("ObserverUpdateService");
		myInteractor = interactor;
	}
	
	addActor(myInteractor);

    myShowUI = true;
    autoRotate = true;
	deltaScale = 0;

	createEntity(myEntityLibrary[0]);

	Camera* cam = getDefaultCamera();
	cam->focusOn(getScene(0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::initUi()
{
	WidgetFactory* wf = getWidgetFactory();

	Container* root = getUi(0);
	root->setUIEventHandler(this);
	root->setLayout(Container::LayoutFree);

	Container* entityButtons = wf->createContainer("entities", root, Container::LayoutHorizontal);

	// Setup ui layout using from config file sections.
	Config* cfg = SystemManager::instance()->getAppConfig();
	if(cfg->exists("config/ui/entityButtons"))
	{
		entityButtons->load(cfg->lookup("config/ui/entityButtons"));
	}
	//if(cfg->exists("config/ui/root"))
	//{
	//	root->load(cfg->lookup("config/ui/root"));
	//}

	entityButtons->setPosition(Vector2f(0, 0));
	entityButtons->setSize(Vector2f(300, 400));
	entityButtons->setDebugModeEnabled(true);

	// Add buttons for each entity
	for(int i = 0; i < myEntityLibrary.size(); i++)
	{
		EntityData* ed = myEntityLibrary[i];
		Button* btn = wf->createButton(ed->name, entityButtons);
		btn->setAutosize(true);
		myEntityButtons.push_back(btn);
	}

	Image* img = wf->createImage("img", root);
	PixelData* pd = new PixelData(PixelData::FormatRgba, 320, 320);
	img->setData(pd);
	img->setAutoRefresh(true);
	img->setPosition(Vector2f(400, 0));
	img->setSize(Vector2f(320, 320));
	img->setDebugModeEnabled(true);

	Camera* cam = createCamera(Camera::ForceMono | Camera::Offscreen | Camera::DefaultFlags);
	cam->setProjection(90, 1, 0.1f, 100);
	cam->setAutoAspect(true);
	cam->setPosition(Vector3f(0, 0, 2));
	cam->getOutput(0)->setReadbackTarget(pd);
	cam->getOutput(0)->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::createEntity(EntityData* ed)
{
	Entity* e = new Entity(ed, this);
	//e->getMesh()->setEffect(myColorIdEffect);
	myEntities.push_back(e);
	myInteractor->setSceneNode(e->getSceneNode());
	mySelectedEntity = e;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::destroyEntity(Entity* e)
{
	myEntities.remove(e);
	delete e;
	myInteractor->setSceneNode(NULL);
	if(mySelectedEntity == e) mySelectedEntity = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::handleEvent(const Event& evt)
{
    EngineServer::handleEvent(evt);
	if(evt.getServiceType() == Service::Pointer) 
	{
		if(evt.getType() == Event::Down && evt.isFlagSet(Event::Middle))
		{
			if(mySelectedEntity != NULL)
			{
				destroyEntity(mySelectedEntity);
			}
		}
		else if(evt.getType() == Event::Down && evt.getExtraDataLength() == 2)
		{
			Ray ray;
			ray.setOrigin(evt.getExtraDataVector3(0));
			ray.setDirection(evt.getExtraDataVector3(1));
			updateSelection(ray);
		}
	}
	if(evt.getServiceType() == Service::Ui) 
	{
		handleUiEvent(evt);
	}
	else if( evt.getServiceType() == Service::Keyboard )
    {
        if((char)evt.getSourceId() == 'e' && evt.getType() == Event::Down) 
        {
			if(mySelectedEntity != NULL)
			{
				destroyEntity(mySelectedEntity);
			}
        }
        if((char)evt.getSourceId() == 's' && evt.getType() == Event::Down) 
        {
            myShowUI = !myShowUI;
        }
        if((char)evt.getSourceId() == 'r' && evt.getType() == Event::Down) 
        {
            autoRotate = !autoRotate;
        }
        else if((char)evt.getSourceId() == 'h' && evt.getType() == Event::Down) 
        {
			if(mySelectedEntity != NULL)
			{
				mySelectedEntity->resetTransform();
			}
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
			EntityData* ed = myEntityLibrary[i];
			createEntity(ed);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity* MeshViewer::findEntity(SceneNode* node)
{
	foreach(Entity* e, myEntities)
	{
		if(e->getSceneNode() == node) return e;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewer::updateSelection(const Ray& ray)
{
	const SceneQueryResultList& sqrl = querySceneRay(0, ray);
	if(sqrl.size() != 0)
	{
		// The ray intersected with something.
		SceneNode* sn = sqrl.front().node;
		Entity* e = findEntity(sn);

		if(mySelectedEntity != e)
		{
			if(mySelectedEntity != NULL)
			{
				mySelectedEntity->getSceneNode()->setSelected(false);
			}
			// The selected entity changed.
			myInteractor->setSceneNode(sn);
			sn->setSelected(true);
			mySelectedEntity = e;
		}
	}
	else
	{
		if(mySelectedEntity != NULL)
		{
			mySelectedEntity->getSceneNode()->setSelected(false);
			mySelectedEntity = NULL;
			myInteractor->setSceneNode(NULL);
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
