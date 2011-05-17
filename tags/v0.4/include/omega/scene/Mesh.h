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
#include "omega/scene/Renderable.h"
#include "omega/scene/SceneManager.h"
#include "omega/scene/MeshData.h"
#include "omega/scene/RenderPass.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class MeshManager;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API Mesh: public Renderable
	{
	public:
		Mesh(MeshManager* manager);

		MeshManager* getManager() { return myMng; }

		Effect* getEffect() { return myEffect; }
		void setEffect(Effect* value) { myEffect = value; }

		MeshData* getData() { return myData; }
		void setData(MeshData* value) { myData = value; }

		virtual void render(SceneNode* node, RenderState* state);
		void update();

		const AlignedBox3* getBoundingBox();
		bool hasBoundingBox();

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