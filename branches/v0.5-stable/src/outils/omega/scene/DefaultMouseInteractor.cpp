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
#include "omega/scene/DefaultMouseInteractor.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////
bool DefaultMouseInteractor::handleEvent(const Event& evt, DrawContext& context) 
{
	// Select objects (use a positive z layer since objects in this program usually lie on the projection plane)
	float z = 1.0f;
	Ray ray = Math::unproject(Vector2f(evt.getPosition(0), evt.getPosition(1)), context.modelview, context.projection, context.viewport, z);

	if(myNode != NULL)
	{
		if(evt.getType() == Event::Down)
		{
			Vector3f handlePos;
			if(myNode->hit(ray, &handlePos, SceneNode::HitBoundingSphere))
			{
				myStartBSphere = myNode->getBoundingSphere();
				myStartOrientation = myNode->getOrientation();
				myStartScale = myNode->getScale()[0];
				myHandlePosition = handlePos; 
				myNode->setSelected(true);
			}
		}
		else if(evt.getType() == Event::Up)
		{
			myNode->setSelected(false);
		}
		else if(evt.getType() == Event::Move)
		{
			// Manipulate object, if one is active.
			if(myNode->isSelected())
			{
				Ray ray = Math::unproject(Vector2f(evt.getPosition(0), evt.getPosition(1)), context.modelview, context.projection, context.viewport, z);

				if(evt.isFlagSet(Event::Left))
				{
					Vector3f origin = ray.getOrigin();
					Vector3f direction = ray.getDirection();
					// Interstect the ray with the Z plane where the handle lies, to get
					// the new handle position.
					float tz = myHandlePosition[2] + myStartBSphere.getCenter()[2];
					float l = (tz - origin[2]) / direction[2];
					float tx = origin[0] + l * direction[0];
					float ty = origin[1] + l * direction[1];

					Vector3f newPos = Vector3f(tx, ty, tz) - myHandlePosition;
					myNode->setPosition(newPos);
				}
				else if(evt.isFlagSet(Event::Right))
				{
					// Intersect the ray with the bounding sphere. 
					// If the point is outside the bounding sphere, perform no rotation.
					std::pair<bool, float> p = ray.intersects(myStartBSphere);
					if(p.first)
					{
						Vector3f pt = ray.getPoint(p.second);
						pt -= myStartBSphere.getCenter();
						Quaternion rot = Math::buildRotation(myHandlePosition, pt , Vector3f::Zero() );
						myNode->setOrientation(rot * myStartOrientation);
					}
				}
			}
		}
		else if(evt.getType() == Event::Zoom)
		{
			// Manipulate object, if one is active.
			float sc;
			if(evt.getExtraDataInt(0) < 0) sc = 0.9f;
			else sc = 1.1f;
			myNode->scale(sc, sc, sc);
		}
	}
	return false; 
}
