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
#ifndef __OSG_ENTITY_H__
#define __OSG_ENTITY_H__

#include "oosg/oosgbase.h"
#include "oosg/OsgRenderPass.h"

#include "omega/osystem.h"
#include "omega/EngineClient.h"
#include "omega/ui.h"
#include "omega/scene/BoundingSphere.h"

namespace osg
{
	class Node;
	class FrameStamp;
	class NodeVisitor;
}

namespace oosg
{
	using namespace omega;
	using namespace omega::scene;
	using namespace omega::ui;

	class OsgRenderable;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! Stores data relative to one Osg scene hierarchy. OsgScene also supports loading scenes from 
	//! .osg files. Scenes can be rendered by attaching them to OsgRenderable instances.
	class OOSG_API OsgEntity
	{
	public:
		OsgEntity();
		~OsgEntity();

		float getRepresentationSize();
		void setRepresentationSize(float value);

		void update(const UpdateContext& context);
		void load(const String& filename);
		void addToScene(SceneNode* node);

	private:
		float myRepresentationSize;

		osg::Node* myModel;
		osg::FrameStamp* myFrameStamp;
		osg::NodeVisitor* myUpdateVisitor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//inline SceneNode* OsgEntity::getSceneNode()
	//{ return mySceneNode; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline float OsgEntity::getRepresentationSize()
	{ return myRepresentationSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void OsgEntity::setRepresentationSize(float value)
	{ myRepresentationSize = value; }
};
#endif