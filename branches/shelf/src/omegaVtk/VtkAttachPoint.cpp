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
#include <vtkMatrix4x4.h>

#include "omegaVtk/VtkModule.h"
#include "omegaVtk/VtkAttachPoint.h"


using namespace omega;
using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkAttachPoint::VtkAttachPoint():
	myDirty(false),
	myVisible(true)
{
	myMatrix = vtkMatrix4x4::New();
	myIdentityMatrix = vtkMatrix4x4::New();
	myIdentityMatrix->Identity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkAttachPoint::~VtkAttachPoint()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkAttachPoint::attachProp(vtkProp3D* prop)
{
	myProps.push_back(prop);
	myDirty = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkAttachPoint::detachProp(vtkProp3D* prop)
{
	myProps.remove(prop);
	myDirty = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkAttachPoint::update(const UpdateContext& context)
{
	SceneNode* owner = getOwner();
	const AffineTransform3& xform =  owner->getFullTransform();
	const Matrix4f& m = xform.matrix();
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			myMatrix->SetElement(i, j, m(i, j));
		}
	}
	foreach(vtkProp3D* vtkProp, myProps)
	{
		if(vtkProp != NULL)
		{
			//omsg("VtkAttachPoint::update matrix");
			//const Vector3f& pos = owner->getPosition();
			//const Vector3f& scale = owner->getScale();
			//vtkProp->SetScale(scale[0], scale[1], scale[2]);
			//vtkProp->SetPosition(pos[0], pos[1], pos[2]);
			vtkProp->SetUserMatrix(myMatrix);
		}
	}
	if(owner->isVisible() != myVisible)
	{
		myDirty = true;
		myVisible = owner->isVisible();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
vtkProp3D* VtkAttachPoint::getFirstProp()
{
	if(myProps.empty()) return NULL;
	return *myProps.begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool VtkAttachPoint::hasBoundingBox() 
{ 
	vtkProp3D* p = getFirstProp();
	return (p != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* VtkAttachPoint::getBoundingBox()
{
	vtkProp3D* prop = getFirstProp();
	if(prop != NULL)
	{
		// We have to make sure no transformation is applied to the actor, to get back the
		// original bounding box.
		prop->SetUserMatrix(myIdentityMatrix);

		double* bounds = prop->GetBounds();
		float fbounds[6];
		fbounds[0] = bounds[0];
		fbounds[1] = bounds[1];
		fbounds[2] = bounds[2];
		fbounds[3] = bounds[3];
		fbounds[4] = bounds[4];
		fbounds[5] = bounds[5];
		myBBox.setExtents(fbounds[0], fbounds[2], fbounds[4], fbounds[1], fbounds[3], fbounds[5]);
		//ofmsg("bbox: %1%  -- %2%", %myBBox.getMinimum() %myBBox.getMaximum());

		prop->SetUserMatrix(myMatrix);

		return &myBBox;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void VtkAttachPoint::queueProps(VtkRenderPass* rp)
{
	foreach(vtkProp3D* prop, myProps)
	{
		if(prop->GetVisibility() && myVisible){
			if(prop->HasTranslucentPolygonalGeometry())
			{
				rp->queueProp(prop, VtkRenderPass::QueueTransparent);
			}
			else
			{
				rp->queueProp(prop, VtkRenderPass::QueueOpaque);
			}
		}
	}
}
