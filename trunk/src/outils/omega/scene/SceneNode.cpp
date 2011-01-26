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

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::addChild(SceneNode* child) 
{ 
	myChildren.push_back(child); child->myParent = this; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::draw()
{
	if(myChanged) updateTransform();

	glPushMatrix();

	// Load the world transform matrix into gl.
	glLoadMatrixf(myScene->getViewTransform().begin());

	if(myBoundingBoxVisible) drawBoundingBox();

	glMultMatrixf(myWorldTransform.begin());

	// Draw drawables attached to this node.
	boost_foreach(Drawable* d, myDrawables)
	{
		d->draw(this);
	}
	// Draw children nodes.
	boost_foreach(SceneNode* n, myChildren)
	{
		n->draw();
	}

	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::updateTransform()
{
	// Compute local transform
	myLocalTransform = Matrix4f::IDENTITY;
	myLocalTransform.set_translation(myPosition);
	myLocalTransform.rotate_x(myRotation[0]);
	myLocalTransform.rotate_y(myRotation[1]);
	myLocalTransform.rotate_z(myRotation[2]);
	myLocalTransform.scale(myScale);

	if(myParent != NULL) myWorldTransform = myParent->myWorldTransform * myLocalTransform;
	else myWorldTransform = myLocalTransform;

	boost_foreach(SceneNode* n, myChildren)
	{
		n->updateTransform();
	}

	// Reset bounding box.
	myBBox.setNull();

	boost_foreach(Drawable* d, myDrawables)
	{
		if(d->hasBoundingBox())
		{
			const AxisAlignedBox& bbox = *(d->getBoundingBox());
			myBBox.merge(bbox);
		}
	}
	boost_foreach(SceneNode* n, myChildren)
	{
		const AxisAlignedBox& bbox = n->getBoundingBox();
		myBBox.merge(bbox);
	}

	myBBox.transformAffine(myWorldTransform);

	// Compute bounding sphere.
	myBSphere = Sphere(myBBox.getCenter(), myBBox.getHalfSize().find_max());

	myChanged = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::drawBoundingBox()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GREATER);
	
	glColor3fv(myBoundingBoxColor.begin());
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