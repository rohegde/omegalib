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
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "omega/osystem.h"
#include "omega/GpuEnvironment.h"
#include "omega/GpuProgram.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class Mesh;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class Material
	{
	public:
		Material(): myProgram(NULL) {}

		void setProgram(GpuProgram* program) { myProgram = program; }
		GpuProgram* getProgram() { return myProgram; }
		
		GpuEnvironment& getEnvironment() { return myEnvironment; }

		OUTILS_API void activate();
		OUTILS_API void deactivate();

		//! Draws a mesh using this material.
		OUTILS_API void draw(Mesh* mesh);

	private:
		GpuEnvironment myEnvironment;
		GpuProgram* myProgram;
	};

}; // namespace scene
}; // namespace omega

#endif