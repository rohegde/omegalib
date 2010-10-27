/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/scene/MeshManager.h"
#include "omega/scene/ply.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshManager::MeshManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshManager::~MeshManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::loadMesh(omega::String name, omega::String filename, MeshFormat format)
{
	if(format == MeshManager::MeshFormatPly)
	{
		PlyDataReader reader;
		reader.readPlyFile(filename);

		createMesh(name, &reader);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mesh* MeshManager::getMesh(omega::String name)
{
	return myMeshes[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::cleanup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::createMesh(omega::String name, MeshData* data)
{
	Mesh* mesh = new Mesh();
	myMeshes[name] = mesh;

	// TODO: fill the mesh index and vertex buffers with data.
}
