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
#include "ovtk/VtkEntity.h"
#include "ovtk/VtkRenderable.h"
#include "ovtk/VtkClient.h"

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkMapper.h>

using namespace ovtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkEntity::VtkEntity(VtkClient* client)
{
	// Default representation size: 10cm.
	myRepresentationSize = 0.1f;

	myClient = client;

	SceneManager* sm = myClient->getEngine()->getSceneManager();
	mySceneNode = onew(SceneNode)(sm);
	sm->getRootNode()->addChild(mySceneNode);

	myBSphere = onew(BoundingSphere)();
	myBSphere->setVisible(false);
	myBSphere->setDrawOnSelected(true);
	mySceneNode->addRenderable(myBSphere);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkEntity::~VtkEntity()
{
	VectorIterator<List<VtkRenderable*> > dit(myRenderables);
	while(dit.hasMoreElements()) 
	{
		VtkRenderable* d = dit.getNext();
		odelete(d);
	}

	SceneNode::ChildNodeIterator it = mySceneNode->getChildIterator();
	while(it.hasMoreElements()) 
	{
		SceneNode* sn = (SceneNode*)it.getNext();
		odelete(sn);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkEntity::loadScript(const String& filename)
{
	myClient->setActiveEntity(this);
	myClient->runScript(filename);

	mySceneNode->update(true, false);
	const Sphere& bs = mySceneNode->getBoundingSphere();

	float scale = 0.1f / bs.getRadius();
	Vector3f center = bs.getCenter();

	// Move all the nodes so they are recentered wrt the root Vtk node.
	Node::ChildNodeIterator it = mySceneNode->getChildIterator();
	while(it.hasMoreElements())
	{
		Node* child = it.getNext();
		child->setPosition(-center);
	}

	mySceneNode->scale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkEntity::addActor(vtkActor* actor)
{
	//actor->GetProperty()->SetAmbient(0.1f);
	//actor->GetProperty()->SetDiffuse(0.2f);

	SceneManager* sm = myClient->getEngine()->getSceneManager();
	SceneNode* sn = onew(SceneNode)(sm);
	mySceneNode->addChild(sn);
	VtkRenderable* vdw = onew(VtkRenderable)();
	vdw->setActor(actor);
	sn->addRenderable(vdw);
}
