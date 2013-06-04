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
#ifndef __VTK_MODULE_H__
#define __VTK_MODULE_H__

#include "omegaVtk/ovtkbase.h"
#include "omegaVtk/VtkRenderPass.h"
#include "omegaVtk/VtkAttachPoint.h"

#include "omega/osystem.h"
//#include "omega/Renderer.h"
#include "omega/Engine.h"
#include "omega/Application.h"

class vtkLight;

namespace omegaVtk
{
	using namespace omega;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkModule: public EngineModule
	{
	public:
	friend class VtkAttachPoint;
		//! Creates and initializes the vtk module singleton.
		//! If called multiple times, subsequent calls will do nothing.
		static VtkModule* createAndInitialize();
	public:
		VtkModule();
		~VtkModule();

		static VtkModule* instance();

		virtual void initialize();
		virtual void dispose();
		virtual void initializeRenderer(Renderer* r);
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt) {}

		void attachProp(vtkProp3D* actor, SceneNode* node);
		void detachProp(vtkProp3D* actor, SceneNode* node);
		void addLight(vtkLight* light);
		void removeLight(vtkLight* light);
		void removeAllLights();

	private:
		static VtkModule* myInstance;

		Dictionary<SceneNode*, VtkAttachPoint*> myAttachPoints;

		Lock myClientLock;
		List<VtkRenderPass*> myRenderPasses;
		Renderer* myActiveClient;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline VtkModule* VtkModule::instance()
	{ return myInstance; }

};
#endif