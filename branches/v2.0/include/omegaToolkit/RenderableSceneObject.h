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
#ifndef __RENDERABLE_SCENE_OBJECT_H__
#define __RENDERABLE_SCENE_OBJECT_H__

#include "omegaToolkitConfig.h"
#include "SceneRenderable.h"
#include "omega/ISceneObject.h"
#include "omegaToolkit/Effect.h"

namespace omegaToolkit {

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API RenderableSceneObject: public RenderableFactory, public ISceneObject
	{
	public:
		RenderableSceneObject();

		virtual Renderable* createRenderable() { return NULL; }

		virtual void update(SceneNode* owner) {}

		//! This method is called by engine client threads to traverse the scene hierarchy during
		//! rendering.
		void draw(SceneNode* node, RenderState* state);

		virtual const AlignedBox3* getBoundingBox() { return NULL; }
		virtual bool hasBoundingBox() { return false; }

		void setEffect(BaseEffect* value) { myEffect = value; }
		BaseEffect* getEffect() { return myEffect; }

		virtual bool isInitialized() {return RenderableFactory::isInitialized(); }
		virtual void initialize(ServerEngine* server) {RenderableFactory::initialize(server); }
	private:
		BaseEffect* myEffect;
	};
}; // namespace omegaToolkit

#endif