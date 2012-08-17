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
#ifndef __CY_DRAWABLE_OBJECT__
#define __CY_DRAWABLE_OBJECT__

#include "cyclopsConfig.h"
#include "EffectNode.h"

#include <osg/Group>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;

	class SceneManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! PYAPI
	class CY_API Entity: public SceneNode
	{
	public:
		Entity(SceneManager* scene);

		// Function called each frame to update the entity state.
		virtual void update(const UpdateContext& context) {}

		osg::Node* getOsgNode() { return myEffect; }
		//SceneNode* getSceneNode() { return mySceneNode; }

		void setTag(const String& value) { myTag = value; }
		const String& getTag() { return myTag; } 

		//!PYAPI
		bool hasEffect();
		//! PYAPI
		void setEffect(const String& effectDefinition);

	protected:
		void initialize(osg::Node* node);

	private:
		SceneManager* mySceneManager;

		String myTag;

		osg::Node* myOsgNode;
		//SceneNode* mySceneNode;
		OsgSceneObject* myOsgSceneObject;

		EffectNode* myEffect;
	};
};

#endif
