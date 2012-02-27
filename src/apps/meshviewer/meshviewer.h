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
#ifndef __MESHVIEWER_H__
#define __MESHVIEWER_H__

#include <omega.h>
#include <oengine.h>
#include "oengine/TabletManagerModule.h"

using namespace omega;
using namespace oengine;
using namespace oengine::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
class Entity: public DynamicObject
{
public:
	Entity(MeshData* data, EngineServer* server, Actor* interactor, const String& name, const String& label);
	~Entity();

	void show();
	void hide();
	
	SceneNode* getSceneNode() { return mySceneNode; }
	Mesh* getMesh() { return myMesh; }
	const String& getName() { return myName; }
	const String& getLabel() { return myLabel; }
	
private:
	EngineServer* myServer;
	SceneNode* mySceneNode;
	Mesh* myMesh;
	MeshData* myMeshData;
	Actor* myInteractor;
	String myName;
	String myLabel;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewer: public EngineServer
{
public:
	MeshViewer(Application* app): EngineServer(app) {}

	virtual void initialize();
	virtual void handleEvent(const Event& evt);
	virtual void handleUiEvent(const Event& evt);
	virtual void update(const UpdateContext& context);

private:
	void loadEntityLibrary();
	void initUi();

private:
	Vector<Entity*> myEntities;
	Entity* mySelectedEntity;

	TabletManagerModule* myTabletManager;

	// Scene
	ReferenceBox* myReferenceBox;

	// UI
	Vector<Button*> myEntityButtons;

	// Interactors.
	Actor* myInteractor;

    bool myShowUI;
   	bool autoRotate;
   	float deltaScale;
};

#endif
