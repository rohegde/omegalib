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
#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include "osystem.h"
#include "omega/ISceneObject.h"
#include "omega/Node.h"

namespace omega {
	class ServerEngine;
	class Renderable;
	class SceneNode;
	struct RenderState;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class SceneNodeListener
	{
	public:
		virtual void onVisibleChanged(SceneNode* source, bool value) {}
		virtual void onSelectedChanged(SceneNode* source, bool value) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API SceneNode: public Node
	{
	public:
		typedef ChildNode<SceneNode> Child;
		enum HitType { HitBoundingSphere };

	public:
		SceneNode(ServerEngine* server):
			myServer(server),
			myListener(NULL),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false),
			mySelectable(false),
			myChanged(false),
			myVisible(true),
			mySelected(false)
			{}

		SceneNode(ServerEngine* server, const String& name):
			Node(name),
			myServer(server),
			myListener(NULL),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false),
			mySelectable(false),
			myChanged(false),
			myVisible(true),
			mySelected(false)
			{}

		ServerEngine* getServer();

		// Object
		//@{
		void addObject(ISceneObject* o);
		int getNumObjects();
		void removeObject(ISceneObject* o);
		//@}

		// Options
		//@{
		bool isSelectable();
		void setSelectable(bool value);
		bool isVisible();
		void setVisible(bool value);
		void setSelected(bool value);
		bool isSelected();
		//@}

		// Bounding box handling
		//@{
		const AlignedBox3& getBoundingBox();
		const Sphere& getBoundingSphere();
		bool isBoundingBoxVisible();
		void setBoundingBoxVisible(bool value);
		const Color& getBoundingBoxColor();
		void setBoundingBoxColor(const Color& color);
		//@}

		// Bounding box handling
		//@{
		void setListener(SceneNodeListener* listener);
		SceneNodeListener* getListener();
		//@}

		//! Hit test.
		bool hit(const Ray& ray, Vector3f* hitPoint, HitType type);

		virtual void update(bool updateChildren, bool parentHasChanged);

		void draw(RenderState* state);

	private:
		void drawBoundingBox();
		void updateBoundingBox();

	private:
		ServerEngine* myServer;

		SceneNodeListener* myListener;

		List<ISceneObject*> myObjects;

		bool mySelectable;
		bool mySelected;
		bool myVisible;

		bool myChanged;
		AlignedBox3 myBBox;
		Sphere myBSphere;

		// Bounding box stuff.
		bool myBoundingBoxVisible;
		Color myBoundingBoxColor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AlignedBox3& SceneNode::getBoundingBox() 
	{ return myBBox; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Sphere& SceneNode::getBoundingSphere() 
	{ return myBSphere; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SceneNode::isBoundingBoxVisible() 
	{ return myBoundingBoxVisible; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setBoundingBoxVisible(bool value) 
	{ myBoundingBoxVisible = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Color& SceneNode::getBoundingBoxColor() 
	{ return myBoundingBoxColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setBoundingBoxColor(const Color& color) 
	{ myBoundingBoxColor = color; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ServerEngine* SceneNode::getServer()
	{ return myServer; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SceneNode::isSelectable() 
	{ return mySelectable; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setSelectable(bool value) 
	{ mySelectable = value; }
}; // namespace omega

#endif