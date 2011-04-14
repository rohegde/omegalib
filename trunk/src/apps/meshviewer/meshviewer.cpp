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
Entity::Entity(const String& name, SceneManager* sm, Mesh* m):
	myName(name),
	myMesh(m),
	myVisible(false),
	myActive(false)
{
	mySelectionSphere = new BoundingSphereDrawable();
	mySelectionSphere->setVisible(false);
	mySelectionSphere->setDrawOnSelected(true);

	mySceneNode = new SceneNode(sm);
	mySceneNode->addDrawable(myMesh);
	mySceneNode->addDrawable(mySelectionSphere);
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
	myEngine = new EngineClient(this);
	myEngine->initialize();

	Config* cfg = getSystemManager()->getAppConfig();

	// Setup the system font.
	if(cfg->exists("config/defaultFont"))
	{
		Setting& fontSetting = cfg->lookup("config/defaultFont");
		myEngine->getFontManager()->createFont("default", fontSetting["filename"], fontSetting["size"]);
	}

	// Load meshes specified in config file.
	MeshManager* mm = myEngine->getMeshManager();
	if(cfg->exists("config/meshes"))
	{
		Setting& meshes = cfg->lookup("config/meshes");
		for(int i = 0; i < meshes.getLength(); i++)
		{
			Setting& meshSetting = meshes[i];
			String meshName = meshSetting.getName();
			String meshFilename = meshSetting["filename"];
			String meshLabel = meshSetting["label"];

			mm->loadMesh(meshName, meshFilename, MeshManager::MeshFormatPly, 0.8f);
			Mesh* mesh = mm->getMesh(meshName);
			Entity* e = new Entity(meshLabel, myEngine->getSceneManager(), mesh);
			myEntities.push_back(e);
		}
	}

	// Create and initialize meshviewer UI
	myUI = new MeshViewerUI();
	myUI->initialize(this);

	// Create a reference box around the scene.
	myReferenceBox = new ReferenceBox();
	myEngine->getSceneManager()->getRootNode()->addDrawable(myReferenceBox);
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
	myEngine->getSceneManager()->addActor(myCurrentInteractor);
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
bool MeshViewerClient::handleEvent(const Event& evt, UpdateContext& context)
{
	return myEngine->handleEvent(evt, context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const Event& evt, DrawContext& context)
{
	return myEngine->handleEvent(evt, context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::update(const UpdateContext& context)
{
	myEngine->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::draw(const DrawContext& context)
{
	myEngine->draw(context, EngineClient::DrawScene | EngineClient::DrawUI);
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
