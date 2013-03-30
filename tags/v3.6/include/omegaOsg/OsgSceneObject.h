/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __OSG_SCENE_OBJECT_H__
#define __OSG_SCENE_OBJECT_H__

#include "omegaOsg/oosgbase.h"
#include "omega/NodeComponent.h"
#include "omega/SceneNode.h"

namespace osg
{
	class MatrixTransform;
	class Node;
}

namespace omegaOsg
{
	using namespace omega;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OOSG_API OsgSceneObject: public NodeComponent, SceneNodeListener
	{
	public:
		OsgSceneObject(osg::Node* node);
		~OsgSceneObject();

		virtual void update(const UpdateContext& context);

		virtual void onVisibleChanged(SceneNode* source, bool value);
		virtual void onSelectedChanged(SceneNode* source, bool value);

		virtual const AlignedBox3* getBoundingBox();
		virtual bool hasBoundingBox() { return true; }

		osg::MatrixTransform* getTransformedNode() { return myTransform; }

		virtual bool isInitialized() { return myInitialized; }
		virtual void initialize(Engine* server) { myInitialized = true; }

		virtual bool hasCustomRayIntersector() { return true; }
		virtual bool intersectRay(const Ray& ray, Vector3f* hitPoint);

		virtual void onAttached(SceneNode*);
		virtual void onDetached(SceneNode*);

	private:
		Ref<osg::Node> myNode;
		Ref<osg::MatrixTransform> myTransform;
		AlignedBox3 myBBox;
		bool myInitialized;
	};
};
#endif