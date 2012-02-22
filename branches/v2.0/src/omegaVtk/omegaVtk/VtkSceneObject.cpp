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
#include "omegaVtk/VtkModule.h"
#include "omegaVtk/VtkSceneObject.h"

#include <vtkMatrix4x4.h>
#include <vtkActor.h>

using namespace omega;
using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkSceneObject::VtkSceneObject(): myName("VtkSceneObject")
{
	VtkModule::instance()->registerSceneObject(this);
	myMatrix = vtkMatrix4x4::New();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkSceneObject::VtkSceneObject(const String& name): myName(name)
{
	VtkModule::instance()->registerSceneObject(this);
	myMatrix = vtkMatrix4x4::New();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkSceneObject::~VtkSceneObject()
{
	VtkModule::instance()->unregisterSceneObject(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* VtkSceneObject::createRenderable()
{
	return new VtkRenderable();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkSceneObject::update(SceneNode* owner)
{
	const AffineTransform3& xform =  owner->getFullTransform();
	const omega::math::matrix<4, 4>& m = xform.matrix();
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			myMatrix->SetElement(i, j, m(i, j));
		}
	}
	foreach(Renderable* r, getRenderables())
	{
		VtkRenderable* vtkr = dynamic_cast<VtkRenderable*>(r);
		vtkProp3D* vtkProp = vtkr->getActor();
		if(vtkProp != NULL)
		{
			vtkProp->SetUserMatrix(myMatrix);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
vtkProp3D* VtkSceneObject::getFirstProp()
{
	Renderable* r = getFirstRenderable();
	if(r != NULL)
	{
		VtkRenderable* vtkr = dynamic_cast<VtkRenderable*>(r);
		return vtkr->getActor();
	}
	return NULL; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool VtkSceneObject::hasBoundingBox() 
{ 
	vtkProp3D* p = getFirstProp();
	return (p != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* VtkSceneObject::getBoundingBox()
{
	vtkProp3D* prop = getFirstProp();
	if(prop != NULL)
	{
		// We have to make sure no transformation is applied to the actor, to get back the
		// original bounding box.
		myMatrix->Identity();
		prop->SetUserMatrix(myMatrix);

		double* bounds = prop->GetBounds();
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
