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
#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__

#include "omega/osystem.h"
#include "omega/scene/Mesh.h"
#include "omega/scene/PlyDataReader.h"

#include "boost/unordered_map.hpp"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! A dictionary containing <String, Mesh*> pairs.
	typedef boost::unordered_map<omega::String, Mesh*> MeshDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Loads images and manages OpenGL textures.
	class MeshManager
	{
	public:
		enum MeshFormat { MeshFormatPly };

	public:
		OUTILS_API MeshManager();
		OUTILS_API ~MeshManager();

		OUTILS_API void cleanup();

		OUTILS_API void loadMesh(omega::String name, omega::String filename, MeshFormat format);
		OUTILS_API Mesh* getMesh(omega::String name);

	private:
		void createMesh(omega::String name, MeshData* data);

	private:
		MeshDictionary myMeshes;
	};

}; // namespace scene
}; // namespace omega

#endif