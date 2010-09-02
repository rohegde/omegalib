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

#include "omega/scene/Material.h"
#include "omega/scene/Mesh.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::activate()
{
	myEnvironment.push();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::deactivate()
{
	myEnvironment.pop();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::draw(Mesh* mesh)
{
	oassert(mesh != NULL);

	if(myProgram != NULL)
	{
		myProgram->setInput(0, mesh->getVertexBuffer());
		myProgram->setInput(1, mesh->getIndexBuffer());
		myProgram->run(GpuProgram::PrimTriangles);
	}
}
