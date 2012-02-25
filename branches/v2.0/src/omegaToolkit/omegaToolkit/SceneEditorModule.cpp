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
#include "omegaToolkit/SceneEditorModule.h"
#include "omega/SceneQuery.h"
#include "omegaToolkit/DefaultMouseInteractor.h"
#include "omegaToolkit/ControllerManipulator.h"
#include "omegaToolkit/BoundingSphere.h"

using namespace omegaToolkit;
using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
EditableObject::EditableObject(SceneNode* node, SceneEditorModule* editor):
	mySceneNode(node), myEditor(editor)
{
	mySceneNode = node;

	mySelectionSphere = new BoundingSphere();
	mySelectionSphere->setDrawOnSelected(false);
	mySelectionSphere->setVisible(true);
	mySceneNode->addObject(mySelectionSphere);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneEditorModule::SceneEditorModule():
	myEngine(NULL), myInteractor(NULL), mySelectedObject(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneEditorModule::~SceneEditorModule()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EditableObject* SceneEditorModule::findEditableObject(SceneNode* node)
{
	foreach(EditableObject* eo, myObjects)
	{
		if(eo->getSceneNode() == node) return eo;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::addNode(SceneNode* node)
{
	EditableObject* eo = new EditableObject(node, this);
	myObjects.push_back(eo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::removeNode(SceneNode* node)
{
	EditableObject* eo = findEditableObject(node); 
	if(eo)
	{
		myObjects.remove(eo);
		delete eo;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* SceneEditorModule::getSelectedNode()
{
	if(mySelectedObject != NULL) return mySelectedObject->getSceneNode();
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::setInteractorStyle(InteractorStyle style)
{
	myInteractorStyle = style;
	if(myInteractorStyle == MouseInteractorStyle)
	{
		DefaultMouseInteractor* interactor = new DefaultMouseInteractor();
		interactor->setMoveButtonFlag(Event::Left);
		interactor->setRotateButtonFlag(Event::Right);
		myInteractor = interactor;
	}
	else if(myInteractorStyle == ControllerInteractorStyle)
	{
		ControllerManipulator* interactor = new ControllerManipulator();
		myInteractor = interactor;
	}
	
	myEngine->addActor(myInteractor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::updateSelection(const Ray& ray)
{
	const SceneQueryResultList& sqrl = myEngine->querySceneRay(0, ray);
	if(sqrl.size() != 0)
	{
		// The ray intersected with something.
		SceneNode* sn = sqrl.front().node;
		EditableObject* e = findEditableObject(sn);

		if(mySelectedObject != e)
		{
			if(mySelectedObject != NULL)
			{
				mySelectedObject->getSceneNode()->setSelected(false);
			}
			// The selected entity changed.
			myInteractor->setSceneNode(sn);
			sn->setSelected(true);
			mySelectedObject = e;
		}
	}
	else
	{
		if(mySelectedObject != NULL)
		{
			mySelectedObject->getSceneNode()->setSelected(false);
			mySelectedObject = NULL;
			myInteractor->setSceneNode(NULL);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::initialize(ServerEngine* server)
{
	omsg("SceneEditorModule initializing...");

	myEngine = server;
	setInteractorStyle(MouseInteractorStyle);

	omsg("SceneEditorModule initialization OK");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneEditorModule::handleEvent(const Event& evt)
{
	if(evt.getServiceType() == Service::Pointer) 
	{
		if(evt.getType() == Event::Down && evt.getExtraDataLength() == 2)
		{
			Ray ray;
			ray.setOrigin(evt.getExtraDataVector3(0));
			ray.setDirection(evt.getExtraDataVector3(1));
			updateSelection(ray);
		}
	}
}
