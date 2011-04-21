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
#ifndef __VTK_RENDER_PASS_H__
#define __VTK_RENDER_PASS_H__

#include "omega/osystem.h"
#include "omega/scene/RenderPass.h"
#include "omega/scene/SceneManager.h"

class vtkProp;
class vtkRenderer;
class vtkOpaquePass;
class vtkGenericOpenGLRenderWindow;
class vtkRenderState;

namespace ovtk
{
	using namespace omega;
	using namespace omega::scene;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class VtkRenderPass: public RenderPass
	{
	friend class VtkRenderable;
	public:
		enum RenderFlags { RenderVtk = RenderPass::RenderCustom << 1 };
		static const int MaxQueuedProps = 1024;

	public:
		VtkRenderPass();

		void initialize();
		virtual void render(SceneManager* mng);

	private:
		void queueProp(vtkProp* actor);

	private:
		vtkRenderer* myRenderer;
		vtkOpaquePass* myOpaquePass;
		vtkRenderState* myRenderState;
		vtkGenericOpenGLRenderWindow* myRenderWindow;

		vtkProp* myPropQueue[MaxQueuedProps];
		int myPropQueueSize;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void VtkRenderPass::queueProp(vtkProp* actor)
	{
		if(myPropQueueSize < MaxQueuedProps)
		{
			myPropQueue[myPropQueueSize++] = actor;
		}
	}
};
#endif