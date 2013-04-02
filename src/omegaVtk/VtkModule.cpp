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
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkOpenGLRenderer.h>
#include <vtkLight.h>

//#include "omegaVtk/PyVtk.h"
#include "omega/PythonInterpreter.h"
#include "omegaVtk/VtkModule.h"
#include "omegaVtk/VtkRenderPass.h"
#include "omegaVtk/VtkAttachPoint.h"

using namespace omega;
using namespace omegaVtk;

VtkModule* VtkModule::myInstance = NULL;

#if defined(OMEGA_TOOL_VS10) || defined(OMEGA_TOOL_VS9)
#define VTK_LIBRARY_DIR_POSTFIX "/Release"
#else
#define VTK_LIBRARY_DIR_POSTFIX 
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkModule* VtkModule::createAndInitialize()
{
	if(myInstance == NULL)
	{
		myInstance = new VtkModule();
		ModuleServices::addModule(myInstance);
		myInstance->doInitialize(Engine::instance());
	}
	return myInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkModule::VtkModule(): EngineModule("VtkModule")
{
	myInstance = this;
	myActiveClient = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkModule::~VtkModule()
{
	myInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::initialize()
{
	omsg("VtkModule initializing...");

	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->addPythonPath(VTK_LIBRARY_DIR VTK_LIBRARY_DIR_POSTFIX);
	interp->addPythonPath(VTK_PYTHON_DIR);
	
	omsg("VtkModule initialization OK");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::dispose()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::initializeRenderer(Renderer* r)
{
	VtkRenderPass* vtkrp = new VtkRenderPass(r, "VtkRenderPass");
	vtkrp->setUserData(this);
	r->addRenderPass(vtkrp, true);
	myRenderPasses.push_back(vtkrp);

	typedef Dictionary<SceneNode*, VtkAttachPoint*>::Item DItem;
	foreach(DItem item, myAttachPoints)
	{
		item->queueProps(vtkrp);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::attachProp(vtkProp3D* actor, SceneNode* node)
{
	VtkAttachPoint* vtkap = myAttachPoints[node];
	if(vtkap == NULL)
	{
		ofmsg("VtkModule::attachProp: creating attach point for node %1%", %node->getName());
		vtkap = new VtkAttachPoint();
		myAttachPoints[node] = vtkap;
		node->addComponent(vtkap);
	}
	vtkap->attachProp(actor);
	vtkap->requestBoundingBoxUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::detachProp(vtkProp3D* actor, SceneNode* node)
{
	VtkAttachPoint* vtkap = myAttachPoints[node];
	if(vtkap != NULL)
	{
		vtkap->detachProp(actor);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::update(const UpdateContext& context)
{
	bool itemDirty = false;
	bool allRenderPassesInitialized = true;

	// Reset render queues only if all render passes are initialized.
	foreach(VtkRenderPass* rp, myRenderPasses) allRenderPassesInitialized &= rp->isInitialized();
	
	if(allRenderPassesInitialized)
	{
		typedef Dictionary<SceneNode*, VtkAttachPoint*>::Item DItem;

		// See if any attach point is dirty.
		foreach(DItem item, myAttachPoints)	itemDirty |= item->isDirty();

		// If any item is dirty, reset render queues for all renderers
		if(itemDirty)
		{
			foreach(VtkRenderPass* rp, myRenderPasses)
			{
				rp->resetPropQueues();
				foreach(DItem item, myAttachPoints)
				{
					item->queueProps(rp);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::addLight(vtkLight* light)
{
	foreach(VtkRenderPass* rp, myRenderPasses) rp->getRenderer()->AddLight(light);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::removeLight(vtkLight* light)
{
	foreach(VtkRenderPass* rp, myRenderPasses) rp->getRenderer()->RemoveLight(light);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkModule::removeAllLights()
{
	foreach(VtkRenderPass* rp, myRenderPasses) rp->getRenderer()->RemoveAllLights();
}

