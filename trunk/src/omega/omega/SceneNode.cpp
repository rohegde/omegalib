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
#include "omega/DrawInterface.h"
#include "omega/SceneNode.h"
#include "omega/Renderable.h"
#include "omega/Engine.h"
#include "omega/glheaders.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* SceneNode::create(const String& name)
{
	return new SceneNode(Engine::instance(), name);
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
void SceneNode::addObject(ISceneObject* o) 
{ 
	myObjects.push_back(o); 
	//needUpdate();
	updateBoundingBox();
	// If the object has not been initialized yet, do it now.
	if(!o->isInitialized()) o->initialize(myServer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SceneNode::getNumObjects()
{ 
	return myObjects.size(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::removeObject(ISceneObject* o) 
{
	myObjects.remove(o);
	updateBoundingBox();
	//needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::draw(RenderState* state)
{
	if(myVisible)
	{
		//if(myChanged) updateTransform();

		if(myBoundingBoxVisible) drawBoundingBox();

		// Draw drawables attached to this node.
		foreach(ISceneObject* d, myObjects)
		{
			d->draw(this, state);
		}

		// Draw children nodes.
		foreach(Node* child, getChildren())
		{
			SceneNode* n = dynamic_cast<SceneNode*>(child);
			n->draw(state);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::update(bool updateChildren, bool parentHasChanged)
{
    // Short circuit the off case
    if (!updateChildren && !mNeedParentUpdate && !mNeedChildUpdate && !parentHasChanged)
    {
        return;
    }

	Node::update(updateChildren, parentHasChanged);

	updateBoundingBox();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::updateBoundingBox()
{
	// Reset bounding box.
	myBBox.setNull();

	foreach(ISceneObject* d, myObjects)
	{
		d->update(this);
		if(d->hasBoundingBox())
		{
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
		foreach(ISceneObject* iso, myObjects)
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
