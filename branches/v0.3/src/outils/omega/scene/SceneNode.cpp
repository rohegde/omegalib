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
	boost_foreach(Drawable* d, myDrawables)
	{
		d->draw();
	}
}
