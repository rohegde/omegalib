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
#include "omega/scene/SceneNode.h"
#include "omega/scene/BoundingSphereDrawable.h"

#include "omega/glheaders.h"

using namespace omega;
using namespace scene;

///////////////////////////////////////////////////////////////////////////////////////////////////
void BoundingSphereDrawable::draw(SceneNode* node)
{
	if(myVisible || (myDrawOnSelected && node->isSelected()))
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
	}
}
