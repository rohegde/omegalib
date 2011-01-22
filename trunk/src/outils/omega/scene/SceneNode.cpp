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

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::pushTransform()
{
	glPushMatrix();

	glTranslatef(myPosition[0], myPosition[1], myPosition[2]);

	glRotatef(myRotation[0], 1, 0, 0);
	glRotatef(myRotation[1], 0, 1, 0);
	glRotatef(myRotation[2], 0, 0, 1);

	glScalef(myScale[0], myScale[1], myScale[2]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::popTransform()
{
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::draw()
{
	pushTransform();
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

	if(myBoundingBoxVisible) drawBoundingBox();
	popTransform();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::updateBounds()
{
	// Reset bounding box.
	float fmax = std::numeric_limits<float>::max();
	float fmin = -std::numeric_limits<float>::max();
	myBBox[0][0] = myBBox[0][1] = myBBox[0][2] = fmax;
	myBBox[1][0] = myBBox[1][1] = myBBox[1][2] = fmin;

	// Draw drawables attached to this node.
	boost_foreach(Drawable* d, myDrawables)
	{
		if(d->hasBoundingBox())
		{
			const AxisAlignedBox& bbox = *(d->getBoundingBox());
			for(int i = 0; i < 3; i++)
			{
				if(bbox[0][i] < myBBox [0][i]) myBBox[0][i] = bbox[0][i];
				if(bbox[1][i] > myBBox[1][i]) myBBox[1][i] = bbox[1][i];
			}
		}
	}
	// Draw children nodes.
	boost_foreach(SceneNode* n, myChildren)
	{
		const AxisAlignedBox& bbox = n->getBoundingBox();
		for(int i = 0; i < 3; i++)
		{
			if(bbox[0][i] < myBBox [0][i]) myBBox[0][i] = bbox[0][i];
			if(bbox[1][i] > myBBox[1][i]) myBBox[1][i] = bbox[1][i];
		}
	}

	myChanged = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneNode::drawBoundingBox()
{
	if(myChanged) updateBounds();

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