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
#include "omega/DrawInterface.h"
#include "omega/SceneNode.h"
#include "omega/Renderable.h"
#include "omega/Engine.h"
#include "omega/Camera.h"
#include "omega/ModuleServices.h"
#include "omega/glheaders.h"
#include "omega/TrackedObject.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* SceneNode::create(const String& name)
{
	SceneNode* sn = new SceneNode(Engine::instance(), name);
	Engine::instance()->getScene()->addChild(sn);
	return sn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::addListener(SceneNodeListener* listener)
{
	myListeners.push_back(listener);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::removeListener(SceneNodeListener* listener)
{
	myListeners.remove(listener);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneNode::isVisible()
{ 
	return myVisible; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::setVisible(bool value)
{ 
	if(myListeners.size() != 0)
	{
		foreach(SceneNodeListener* l, myListeners)
		{
			l->onVisibleChanged(this, value);
		}
	}
	myVisible = value; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::setSelected(bool value)
{
	mySelected = value;
	if(myListeners.size() != 0)
	{
		foreach(SceneNodeListener* l, myListeners)
		{
			l->onSelectedChanged(this, value);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneNode::isSelected()
{
	return mySelected;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::addComponent(NodeComponent* o) 
{ 
	myObjects.push_back(o); 
	o->attach(this);
	//needUpdate();
	updateBoundingBox();
	// If the object has not been initialized yet, do it now.
	if(!o->isInitialized()) o->initialize(myServer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SceneNode::getNumComponents()
{ 
	return myObjects.size(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::removeComponent(NodeComponent* o) 
{
	myObjects.remove(o);
	o->detach(this);
	updateBoundingBox();
	//needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::draw(const DrawContext& context)
{
	if(myVisible)
	{
		//if(myChanged) updateTransform();

		if(myBoundingBoxVisible) drawBoundingBox();

		// Draw drawables attached to this node.
		foreach(NodeComponent* d, myObjects)
		{
			d->draw(context);
		}

		// Draw children nodes.
		foreach(Node* child, getChildren())
		{
			SceneNode* n = dynamic_cast<SceneNode*>(child);
			n->draw(context);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::update(bool updateChildren, bool parentHasChanged)
{
	// Reorient the node if a facing camera is set
	if(myFacingCamera != NULL)
	{
		lookAt(myFacingCamera->getPosition() + myFacingCamera->getHeadOffset(), Vector3f::UnitY());
	}

    // Short circuit the off case
    if (!updateChildren && !mNeedParentUpdate && !mNeedChildUpdate && !parentHasChanged)
    {
        return;
    }

	Node::update(updateChildren, parentHasChanged);

	updateBoundingBox();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::update(const UpdateContext& context)
{
	// Step 1: traverse the scene graph and invoke update on all attached node components.
	// Node components will have the change to change the owner node transforms.
	updateTraversal(context);

	// Step 2: update all needed transforms in the node hierarchy
    update(true, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::updateTraversal(const UpdateContext& context)
{
	// Update attached components
	foreach(NodeComponent* d, myObjects)
	{
		d->update(context);
	}
	// Update components of children nodes
	foreach(Node* child, getChildren())
	{
		SceneNode* n = dynamic_cast<SceneNode*>(child);
		if(n) n->updateTraversal(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneNode::needsBoundingBoxUpdate() 
{ 
	bool needbbupdate = false;
	foreach(NodeComponent* d, myObjects) needbbupdate |= d->needsBoundingBoxUpdate();
	if(needbbupdate) return true;

	foreach(Node* n, getChildren())
	{
		SceneNode* child = dynamic_cast<SceneNode*>(n);
		needbbupdate |= child->needsBoundingBoxUpdate();
	}
	return needbbupdate;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3& SceneNode::getBoundingBox() 
{ 
	updateBoundingBox();
	return myBBox; 
}

///////////////////////////////////////////////////////////////////////////////////////////////
const Sphere& SceneNode::getBoundingSphere() 
{ 
	updateBoundingBox();
	return myBSphere; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f& SceneNode::getBoundMinimum()
{
	updateBoundingBox();
	return myBBox.getMinimum();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f& SceneNode::getBoundMaximum()
{
	updateBoundingBox();
	return myBBox.getMaximum();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f SceneNode::getBoundCenter()
{
	updateBoundingBox();
	return myBBox.getCenter();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float SceneNode::getBoundRadius()
{
	updateBoundingBox();
	return myBSphere.getRadius();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::updateBoundingBox(bool force)
{
	// Exit now if bouning box does not need an update.
	if(!force && !needsBoundingBoxUpdate()) return;

	// Reset bounding box.
	myBBox.setNull();

	foreach(NodeComponent* d, myObjects)
	{
		if(d->hasBoundingBox())
		{
			if(d->needsBoundingBoxUpdate()) d->updateBoundingBox();
			const AlignedBox3& bbox = *(d->getBoundingBox());
			myBBox.merge(bbox);
		}
	}

	myBBox.transformAffine(getFullTransform());

	foreach(Node* child, getChildren())
	{
		SceneNode* n = dynamic_cast<SceneNode*>(child);
		if(n != NULL)
		{
			const AlignedBox3& bbox = n->getBoundingBox();
			myBBox.merge(bbox);
		}
	}

	if(!myBBox.isNull())
	{
		// Compute bounding sphere.
		myBSphere = Sphere(myBBox.getCenter(), myBBox.getHalfSize().maxCoeff());
	}

	SceneNode* parent = dynamic_cast<SceneNode*>(getParent());
	if(parent != NULL)
	{
		parent->updateBoundingBox(true);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::drawBoundingBox()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GREATER);
	
	glColor3fv(myBoundingBoxColor.data());
	glBegin(GL_LINES);

	glVertex3f(myBBox[0][0], myBBox[0][1], myBBox[0][2]);
	glVertex3f(myBBox[1][0], myBBox[0][1], myBBox[0][2]);

	glVertex3f(myBBox[0][0], myBBox[0][1], myBBox[0][2]);
	glVertex3f(myBBox[0][0], myBBox[0][1], myBBox[1][2]);

	glVertex3f(myBBox[0][0], myBBox[0][1], myBBox[0][2]);
	glVertex3f(myBBox[0][0], myBBox[1][1], myBBox[0][2]);

	glVertex3f(myBBox[1][0], myBBox[1][1], myBBox[1][2]);
	glVertex3f(myBBox[1][0], myBBox[0][1], myBBox[1][2]);

	glVertex3f(myBBox[1][0], myBBox[1][1], myBBox[1][2]);
	glVertex3f(myBBox[1][0], myBBox[1][1], myBBox[0][2]);

	glVertex3f(myBBox[1][0], myBBox[1][1], myBBox[1][2]);
	glVertex3f(myBBox[0][0], myBBox[1][1], myBBox[1][2]);

	glVertex3f(myBBox[1][0], myBBox[0][1], myBBox[1][2]);
	glVertex3f(myBBox[0][0], myBBox[0][1], myBBox[1][2]);

	glVertex3f(myBBox[1][0], myBBox[0][1], myBBox[1][2]);
	glVertex3f(myBBox[1][0], myBBox[0][1], myBBox[0][2]);

	glVertex3f(myBBox[1][0], myBBox[1][1], myBBox[0][2]);
	glVertex3f(myBBox[0][0], myBBox[1][1], myBBox[0][2]);

	glVertex3f(myBBox[1][0], myBBox[1][1], myBBox[0][2]);
	glVertex3f(myBBox[1][0], myBBox[0][1], myBBox[0][2]);

	glVertex3f(myBBox[0][0], myBBox[1][1], myBBox[1][2]);
	glVertex3f(myBBox[0][0], myBBox[0][1], myBBox[1][2]);

	glVertex3f(myBBox[0][0], myBBox[1][1], myBBox[1][2]);
	glVertex3f(myBBox[0][0], myBBox[1][1], myBBox[0][2]);
	glEnd();

	glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneNode::hit(const Ray& ray, Vector3f* hitPoint, HitType hitType)
{
	if(hitType == HitBest)
	{
		bool hasCustomIntersectors = false;
		foreach(NodeComponent* iso, myObjects)
		{
			if(iso->hasCustomRayIntersector())
			{
				hasCustomIntersectors = true;
				if(iso->intersectRay(ray, hitPoint)) return true;
			}
		}

		// If no attached scene object has a custom ray intersector, fall back to the bounding sphere technique
		if(!hasCustomIntersectors) hitType = HitBoundingSphere;
	}

	if(hitType == HitBoundingSphere)
	{
		const Sphere& s = getBoundingSphere();
		std::pair<bool, float> h = ray.intersects(s);
		if(h.first)
		{
			(*hitPoint) = ray.getPoint(h.second);
		}
		return h.first;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::followTrackable(int trackableId)
{
	if(myTracker == NULL)
	{
		myTracker = new TrackedObject();
		ModuleServices::addModule(myTracker);
		myTracker->setSceneNode(this);
	}
	myTracker->setTrackableSourceId(trackableId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::setFollowOffset(const Vector3f& offset, const Quaternion& ooffset)
{
	myTracker->setOffset(offset);
	myTracker->setOrientationOffset(ooffset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::unfollow()
{
	if(myTracker != NULL)
	{
		ModuleServices::removeModule(myTracker);
		delete myTracker;
		myTracker = NULL;
	}
}
