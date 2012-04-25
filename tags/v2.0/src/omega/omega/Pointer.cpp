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
#include "omega/Pointer.h"
#include "omega/DrawInterface.h"

#include "omega/glheaders.h"

using namespace omega;


///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Pointer::createRenderable()
{
	return new PointerRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PointerRenderable::draw(RenderState* state)
{
	if(myPointer->myVisible)
	{
		if(state->context->task == DrawContext::OverlayDrawTask && 
			myPointer->getPointerMode() == Pointer::ModeMouse)
		{
			int size = 30;
			int x = myPointer->myPosition[0];
			int y = myPointer->myPosition[1];

			glColor4fv(myPointer->myColor.data());
			glBegin(GL_TRIANGLES);
			glVertex2i(x, y);
			glVertex2i(x + size, y + size / 2);
			glVertex2i(x + size / 2, y + size);
			glEnd();

			glColor4f(1, 1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex2i(x, y);
			glVertex2i(x + size, y + size / 2);
			glVertex2i(x + size / 2, y + size);
			glEnd();

			Font* fnt = getRenderer()->getDefaultFont();
			if(fnt != NULL)
			{
				getRenderer()->drawText(myPointer->myText, fnt, Vector2f(x + size, y + size), Font::HALeft | Font::VABottom);
			}
		}
		if(state->context->task == DrawContext::SceneDrawTask && 
			myPointer->getPointerMode() == Pointer::ModeWand)
		{
			glDisable(GL_LIGHTING);

			float size = myPointer->myPointerSize;
			float length = myPointer->myPointerLength;
			float px = myPointer->myRay.getOrigin().x();
			float py = myPointer->myRay.getOrigin().y();
			float pz = myPointer->myRay.getOrigin().z();

			float x = px + myPointer->myRay.getDirection().x() * length;
			float y = py + myPointer->myRay.getDirection().y() * length;
			float z = pz + myPointer->myRay.getDirection().z() * length;

			glColor4fv(myPointer->myColor.data());
			glBegin(GL_TRIANGLES);
			glVertex3f(x, y, z);
			glVertex3f(px + size, py, pz);
			glVertex3f(px - size, py, pz);
			glVertex3f(x, y, z);
			glVertex3f(px, py + size, pz);
			glVertex3f(px, py - size, pz);
			glEnd();

			glColor4f(1, 1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex3f(x, y, z);
			glVertex3f(px + size, py, pz);
			glVertex3f(px - size, py, pz);
			glVertex3f(x, y, z);
			glVertex3f(px, py + size, pz);
			glVertex3f(px, py - size, pz);
			glEnd();
		}
	}
}
