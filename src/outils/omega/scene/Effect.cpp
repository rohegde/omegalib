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

#include "omega/scene/Effect.h"
#include "omega/scene/Mesh.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect::activate()
{
	myParams.bind(myProgram, GpuProgram::RenderStage);
	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_AMBIENT, myAmbientColor.begin());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, myDiffuseColor.begin());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mySpecularColor.begin());
	glMaterialfv(GL_FRONT, GL_EMISSION, myEmissiveColor.begin());
	glMaterialf(GL_FRONT, GL_SHININESS, myShininess);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Effect::deactivate()
{
	myParams.unbind(myProgram, GpuProgram::RenderStage);
}

