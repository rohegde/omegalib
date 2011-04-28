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
#include "ovtk/VtkRenderPass.h"
#include "ovtk/vtkGenericOpenGLRenderWindow.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkOpaquePass.h>
#include <vtkDepthPeelingPass.h>
#include <vtkTranslucentPass.h>
#include <vtkOverlayPass.h>
#include <vtkRenderState.h>

using namespace ovtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkRenderPass::VtkRenderPass():
	myPropQueueSize(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderPass::initialize()
{
	// Setup renderer and render window
	myRenderer = vtkRenderer::New();

	myRenderWindow = vtkGenericOpenGLRenderWindow::New();
	myRenderWindow->AddRenderer(myRenderer);
	myOpaquePass = vtkOpaquePass::New();
	myTranslucentPass = vtkDepthPeelingPass::New();
	myRenderState = new vtkRenderState(myRenderer);

	myTranslucentPass->SetTranslucentPass(vtkTranslucentPass::New());
	//myTranslucentPass->SetOcclusionRatio(0.1f);
	//myTranslucentPass->SetMaximumNumberOfPeels(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderPass::render(SceneManager* mng)
{
	RenderState state;
	state.pass = this;
	state.flags = VtkRenderPass::RenderVtk;

	myPropQueueSize = 0;

	mng->getRootNode()->draw(&state);

	// For scene node drawing, we are not using the gl matrix stack, we are using our own transforms,
	// stored inside the scene nodes. So, create a new, clean transform on the stack.
	glPushMatrix();
	glLoadMatrixf(mng->getViewTransform().data());

	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_NORMALIZE);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	myRenderState->SetPropArrayAndCount(myPropQueue, myPropQueueSize);
	myOpaquePass->Render(myRenderState);

	glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);

	myTranslucentPass->Render(myRenderState);

	glPopAttrib();
	glPopMatrix();
}
