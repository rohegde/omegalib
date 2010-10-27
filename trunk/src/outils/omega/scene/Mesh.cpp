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

#include "omega/scene/Mesh.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::initialize(SceneManager* scene, int numVertices, int vertexSize, void* vertexData, void* indexData)
{
	GpuManager* gpuMng = scene->getGpuManager();
	myVertexBuffer = new VertexBuffer(gpuMng);
	myIndexBuffer = new IndexBuffer(gpuMng);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::draw()
{
}

