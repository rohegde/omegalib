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
void MeshViewerClient::initialize()
{
	myEngine = new EngineClient(this);
	myEngine->initialize();

	myEngine->getFontManager()->createFont("arial", "fonts/arial.ttf", 30);

	MeshManager* mm = myEngine->getMeshManager();
	mm->loadMesh("screwdriver", "meshes/screwdriver.ply", MeshManager::MeshFormatPly, true);
	mm->loadMesh("arm", "meshes/rockerArm.ply", MeshManager::MeshFormatPly, true);
	mm->loadMesh("bonney", "meshes/bonney.ply", MeshManager::MeshFormatPly, true);
	mm->loadMesh("glacier", "meshes/glacier.ply", MeshManager::MeshFormatPly, true);

	Mesh* mesh1 = mm->getMesh("screwdriver");
	Mesh* mesh2 = mm->getMesh("arm");
	Mesh* mesh3 = mm->getMesh("bonney");
	Mesh* mesh4 = mm->getMesh("glacier");

	myEntities.push_back(new Entity("Screwdriver", myEngine->getSceneManager(), mesh1));
	myEntities.push_back(new Entity("Rocker Arm", myEngine->getSceneManager(), mesh2));
	myEntities.push_back(new Entity("Bonney", myEngine->getSceneManager(), mesh3));
	myEntities.push_back(new Entity("Glacier", myEngine->getSceneManager(), mesh4));

	// Create and initialize meshviewer UI
	myUI = new MeshViewerUI();
	myUI->initialize(this);
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::processPointerEvent(const InputEvent& evt)
{
	if(evt.type == InputEvent::Down)
	{
		// Select objects.
		Ray ray = unproject(Vector2f(evt.position[0], evt.position[1]));

		if(myVisibleEntity != NULL)
		{
			Vector3f handlePos;
			if(myVisibleEntity->hit(ray, &handlePos))
			{
				myVisibleEntity->activate(handlePos);
			}
		}
	}
	else if(evt.type == InputEvent::Up)
	{
		// Deselect objects.
		if(myVisibleEntity != NULL)
		{
			myVisibleEntity->deactivate();
		}
	}
	else if(evt.type == InputEvent::Move)
	{
		// Manipulate object, if one is active.
		if(myVisibleEntity != NULL)
		{
			Ray ray = unproject(Vector2f(evt.position[0], evt.position[1]));

			if(evt.isFlagSet(InputEvent::Left))
			{
				myVisibleEntity->manipulate(Entity::Move, ray);
			}
			else if(evt.isFlagSet(InputEvent::Right))
			{
				myVisibleEntity->manipulate(Entity::Rotate, ray);
			}
			else if(evt.isFlagSet(InputEvent::Middle))
			{
				myVisibleEntity->manipulate(Entity::Scale, ray);
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::processObserverEvent(const InputEvent& evt)
{
	if(evt.position.length() > 0.1f)
	{
		Observer* o = SystemManager::instance()->getDisplaySystem()->getObserver(0);
		o->update(evt.position, Quaternion(evt.rotation[0], evt.rotation[1], evt.rotation[2], evt.rotation[3]));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const InputEvent& evt)
{
	myEngine->handleEvent(evt);

	switch(evt.serviceType)
	{
	case InputService::Pointer:
		processPointerEvent(evt);
		break;
	case InputService::Touch:
	break;
	case InputService::Mocap:
		// Update observer
		if(evt.sourceId == 1) processObserverEvent(evt);
	break;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::update(const UpdateContext& context)
{
	myEngine->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::draw(const DrawContext& context)
{
	switch(context.layer)
	{
	case 0:
		myEngine->draw(context, EngineClient::DrawScene | EngineClient::DrawUI);
		break;
	case 1:
		myEngine->draw(context, EngineClient::DrawScene);
		break;
	case 2:
		myEngine->draw(context, EngineClient::DrawUI);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
void main(int argc, char** argv)
{
	MeshViewerApplication app;
	omain(app, argv[1], "meshviewer.log", new FilesystemDataSource("../../data/"));
}
