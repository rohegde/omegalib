/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/scene/SceneNode.h"
#include "omega/scene/SceneManager.h"
#include "omega/glheaders.h"
#include "omega/StringUtils.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::setListener(SceneNodeListener* listener)
{
	if(listener != NULL && myListener != NULL)
	{
		owarn("Node switched listener: old listener won't receive notification anymore.");
	}

	myListener = listener;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneNodeListener* SceneNode::getListener()
{
	return myListener;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneNode::isVisible()
{ 
	return myVisible; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::setVisible(bool value)
{ 
	if(myListener) myListener->onVisibleChanged(this, value);
	myVisible = value; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::setSelected(bool value)
{
	if(myListener) myListener->onSelectedChanged(this, value);
	mySelected = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneNode::isSelected()
{
	return mySelected;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::addRenderable(Renderable* child) 
{ 
	myRenderables.push_back(child); 
	needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SceneNode::getNumRenderables()
{ 
	return myRenderables.size(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::clearRenderables() 
{
	myRenderables.empty();
	needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::draw(RenderState* state)
{
	if(myVisible)
	{
		//if(myChanged) updateTransform();

		if(myBoundingBoxVisible) drawBoundingBox();

		//glMultMatrixf(getFullTransform().data());

		glPushMatrix();
		glMultMatrixf(getFullTransform().data());
		// Draw drawables attached to this node.
		VectorIterator<Vector<Renderable*> > it(myRenderables);
		while(it.hasMoreElements())
		{
			Renderable* d = it.getNext();
			d->render(this, state);
		}
		glPopMatrix();

		// Draw children nodes.
		ChildNodeIterator i = getChildIterator();
		while(i.hasMoreElements())
		{
			SceneNode* n = (SceneNode*)i.getNext();
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

	// Reset bounding box.
	myBBox.setNull();

	VectorIterator<Vector<Renderable*> > it(myRenderables);
	while(it.hasMoreElements())
	{
		Renderable* d = it.getNext();
		if(d->hasBoundingBox())
		{
			const AlignedBox3& bbox = *(d->getBoundingBox());
			myBBox.merge(bbox);
		}
	}

	myBBox.transformAffine(getFullTransform());

	ChildNodeIterator i = getChildIterator();
	while(i.hasMoreElements())
	{
		SceneNode* n = (SceneNode*)i.getNext();
		const AlignedBox3& bbox = n->getBoundingBox();
		myBBox.merge(bbox);
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
	if(hitType == HitBoundingSphere)
	{
		const Sphere& s = getBoundingSphere();
		std::pair<bool, float> h = ray.intersects(s);
		if(h.first)
		{
			(*hitPoint) = ray.getPoint(h.second) - s.getCenter();
		}
		return h.first;
	}
	return false;
}
