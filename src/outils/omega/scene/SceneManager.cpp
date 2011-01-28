/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * DrawContext
 *********************************************************************************************************************/

#include "omega/scene/SceneManager.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initialize()
{
	myRoot = new SceneNode(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::draw()
{
	// Setup view matrix (read back from gl)
	glGetFloatv( GL_MODELVIEW_MATRIX, myViewTransform.begin() );

	// For scene node drawing, we are not using the gl matrix stack, we are using our own transforms,
	// stored inside the scene nodes. So, create a new, clean transform on the stack.
	glPushMatrix();
	glLoadIdentity();

	// Update transform hierarchy
	myRoot->update(true, true);

	myRoot->draw();
	glPopMatrix();
}
