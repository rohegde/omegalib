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
#include <vtkCamera.h>
#include <vtkLightsPass.h>
#include <vtkOpenGLRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkOpaquePass.h>
#include <vtkTranslucentPass.h>
#include <vtkOverlayPass.h>
#include <vtkVolumetricPass.h>
#include <vtkRenderState.h>
#include <vtkOpenGLRenderer.h>

#include "CustomDepthPeelingPass.h"
#include "omegaVtk/VtkRenderPass.h"
#include "omegaVtk/vtkGenericOpenGLRenderWindow.h"


using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkRenderPass::VtkRenderPass(Renderer* client, const String& name): RenderPass(client, name)
{
	resetPropQueues();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkRenderPass::~VtkRenderPass()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vtkOpenGLRenderer* VtkRenderPass::getRenderer() 
{
	return myRenderer; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderPass::initialize()
{
	RenderPass::initialize();

	// Setup renderer and render window
	myRenderer = vtkOpenGLRenderer::New();
	myRenderer->UseDepthPeelingOn();

	myRenderWindow = vtkGenericOpenGLRenderWindow::New();
	myRenderWindow->MakeCurrent();
	myRenderWindow->OpenGLInit();
	myRenderWindow->AddRenderer(myRenderer);
	
	myOpaquePass = vtkOpaquePass::New();
	myDepthPeelingPass = CustomDepthPeelingPass::New();
	myTranslucentPass = vtkTranslucentPass::New();
	myOverlayPass = vtkOverlayPass::New();
	myVolumetricPass = vtkVolumetricPass::New();
	myLightsPass = vtkLightsPass::New();

	myRenderState = new vtkRenderState(myRenderer);

	//resetPropQueues();
	myDepthPeelingPass->SetTranslucentPass(myTranslucentPass);
	//myTranslucentPass->SetTranslucentPass(vtkTranslucentPass::New());
}

Lock sLock;

///////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderPass::queueProp(vtkProp* actor, QueueType queue)
{
sLock.lock();
	oassert(queue < NumQueues);

	if(myPropQueueSize[queue] < MaxQueuedProps)
	{
		myPropQueue[queue][myPropQueueSize[queue]++] = actor;
	}
	//myRenderer->AddViewProp(actor);
sLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderPass::resetPropQueues()
{
	for(int i = 0; i < NumQueues; i++) myPropQueueSize[i] = 0;
	//myRenderer->RemoveAllViewProps();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderPass::render(Renderer* mng, const DrawContext& context)
{
	if(context.task == DrawContext::SceneDrawTask)
	{
		myRenderWindow->SetSize(context.tile->pixelSize[0], context.tile->pixelSize[1]);
		//myRenderWindow->SetTileViewport(context
		//RenderState state;
		//state.pass = this;
		//state.flags = VtkRenderPass::RenderVtk;
		//state.renderer = mng->getRenderer();

		//mng->getRootNode()->draw(&state);

		// For scene node drawing, we are not using the gl matrix stack, we are using our own transforms,
		// stored inside the scene nodes. So, create a new, clean transform on the stack.
		glPushMatrix();
		glLoadMatrixf(context.modelview.data());

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_NORMALIZE);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		//myRenderer->Render();

sLock.lock();
		myLightsPass->Render(myRenderState);
		myRenderState->SetPropArrayAndCount(myPropQueue[QueueOpaque], myPropQueueSize[QueueOpaque]);
		myOpaquePass->Render(myRenderState);
sLock.unlock();

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glDisable(GL_DEPTH_TEST);

sLock.lock();
		myRenderState->SetPropArrayAndCount(myPropQueue[QueueTransparent], myPropQueueSize[QueueTransparent]);
		//myTranslucentPass->Render(myRenderState);
		myDepthPeelingPass->Render(myRenderState);
sLock.unlock();
	
		// Volume rendering not supported for now: it requires forwarding to vtk information
		// about the view through the renderer active camera (vtkCamera) and also information
		// about the viewport. This is doable but will probably require a custom version of
		// vtkCamera and vtkRenderer
		//myVolumetricPass->Render(myRenderState);
	
sLock.lock();
		myRenderState->SetPropArrayAndCount(myPropQueue[QueueOverlay], myPropQueueSize[QueueOverlay]);
		myOverlayPass->Render(myRenderState);
sLock.unlock();

		glPopAttrib();
		glPopMatrix();
	}
}
