/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __SCENE_RENDERABLE_H__
#define __SCENE_RENDERABLE_H__

#include "omegaToolkitConfig.h"
#include "omega/Renderable.h"
#include "omega/SceneNode.h"

namespace omegaToolkit {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API SceneRenderable: public Renderable
	{
	public:
		SceneRenderable();
		virtual ~SceneRenderable();

		void setSceneNode(SceneNode* value);
		SceneNode* getSceneNode();
		void pushNodeTransform();
		void popNodeTransform();
	
	private:
		SceneNode* mySceneNode;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneRenderable::setSceneNode(SceneNode* value)
	{ mySceneNode = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneNode* SceneRenderable::getSceneNode()
	{ return mySceneNode; }
}; // namespace omegaToolkit

#endif