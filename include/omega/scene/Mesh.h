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
#ifndef __MESH_H__
#define __MESH_H__

#include "omega/osystem.h"
#include "omega/GpuBuffer.h"
#include "omega/scene/Effect.h"
#include "omega/scene/Drawable.h"
#include "omega/scene/SceneManager.h"
#include "omega/scene/MeshData.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class MeshManager;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class Mesh: public Drawable
	{
	public:
		Mesh(MeshManager* manager);

		MeshManager* getManager() { return myMng; }

		Effect* getEffect() { return myEffect; }
		void setEffect(Effect* value) { myEffect = value; }

		MeshData* getData() { return myData; }
		void setData(MeshData* value) { myData = value; }

		OUTILS_API virtual void draw();
		OUTILS_API void update();

	private:
		VertexBuffer* myVertexBuffer;
		unsigned int* myIndexData;

		Effect* myEffect;
		MeshData* myData;
		MeshManager* myMng;
	};

}; // namespace scene
}; // namespace omega

#endif