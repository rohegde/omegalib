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
#ifndef __VTK_CLIENT_H__
#define __VTK_CLIENT_H__

#include "omega/osystem.h"
#include "omega/EngineClient.h"
#include "ovtk/ovtkbase.h"
#include "ovtk/VtkRenderPass.h"
#include "ovtk/PythonUIEventHandler.h"

class vtkActor;
namespace ovtk
{
	using namespace omega;
	using namespace omega::scene;

	class PythonInterpreter;
	class VtkDrawable;
	class VtkEntity;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkClient
	{
	public:
		VtkClient(EngineClient* engine);
		~VtkClient();

		static VtkClient* instance();

		void initialize();

		void runScript(const String& name);
		EngineClient* getEngine();

		VtkEntity* createEntity();
		void destroyEntity(VtkEntity* entity);

		//! Script ovtk calls will apply to this entity
		void setActiveEntity(VtkEntity* entity);
		VtkEntity* getActiveEntity();

		PythonInterpreter* getInterpreter();

	private:
		static VtkClient* myInstance;

		SceneNode* myVtkNode;
		EngineClient* myEngine;
		VtkRenderPass* myRenderPass;
		PythonInterpreter* myInterpreter;

		List<VtkEntity*> myEntities;
		VtkEntity* myActiveEntity;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline VtkClient* VtkClient::instance()
	{ return myInstance; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline EngineClient* VtkClient::getEngine()
	{ return myEngine; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void VtkClient::setActiveEntity(VtkEntity* entity)
	{ myActiveEntity = entity; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline VtkEntity* VtkClient::getActiveEntity()
	{ return myActiveEntity; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline PythonInterpreter* VtkClient::getInterpreter()
	{ return myInterpreter; }
};
#endif