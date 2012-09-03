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
#ifndef __VTK_ATTACH_POINT_H__
#define __VTK_ATTACH_POINT_H__

#include "omega/ISceneObject.h"

#include "ovtkbase.h"

class vtkProp3D;
class vtkMatrix4x4;
namespace omegaVtk {

	using namespace omega;
	//using namespace omegaToolkit;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkAttachPoint: public ISceneObject
	{
	public:
		VtkAttachPoint();
		~VtkAttachPoint();

		float getRepresentationSize();
		void setRepresentationSize(float value);

		virtual void update(SceneNode* owner);

		virtual const AlignedBox3* getBoundingBox();
		virtual bool hasBoundingBox();

		virtual bool isInitialized() { return myInitialized; }
		virtual void initialize(Engine* server) { myInitialized = true; }

		void attachProp(vtkProp3D* prop);
		void detachProp(vtkProp3D* prop);

		void queueProps(VtkRenderPass* rp);

		bool isDirty() { return myDirty; }
		void setDirty(bool value) { myDirty = value; }

	private:
		//! Gets the first available vtk prop from the attached renderables.
		vtkProp3D* getFirstProp();

	private:
		vtkMatrix4x4* myMatrix;
		AlignedBox3 myBBox;
		float myRepresentationSize;
		bool myInitialized;

		List<vtkProp3D*> myProps;

		bool myDirty;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline float VtkAttachPoint::getRepresentationSize()
	{ return myRepresentationSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void VtkAttachPoint::setRepresentationSize(float value)
	{ myRepresentationSize = value; }
};
#endif
