/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __MESH_H__
#define __MESH_H__

#include "oenginebase.h"
#include "Effect.h"
#include "Renderable.h"
#include "MeshData.h"
#include "RenderPass.h"
#include "SceneObject.h"
#include "omega/GpuBuffer.h"

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API Mesh: public SceneObject
	{
	OMEGA_DECLARE_TYPE(Mesh)
	public:
		Mesh();
		~Mesh();
		virtual Renderable* createRenderable();

		MeshData* getData();
		void setData(MeshData* value);

		const AlignedBox3* getBoundingBox();
		bool hasBoundingBox();

		void setEffect(Effect* value) { myEffect = value; }
		Effect* getEffect() { return myEffect; }

	private:
		MeshData* myData;
		Effect* myEffect;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API MeshRenderable: public SceneRenderable
	{
	OMEGA_DECLARE_TYPE(MeshRenderable)
	public:
		MeshRenderable(Mesh* mesh);
		~MeshRenderable();
		virtual void initialize();
		virtual void dispose();
		void draw(RenderState* state);
		void refresh();

	private:
		Mesh* myMesh;
		VertexBuffer* myVertexBuffer;
		unsigned int* myIndexData;
	};
}; // namespace oengine

#endif