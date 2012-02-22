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
//#include "omegaVtk/PyVtk.h"
#include "omegaVtk/VtkModule.h"
#include "omegaVtk/VtkRenderPass.h"
#include "omegaVtk/VtkSceneObject.h"


#include <vtkActor.h>
#include <vtkProperty.h>

using namespace omega;
using namespace omegaVtk;

VtkModule* VtkModule::myInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkModule::VtkModule()
{
	myInstance = this;
	myActiveClient = NULL;
	myActiveRenderPass = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkModule::~VtkModule()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::initialize(EngineServer* server)
{
	omsg("VtkModule initializing...");

	myEngine = server;
	myEngine->registerRenderPassClass("VtkRenderPass", (EngineServer::RenderPassFactory)VtkRenderPass::createInstance);
	myEngine->addRenderPass("VtkRenderPass", this, true);

	omsg("VtkModule initialization OK");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::beginClientInitialize(EngineClient* client)
{
	oassert(myActiveClient == NULL);

	myClientLock.lock();
	myActiveClient = client;
	myActiveRenderPass = dynamic_cast<VtkRenderPass*>(myActiveClient->getRenderPass("VtkRenderPass"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::endClientInitialize()
{
	if(myActiveClient != NULL)
	{
		myActiveClient = NULL;
		myActiveRenderPass = NULL;
		myClientLock.unlock();
	}
	else
	{
		owarn("VtkModule::endClientInitialize: beginClientInitialize has not been called. Returning.");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::attachActor(vtkActor* actor, VtkSceneObject* sceneObject)
{
	if(myActiveClient != NULL)
	{
		if(actor->HasTranslucentPolygonalGeometry())
		{
			myActiveRenderPass->queueProp(actor, VtkRenderPass::QueueTransparent);
		}
		else
		{
			myActiveRenderPass->queueProp(actor, VtkRenderPass::QueueOpaque);
		}

		VtkRenderable* renderable = dynamic_cast<VtkRenderable*>(sceneObject->getRenderable(myActiveClient));
		renderable->setActor(actor);
	}
	else
	{
		owarn("VtkModule::addActor: beginClientInitialize has not been called. Returning");
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::registerSceneObject(VtkSceneObject* sceneObject)
{
	if(sceneObject != NULL)
	{
		mySceneObjects[sceneObject->getName()] = sceneObject;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::unregisterSceneObject(VtkSceneObject* sceneObject)
{
	if(sceneObject != NULL)
	{
		mySceneObjects[sceneObject->getName()] = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkSceneObject* VtkModule::getSceneObject(const String& name)
{
	return mySceneObjects[name];
}

