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
#ifndef __CUBE_H__
#define __CUBE_H__

#include "omega/osystem.h"
#include "omega/scene/Mesh.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class MeshUtils
	{
	public:
		OUTILS_API Mesh* createCube(float size);

	private:
		MeshUtils() {}
	};
}; // namespace scene
}; // namespace omega

#endif