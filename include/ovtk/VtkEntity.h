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
#ifndef __VTK_ENTITY_H__
#define __VTK_ENTITY_H__

#include "ovtk/ovtkbase.h"
#include "ovtk/VtkRenderPass.h"
#include "ovtk/PythonUIEventHandler.h"

#include "omega/osystem.h"
#include "omega/EngineClient.h"
#include "omega/ui.h"
#include "omega/scene/BoundingSphere.h"


class vtkProp3D;
namespace ovtk
{
	using namespace omega;
	using namespace omega::scene;
	using namespace omega::ui;

	class VtkDrawable;
	class VtkClient;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OVTK_API VtkEntity
	{
	friend class VtkClient;
	public:

		float getRepresentationSize();
		void setRepresentationSize(float value);

		SceneNode* getSceneNode();

		void loadScript(const String& filename);

		//! Internal
		void addActor(vtkProp3D* actor);
		void addButton(const String& name, const String& clickCommand);
		void addCheckButton(const String& name, const String& getValueCommand, const String& setValueCommand);

	private:
		VtkEntity(VtkClient* client);
		~VtkEntity();

	private:
		BoundingSphere* myBSphere;
		List<VtkRenderable*> myRenderables;
		VtkClient* myClient;
		float myRepresentationSize;

		SceneNode* mySceneNode;
		Container* myUI;
		List<PythonUIEventHandler*> myEventHandlers;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneNode* VtkEntity::getSceneNode()
	{ return mySceneNode; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline float VtkEntity::getRepresentationSize()
	{ return myRepresentationSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void VtkEntity::setRepresentationSize(float value)
	{ myRepresentationSize = value; }
};
#endif