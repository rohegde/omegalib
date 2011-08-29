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

#include <omega>
#include <oengine>

using namespace omega;
using namespace oengine;
using namespace oengine::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
class EntityData
{
public:
	EntityData():
	  meshData(NULL) {}

	String name;
	String label;
	MeshData* meshData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class Entity: public DynamicObject
{
public:
	Entity(EntityData* data, EngineClient* client);

	EntityData* getData() { return myData; }

	void resetTransform();
	bool isVisible() { return myVisible; }
	void setVisible(bool value);

	SceneNode* getSceneNode() { return mySceneNode; }

	Mesh* getMesh() { return myMesh; }
	Texture* getRightImage() { return myRightImage; }
	Texture* getLeftImage() { return myLeftImage; }

private:
	EntityData* myData;
	EngineClient* myClient;

	SceneNode* mySceneNode;
	Mesh* myMesh;
	BoundingSphere* mySelectionSphere;
	Texture* myLeftImage;
	Texture* myRightImage;
	bool myVisible;
};

class MeshViewerClient;
	
///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerServer: public ApplicationServer
{
public:
	typedef Dictionary<String, EntityData*> EntityDictionary;
public:
	MeshViewerServer(Application* app): ApplicationServer(app) {}

	virtual void initialize();
	void createEntities(MeshViewerClient* client);

private:
	EntityDictionary myEntities;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public EngineClient
{
public:
	MeshViewerClient(ApplicationServer* server): 
	  EngineClient(server), 
		myVisibleEntity(NULL)
	  {}

	virtual void initialize();
	void initUI();

	void addEntity(EntityData* ed);

	virtual void handleEvent(const Event& evt);
    void draw( const DrawContext& context);

	void handleUIEvent(const Event& evt);
	void setVisibleEntity(int entityId);
	void update(const UpdateContext& context);

private:
	// Entities
	Vector<Entity*> myEntities;
	Entity* myVisibleEntity;

	// Scene
	ReferenceBox* myReferenceBox;

	// UI
	Vector<Button*> myEntityButtons;

	// Interactors.
	Actor* myCurrentInteractor;

	Effect* myColorIdEffect;
    
    bool myShowUI;
   	bool autoRotate;
   	float deltaScale;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationServer* createServer() { return new MeshViewerServer(this); }
	virtual ApplicationClient* createClient(ApplicationServer* server) { return new MeshViewerClient(server); }
};

#endif
