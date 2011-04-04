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
#include "omega/scene/SceneManager.h"
#include "omega/glheaders.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initialize()
{
	myRoot = new SceneNode(this);
	myBackgroundColor = Color(0.1f, 0.1f, 0.15f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::draw(const Rect& viewport)
{
	glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();

	//// Draw background
	//glDisable(GL_DEPTH_TEST);

	//glColor4fv(myBackgroundColor.data());
	//int width = viewport.width();
	//int height = viewport.height();
	//int x = - width / 2;
	//int y = - height / 2;
	//glRectf(-1, -1, 1, 1);
	//glEnable(GL_DEPTH_TEST);

	//glPopMatrix();

	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();

	// Setup view matrix (read back from gl)
	glGetFloatv( GL_MODELVIEW_MATRIX, myViewTransform.data() );

	// For scene node drawing, we are not using the gl matrix stack, we are using our own transforms,
	// stored inside the scene nodes. So, create a new, clean transform on the stack.
	glPushMatrix();
	glLoadIdentity();

	// Update transform hierarchy
	myRoot->update(false, false);

	// Draw background
	//glDisable(GL_DEPTH_TEST);

	// Draw scene
	myRoot->draw();
	glPopMatrix();
}
