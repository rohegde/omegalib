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
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "omega/osystem.h"
#include "omega/GpuManager.h"
#include "omega/scene/SceneNode.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class SceneManager
	{
	public:
		SceneManager(omega::GpuManager* gpu): myGpuMng(gpu) {}

		GpuManager* getGpuManager() { return myGpuMng; }
		SceneNode* getRootNode() { return &myRoot; }

		OUTILS_API void draw();

	private:
		omega::GpuManager* myGpuMng;
		SceneNode myRoot;
	};
}; // namespace scene
}; // namespace omega

#endif