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
#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include "osystem.h"
#include "omega/NodeComponent.h"
#include "omega/Node.h"

namespace omega {
	class Engine;
	class Renderable;
	class SceneNode;
	class Camera;
	class TrackedObject;
	struct RenderState;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API SceneNodeListener
	{
	public:
		virtual void onVisibleChanged(SceneNode* source, bool value) {}
		virtual void onSelectedChanged(SceneNode* source, bool value) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Represents a node in the omegalib scene graph.
	//! @remarks
	//!		SceneNode instances add some functionality over the Node base class. Namely:
	//!			- renderable objects can be attached to a scene node;
	//!			- a scene node has a bounding box;
	//!			- scene nodes have selection and visibility flags
	//!			- it is possible to attach listeners to scene nodes, to handle visibility change,
	//!				selection change and other events.
	class OMEGA_API SceneNode: public Node
	{
	public:
//		typedef ChildNode<SceneNode> Child;
		enum HitType { 
			//! Perform hit tests on object bounding sphere
			HitBoundingSphere, 
			//! Perform hit tests using best available intersector for the node.
			HitBest };
		static SceneNode* create(const String& name);

	public:
		SceneNode(Engine* server):
			myServer(server),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false),
			mySelectable(false),
			myChanged(false),
			myVisible(true),
			mySelected(false),
			myFacingCamera(NULL),
			myTracker(NULL)
			{}

		SceneNode(Engine* server, const String& name):
			Node(name),
			myServer(server),
			myBoundingBoxColor(1, 1, 1, 1),
			myBoundingBoxVisible(false),
			mySelectable(false),
			myChanged(false),
			myVisible(true),
			mySelected(false),
			myFacingCamera(NULL),
			myTracker(NULL)
			{}

		Engine* getEngine();

		// Object
		//@{
		void addComponent(NodeComponent* o);
		int getNumComponents();
		void removeComponent(NodeComponent* o);
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

		// Listeners
		//@{
		void addListener(SceneNodeListener* listener);
		void removeListener(SceneNodeListener* listener);
		//@}

		//! Hit test.
		bool hit(const Ray& ray, Vector3f* hitPoint, HitType type);

		virtual void update(bool updateChildren, bool parentHasChanged);

		void draw(const DrawContext& context);

		void setTag(const String& value) { myTag = value; }
		const String& getTag() { return myTag; } 

		//! Billboard mode
		//@{
		void setFacingCamera(Camera* cam);
		Camera* getFacingCamera();
		//@}

		//! Trackable object
		//@{
		void followTrackable(int trackableId);
		void setFollowOffset(const Vector3f& offset, const Quaternion& ooffset);
		void unfollow();
		//@}

	private:
		void drawBoundingBox();
		void updateBoundingBox();
		bool needsBoundingBoxUpdate();

	private:
		Engine* myServer;

		List<SceneNodeListener*> myListeners;

		List< Ref<NodeComponent> > myObjects;

		bool mySelectable;
		bool mySelected;
		bool myVisible;

		bool myChanged;
		AlignedBox3 myBBox;
		Sphere myBSphere;

		String myTag;

		// Bounding box stuff.
		bool myBoundingBoxVisible;
		Color myBoundingBoxColor;

		// Target camera for billboard mode. Can't use Ref due to circular dependency.
		Camera* myFacingCamera;
		// Tracked object. This is internally managed and does not need Ref. 
		TrackedObject* myTracker;
	};

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
	inline Engine* SceneNode::getEngine()
	{ return myServer; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SceneNode::isSelectable() 
	{ return mySelectable; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setSelectable(bool value) 
	{ mySelectable = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneNode::setFacingCamera(Camera* cam)
	{ myFacingCamera = cam; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Camera* SceneNode::getFacingCamera()
	{ return myFacingCamera; }
}; // namespace omega

#endif