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
#include "meshviewer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void SelectionSphere::draw(SceneNode* node)
{
	//if(myVisible)
	{
		float radius = node->getBoundingSphere().getRadius();
		float stp = Math::Pi * 2 / 32;
		float stp2 = Math::Pi * 2 / 16;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.8f, 0.8f, 1.0f, 0.2f);
	
		for(float g = 0; g < 2 * Math::Pi; g+= stp2)
		{
			glBegin(GL_LINE_LOOP);
			for(float t = 0; t < 2 * Math::Pi; t+= stp)
			{
				float ptx = Math::sin(t) * Math::sin(g) * radius;
				float pty = Math::cos(t) * Math::sin(g) * radius;
				float ptz = Math::cos(g) * radius;
				glVertex3f(ptx, pty, ptz);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for(float t = 0; t < 2 * Math::Pi; t+= stp)
			{
				float ptz = Math::sin(t) * Math::sin(g) * radius;
				float pty = Math::cos(t) * Math::sin(g) * radius;
				float ptx = Math::cos(g) * radius;
				glVertex3f(ptx, pty, ptz);
			}
			glEnd();
		}

		// Draw handle positions;
		//glColor4f(1.0, 0.0, 0.0, 1.0);
		//glPointSize(16);
		//glBegin(GL_POINTS);
		//glVertex3fv(myEntity->getHandlePosition().begin());
		//glEnd();
		//glPointSize(1);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(const String& name, SceneManager* sm, Mesh* m)
{
	myName = name;
	myMesh = m;
	mySelectionSphere = new SelectionSphere(this);
	mySelectionSphere->setVisible(true);
	myVisible = false;

	mySceneNode = new SceneNode(sm);
	mySceneNode->addDrawable(myMesh);
	mySceneNode->addDrawable(mySelectionSphere);
	mySceneNode->setPosition(Vector3f::ZERO);
	mySceneNode->setSelectable(true);
	mySceneNode->setVisible(false);
	sm->getRootNode()->addChild(mySceneNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setVisible(bool value)
{
	myVisible = value;
	mySceneNode->setVisible(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::resetTransform()
{
	mySceneNode->setPosition(0, 0, 0);
	mySceneNode->resetOrientation();
	mySceneNode->setScale(0.2f, 0.2f, 0.2f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::activate(const Vector3f handlePos)
{
	myActive = true;
	mySelectionSphere->setVisible(true);

	myStartBSphere = mySceneNode->getBoundingSphere();
	myStartOrientation = mySceneNode->getOrientation();
	myStartScale = mySceneNode->getScale()[0];
	myHandlePosition = handlePos; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::deactivate()
{
	myActive = false;
	mySelectionSphere->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::hit(const Ray& ray, Vector3f* handlePos)
{
	const Sphere& s = mySceneNode->getBoundingSphere();
	std::pair<bool, float> h = ray.intersects(s);
	if(h.first)
	{
		(*handlePos) = ray.getPoint(h.second) - s.getCenter();
	}
	return h.first;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::manipulate(Operation op, const Ray& ray1, const Ray& ray2)
{
	if(op == Move)
	{
		Vector3f origin = ray1.getOrigin();
		Vector3f direction = ray1.getDirection();
		// Interstect the ray with the Z plane where the handle lies, to get
		// the new handle position.
		float tz = myHandlePosition[2] + myStartBSphere.getCenter()[2];
		float l = (tz - origin[2]) / direction[2];
		float tx = origin[0] + l * direction[0];
		float ty = origin[1] + l * direction[1];

		Vector3f newPos = Vector3f(tx, ty, tz) - myHandlePosition;
		mySceneNode->setPosition(newPos);
	}
	else if(op == Rotate)
	{
		// Intersect the ray with the bounding sphere. 
		// If the point is outside the bounding sphere, perform no rotation.
		std::pair<bool, float> p = ray1.intersects(myStartBSphere);
		if(p.first)
		{
			Vector3f pt = ray1.getPoint(p.second);
			pt -= myStartBSphere.getCenter();
			Quaternion rot = Math::buildRotation(myHandlePosition, pt);
			mySceneNode->setOrientation(rot * myStartOrientation);
		}
	}
	else if(op == Scale)
	{
		Vector3f origin = ray1.getOrigin();
		Vector3f direction = ray1.getDirection();
		// Interstect the ray with the Z plane where the handle lies, to get
		// the new handle position.
		float tz = myHandlePosition[2] + myStartBSphere.getCenter()[2];
		float l = (tz - origin[2]) / direction[2];
		float tx = origin[0] + l * direction[0];
		float ty = origin[1] + l * direction[1];

		Vector3f newPos = Vector3f(tx, ty, tz);// - myHandlePosition;
		float d = (newPos - myStartBSphere.getCenter()).length();

		float scale = myStartScale * d / myStartBSphere.getRadius();

		mySceneNode->setScale(scale, scale, scale);
	}
}

