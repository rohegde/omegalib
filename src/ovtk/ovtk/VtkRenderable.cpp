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
#include "ovtk/VtkRenderable.h"
#include "ovtk/VtkRenderPass.h"
#include "omega/StringUtils.h"

using namespace ovtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkRenderable::VtkRenderable(): myActor(NULL) 
{
	myMatrix = vtkMatrix4x4::New();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkRenderable::render(SceneNode* node, RenderState* state)
{
	// Do stuff only if render pass is a Vtk render pass
	if(state->isFlagSet(VtkRenderPass::RenderVtk))
	{
		if(myActor != NULL && myActor->GetVisibility() != 0)
		{
			const AffineTransform3& xform =  node->getFullTransform();
			const omega::math::matrix<4, 4>& m = xform.matrix();
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					myMatrix->SetElement(i, j, m(i, j));
				}
			}

			myActor->SetUserMatrix(myMatrix);

			// Add actor to render pass prop list.
			// NOTE: we assume this cast works since only VtkRenderPass should set the 
			// RenderVtk flag on a render state.
			VtkRenderPass* vtkrp = (VtkRenderPass*)state->pass;

			if(myActor->HasTranslucentPolygonalGeometry())
			{
				vtkrp->queueProp(myActor, VtkRenderPass::QueueTransparent);
			}
			else
			{
				vtkrp->queueProp(myActor, VtkRenderPass::QueueOpaque);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* VtkRenderable::getBoundingBox()
{
	if(myActor != NULL)
	{
		// We have to make sure no transformation is applied to the actor, to get back the
		// original bounding box.
		myMatrix->Identity();
		myActor->SetUserMatrix(myMatrix);

		double* bounds = myActor->GetBounds();
		float fbounds[6];
		fbounds[0] = bounds[0];
		fbounds[1] = bounds[1];
		fbounds[2] = bounds[2];
		fbounds[3] = bounds[3];
		fbounds[4] = bounds[4];
		fbounds[5] = bounds[5];
		myBBox.setExtents(fbounds[0], fbounds[2], fbounds[4], fbounds[1], fbounds[3], fbounds[5]);
		return &myBBox;
	}
	return NULL;
}

