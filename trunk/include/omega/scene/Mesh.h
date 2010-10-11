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
#include "omega/scene/Material.h"
#include "omega/scene/Drawable.h"
#include "omega/scene/SceneManager.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @warning This is a work in progress! It may be deeply modified or removed altogether in future versions.
	class Mesh: public Drawable
	{
	public:
		Mesh(): myVertexBuffer(NULL), myIndexBuffer(NULL), myMaterial(NULL) {}

		VertexBuffer* getVertexBuffer() { return myVertexBuffer; }
		IndexBuffer* getIndexBuffer()  { return myIndexBuffer; }

		Material* getMaterial() { return myMaterial; }
		void setMaterial(Material* value) { myMaterial = value; }

		OUTILS_API void initialize(SceneManager* scene, int numVertices, int vertexSize, void* vertexData, void* indexData);
		OUTILS_API virtual void draw();

	private:
		VertexBuffer* myVertexBuffer;
		IndexBuffer* myIndexBuffer;
		Material* myMaterial;
	};

}; // namespace scene
}; // namespace omega

#endif