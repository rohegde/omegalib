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
#ifndef __OSG_MODULE_H__
#define __OSG_MODULE_H__

#include "omegaOsg/oosgbase.h"
#include "omega/EngineServer.h"

//! Vector conversion macros
#define OOSG_VEC3(v) osg::Vec3(v[0], v[1], v[2])
#define OOSG_VEC2(v) osg::Vec2(v[0], v[1])

namespace osg
{
	class Node;
	class FrameStamp;
	class NodeVisitor;
}

namespace omegaOsg
{
	using namespace omega;

	class OsgSceneObject;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OOSG_API OsgModule
	{
	public:
		OsgModule();
		~OsgModule();

		float getRepresentationSize();
		void setRepresentationSize(float value);

		void initialize(EngineServer* server);
		void update(const UpdateContext& context);
		//void loadScene(const String& filename);
		//void addToScene(SceneNode* node);

		osg::FrameStamp* getFrameStamp() { return myFrameStamp; }
		osg::Node* getRootNode() { return myRootNode; }
		void setRootNode(osg::Node* value);

	private:
		EngineServer* myServer;
		float myRepresentationSize;

		//OsgSceneObject* myRootSceneObject;
		osg::Node* myRootNode;
		osg::FrameStamp* myFrameStamp;
		osg::NodeVisitor* myUpdateVisitor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline float OsgModule::getRepresentationSize()
	{ return myRepresentationSize; }

	///////////////////////////////////////////////////////////////////////////////////////////////////
	inline void OsgModule::setRepresentationSize(float value)
	{ myRepresentationSize = value; }
};
#endif