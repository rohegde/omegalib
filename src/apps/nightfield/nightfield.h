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
#ifndef __NIGHTFIELD_H__
#define __NIGHTFIELD_H__

#include <omega.h>
#include <oengine.h>
#include "../common/Flock.h"
#include "oengine/TabletManagerModule.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
class AffectorEntity: public DynamicObject
{
public:
	AffectorEntity(SceneObject* obj, EngineServer* server);
	~AffectorEntity();

	void resetTransform();
	bool isVisible() { return myVisible; }
	void setVisible(bool value);

	void setup(const Setting& setting);
	void update(const UpdateContext& context);
	void handleEvent(const Event& evt);

	SceneNode* getSceneNode() { return mySceneNode; }
	SceneObject* getObject() { return myObject; }

	void updateFlockAffector(FlockAffector* af);
	void setEnabled(bool value) { myEnabled = value; }
	bool isEnabled() { return myEnabled; }

	bool isInteractive() { return myInteractive; }
	bool isRotating() { return myRotating; }
	void setRotating(bool value) { myRotating = value; }

	void select();
	void deselect();
	bool isSelected() { return mySelected; }

private:
	EngineServer* myServer;

	SceneNode* mySceneNode;
	SceneObject* myObject;

	BoundingSphere* mySelectionSphere;
	bool myVisible;
	bool myEnabled;
	bool myInteractive;
	bool mySelected;
	bool myRotating;
	float myRotationSpeed;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class Nightfield: public EngineServer
{
public:
	Nightfield(Application* app): EngineServer(app) {}
	virtual void initialize();
	void update(const UpdateContext& context);
	void handleEvent(const Event& evt);
	AffectorEntity* findEntity(SceneNode* node);
	void updateSelection(const Ray& ray);

private:
	Settings mySettings;

	ReferenceBox* myReferenceBox;
	SceneNode* mySceneNode;
	Flock* myFlock;
	Actor* myInteractor;

	AffectorEntity* mySelectedEntity;
	List<AffectorEntity*> myEntities;

	TabletManagerModule* myTabletManager;
};

#endif