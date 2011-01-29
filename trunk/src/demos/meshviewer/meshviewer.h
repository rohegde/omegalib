/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include "omega.h"
#include "omega/scene.h"
#include "omega/ui.h"

#include "omega/Plane.h"
#include "omega/Ray.h"

using namespace omega;
using namespace omega::scene;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations.
class Entity;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SelectionSphere: public Drawable
{
public:
	SelectionSphere(Entity* e): myEntity(e) {}

	void draw(SceneNode* node);

	void setVisible(bool value) { myVisible = value; }
	bool isVisisble() { return myVisible; }

private:
	Entity* myEntity;
	bool myVisible;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity
{
public:
	enum Operation { Move, Scale, Rotate, Compound };

public:
	Entity(SceneManager* sm, Mesh* m, const Vector3f& position);

	bool hit(const Ray& ray, Vector3f* handlePos);
	void manipulate(Operation op, const Ray& ray1, const Ray& ray2 = Ray(Vector3f::ZERO, Vector3f::ZERO));

	void activate(const Vector3f handlePos);
	void deactivate();
	bool isActive() { return myActive; }

	Vector3f getHandlePosition() { return myHandlePosition; }

private:
	SceneNode* mySceneNode;
	Mesh* myMesh;
	SelectionSphere* mySelectionSphere;

	// Interaction stuff
	Vector3f myHandlePosition;
	Sphere myStartBSphere;
	Quaternion myStartOrientation;
	float myStartScale;
	bool myActive;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public ApplicationClient, IUIEventHandler
{
public:
	MeshViewerClient(Application* app): ApplicationClient(app), myGpu(NULL), myActiveEntity(NULL) {}

	virtual void initialize();
	virtual bool handleEvent(const InputEvent& evt);
	virtual void handleUIEvent(const UIEvent& evt);
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	void addEntity(Mesh* m, const Vector3f& position);

private:
	// Managers
	GpuManager* myGpu;
	TextureManager* myTexMng;
	FontManager* myFontMng;
	SceneManager* mySceneManager;
	MeshManager* myMeshManager;
	EffectManager* myEffectManager;
	UIManager myUI;

	// Entity list
	std::list<Entity*> myEntities;

	// Active entity.
	Entity* myActiveEntity;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MeshViewerClient(this); }
};

#endif