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
MeshManager::MeshManager(omega::GpuManager* gpu, EffectManager* effectMng): myGpuMng(gpu), myEffectMng(effectMng)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshManager::~MeshManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::loadMesh(omega::String name, omega::String filename, MeshFormat format, bool rescale)
{
	if(format == MeshManager::MeshFormatPly)
	{
		PlyDataReader* reader = new PlyDataReader();
		reader->readPlyFile(filename);
		if(rescale)	reader->scale(1);

		addMesh(name, reader);
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
void MeshManager::addMesh(omega::String name, MeshData* data)
{
	Mesh* mesh = new Mesh(this);
	myMeshes[name] = mesh;
	mesh->setData(data);
	mesh->update();
}
