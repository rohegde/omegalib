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
#include "ovtk/VtkModel.h"
#include "ovtk/VtkViewClient.h"
#include "ovtk/VtkModule.h"
#include "ovtk/VtkRenderable.h"

using namespace omega;
using namespace ovtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkViewClient::VtkViewClient(View* owner, EngineClient* client):
	ViewClient(owner, client)
{
	SceneManager* sm = getEngine()->getSceneManager();
	mySceneNode = new SceneNode(sm);
	sm->getRootNode()->addChild(mySceneNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewClient::addActor(vtkProp3D* actor)
{
	SceneManager* sm = getEngine()->getSceneManager();
	SceneNode* sn = onew(SceneNode)(sm);
	mySceneNode->addChild(sn);
	VtkRenderable* vdw = onew(VtkRenderable)();
	vdw->setActor(actor);
	sn->addRenderable(vdw);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewClient::onModelStateChanged(Model* model)
{
	if(model->getState() == Model::StateLoaded)
	{
		VtkModel* vtkModel = (VtkModel*)model;

		VtkModule::instance()->setActiveClient(this);

		vtkModel->evalCommand("initializeView()");

		// All of this in VtkViewClient
		mySceneNode->update(true, false);
		const Sphere& bs = mySceneNode->getBoundingSphere();

		float scale = 0.1f / bs.getRadius();
		Vector3f center = bs.getCenter();

		// Move all the nodes so they are recentered wrt the root Vtk node.
		foreach(SceneNode::Child child, mySceneNode->getChildren())
		{
			child->setPosition(-center);
		}
		mySceneNode->scale(scale, scale, scale);

		//
		mySceneNode->update(true, false);
		mySceneNode->setBoundingBoxVisible(true);
	}
}
