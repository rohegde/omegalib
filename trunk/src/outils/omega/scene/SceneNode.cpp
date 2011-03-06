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

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::addDrawable(Drawable* child) 
{ 
	myDrawables.push_back(child); 
	needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SceneNode::getNumDrawables()
{ 
	return myDrawables.size(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::clearDrawables() 
{
	myDrawables.empty();
	needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::draw()
{
	if(myVisible)
	{
		//if(myChanged) updateTransform();

		glPushMatrix();

		// Load the world transform matrix into gl.
		glLoadMatrixf(myScene->getViewTransform().data());

		if(myBoundingBoxVisible) drawBoundingBox();

		glMultMatrixf(getFullTransform().data());

		// Draw drawables attached to this node.
		VectorIterator<Vector<Drawable*> > it(myDrawables);
		while(it.hasMoreElements())
		{
			Drawable* d = it.getNext();
			d->draw(this);
		}
		// Draw children nodes.
		ChildNodeIterator i = getChildIterator();
		while(i.hasMoreElements())
		{
			SceneNode* n = (SceneNode*)i.getNext();
			n->draw();
		}

		glPopMatrix();
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

	VectorIterator<Vector<Drawable*> > it(myDrawables);
	while(it.hasMoreElements())
	{
		Drawable* d = it.getNext();
		if(d->hasBoundingBox())
		{
			const AxisAlignedBox& bbox = *(d->getBoundingBox());
			myBBox.merge(bbox);
		}
	}
	ChildNodeIterator i = getChildIterator();
	while(i.hasMoreElements())
	{
		SceneNode* n = (SceneNode*)i.getNext();
		const AxisAlignedBox& bbox = n->getBoundingBox();
		myBBox.merge(bbox);
	}

	myBBox.transformAffine(getFullTransform());

	// Compute bounding sphere.
	myBSphere = Sphere(myBBox.getCenter(), myBBox.getHalfSize().maxCoeff());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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