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
#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include "omega.h"
#include "omega/scene.h"
#include "omega/ui.h"
#include "omega/EngineClient.h"
#include "omega/Texture.h"
#include "omega/ObserverUpdateService.h"

#ifdef OMEGA_USE_OPENNI
#include "omega/OpenNIService.h"
#endif


using namespace omega;
using namespace omega::scene;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations.
class Entity;
class MeshViewerClient;

///////////////////////////////////////////////////////////////////////////////////////////////////
class Entity: public DynamicObject
{
public:
	enum Operation { Move, Scale, Rotate, Compound };

public:
	Entity(const String& name, SceneManager* sm, Mesh* m);

	const String& getName() { return myName; }

	void resetTransform();
	bool isVisible() { return myVisible; }
	void setVisible(bool value);

	SceneNode* getSceneNode() { return mySceneNode; }

private:
	String myName;
	SceneNode* mySceneNode;
	Mesh* myMesh;
	BoundingSphereDrawable* mySelectionSphere;

	bool myActive;
	bool myVisible;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerUI: public IUIEventHandler
{
public:
	MeshViewerUI(): myClient(NULL) {}

	void initialize(MeshViewerClient* client);
	void handleUIEvent(const UIEvent& evt);

private:
	MeshViewerClient* myClient;
	Vector<Button*> myEntityButtons;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public EngineClient
{
public:
	MeshViewerClient(Application* app): 
	  EngineClient(app), 
		myVisibleEntity(NULL)
	  {}

	virtual void initialize();

	void setVisibleEntity(int entityId);
	int getNumEntities() { return myEntities.size(); }
	Entity* getEntity(int entityId) { return myEntities[entityId]; }

private:
	// UI
	MeshViewerUI* myUI;

	// Entity list
	Vector<Entity*> myEntities;

	// Active entity.
	Entity* myVisibleEntity;

	ReferenceBox* myReferenceBox;

	// Interactors.
	Actor* myCurrentInteractor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MeshViewerClient(this); }
};

#endif