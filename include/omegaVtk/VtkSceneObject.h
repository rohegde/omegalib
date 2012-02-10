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
#ifndef __VTK_VIEW_CLIENT_H__
#define __VTK_VIEW_CLIENT_H__

#include "omegaToolkit/RenderableSceneObject.h"

#include "ovtkbase.h"

class vtkProp3D;
class vtkMatrix4x4;
namespace omegaVtk {

	using namespace omega;
	using namespace omegaToolkit;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkSceneObject: public RenderableSceneObject
	{
	public:
		VtkSceneObject(const String& name);
		~VtkSceneObject();

		virtual Renderable* createRenderable();

		float getRepresentationSize();
		void setRepresentationSize(float value);

		virtual void update(SceneNode* owner);

		virtual const AlignedBox3* getBoundingBox();
		virtual bool hasBoundingBox();

		virtual bool isInitialized() { return myInitialized; }
		virtual void initialize(EngineServer* server) { myInitialized = true; }

		const String& getName() { return myName; }

	private:
		//! Gets the first available vtk prop from the attached renderables.
		vtkProp3D* getFirstProp();

	private:
		vtkMatrix4x4* myMatrix;
		AlignedBox3 myBBox;
		float myRepresentationSize;
		bool myInitialized;
		String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkRenderable: public Renderable
	{
	public:
		VtkRenderable(): myActor(NULL) {}

		virtual void draw(RenderState* state) {}

		void setActor(vtkProp3D* value);
		vtkProp3D* getActor();

	private:
		vtkProp3D* myActor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline float VtkSceneObject::getRepresentationSize()
	{ return myRepresentationSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void VtkSceneObject::setRepresentationSize(float value)
	{ myRepresentationSize = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void VtkRenderable::setActor(vtkProp3D* value)
	{ myActor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline vtkProp3D* VtkRenderable::getActor()
	{ return myActor; }
};
#endif
