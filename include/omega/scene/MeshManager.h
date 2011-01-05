/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__

#include "omega/osystem.h"
#include "omega/scene/EffectManager.h"
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
	//! Loads and manages Meshes.
	class MeshManager
	{
	public:
		enum MeshFormat { MeshFormatPly };

	public:
		OUTILS_API MeshManager(omega::GpuManager* gpu, EffectManager* effectMng);
		OUTILS_API ~MeshManager();

		GpuManager* getGpuManager() { return myGpuMng; }
		EffectManager* getEffectManager() { return myEffectMng; }

		OUTILS_API void cleanup();

		OUTILS_API void loadMesh(omega::String name, omega::String filename, MeshFormat format);
		OUTILS_API Mesh* getMesh(omega::String name);
		OUTILS_API void addMesh(omega::String name, MeshData* data);

	private:
		MeshDictionary myMeshes;
		omega::GpuManager* myGpuMng;
		EffectManager* myEffectMng;
	};

}; // namespace scene
}; // namespace omega

#endif