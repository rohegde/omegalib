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

#include "omega/scene/Mesh.h"
#include "omega/scene/MeshManager.h"
#include "omega/scene/DefaultRenderPass.h"
#include "omega/Renderer.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(MeshManager* manager): myVertexBuffer(NULL), myIndexData(NULL), myData(NULL), myMng(manager) 
{
	myEffect = myMng->getEffectManager()->getDefaultEffect();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::render(SceneNode* node, RenderState* state)
{
	if(state->isFlagSet(RenderPass::RenderOpaque))
	{
		state->renderer->pushTransform(node->getFullTransform());
		if(myEffect != NULL)
		{
			myEffect->activate();
			myVertexBuffer->bind();
			RenderStageOptions options;
			options.items = myData->getNumTriangles() * 3;
			options.primType = RenderStageOptions::PrimTriangles;
			options.indices = myIndexData;
			myEffect->getProgram()->runRenderStage(options);
			myEffect->deactivate();
		}
		state->renderer->popTransform();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* Mesh::getBoundingBox()
{
	return &myData->getBoundingBox();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Mesh::hasBoundingBox()
{
	if(myData != NULL) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::update()
{
	if(myVertexBuffer != NULL)
	{
		delete myVertexBuffer;
		myVertexBuffer = NULL;
	}

	if(myIndexData != NULL)
	{
		delete myIndexData;
	}

	// Allocate index and vertex buffers.
	GpuManager* gpuMng = myMng->getGpuManager();
	myVertexBuffer = new VertexBuffer(gpuMng);

	// Define position attribute for vertex buffer.
	VertexAttribute posAttrib;
	posAttrib.components = 3;
	posAttrib.offset = 0;
	posAttrib.type = VertexAttribute::TypeFloat;
	posAttrib.target = VertexAttribute::TargetPosition;

	// Define normal attribute for vertex buffer.
	VertexAttribute normalAttrib;
	normalAttrib.components = 3;
	normalAttrib.offset = 12;
	normalAttrib.type = VertexAttribute::TypeFloat;
	normalAttrib.target = VertexAttribute::TargetNormal;

	// Define normal attribute for vertex buffer.
	VertexAttribute colorAttrib;
	colorAttrib.components = 4;
	colorAttrib.type = VertexAttribute::TypeFloat;
	colorAttrib.target = VertexAttribute::TargetPrimaryColor;


	int dataSize = myData->getNumVertices() * 3 * sizeof(float);
	int elementSize = 3 * sizeof(float);
	myVertexBuffer->addAttribute(posAttrib);

	if(myData->hasNormals())
	{
		dataSize += myData->getNumVertices() * 3 * sizeof(float);
		elementSize += 3 * sizeof(float);
		myVertexBuffer->addAttribute(normalAttrib);
	}
	if(myData->hasColors())
	{
		dataSize += myData->getNumVertices() * 4 * sizeof(float);
		elementSize += 4 * sizeof(float);
		if(myData->hasNormals()) colorAttrib.offset = 24;
		else colorAttrib.offset = 12;
		myVertexBuffer->addAttribute(colorAttrib);
	}

	// Copy and organize raw vertex buffer data into temporary buffer
	float* data = new float[dataSize];
	int j = 0;
	for(int i = 0; i < myData->getNumVertices(); i++)
	{
		const Vector3f& pos = myData->getVertexPosition(i);
		data[j++] = pos[0];
		data[j++] = pos[1];
		data[j++] = pos[2];
		if(myData->hasNormals())
		{
			const Vector3f& n = myData->getVertexNormal(i);
			data[j++] = n[0];
			data[j++] = n[1];
			data[j++] = n[2];
		}
		if(myData->hasColors())
		{
			const Color& c = myData->getVertexColor(i);
			data[j++] = c[0];
			data[j++] = c[1];
			data[j++] = c[2];
			data[j++] = c[3];
		}
	}

	// Write data to the vertex buffer.
	myVertexBuffer->initialize(dataSize, elementSize, data);
	delete data;

	// Copy and organize raw index buffer data into temporary buffer
	dataSize = myData->getNumTriangles() * 3;
	elementSize = 3 * sizeof(int);
	unsigned int* indexData = new unsigned int[dataSize];
	j = 0;
	for(int i = 0; i < myData->getNumTriangles(); i++)
	{
		const Triangle& t = myData->getTriangle(i);
		indexData[j++] = t[0];
		indexData[j++] = t[1];
		indexData[j++] = t[2];
	}
	// Write data to index buffer.
	myIndexData = indexData;
}

