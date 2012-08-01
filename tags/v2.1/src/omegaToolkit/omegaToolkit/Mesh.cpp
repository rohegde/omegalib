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
#include "omegaToolkit/Mesh.h"
#include "omega/DrawInterface.h"
#include "omega/Renderer.h"

using namespace omega;
using namespace omegaToolkit;

///////////////////////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(): 
	myData(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Mesh::~Mesh()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MeshData* Mesh::getData() 
{
	return myData; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::setData(MeshData* value) 
{
	myData = value; 
	refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* Mesh::getBoundingBox()
{
	return &myData->getBoundingBox();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Mesh::hasBoundingBox()
{
	if(myData != NULL) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Mesh::createRenderable()
{
	return new MeshRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MeshRenderable::MeshRenderable(Mesh* mesh): 
	myMesh(mesh), 
	myVertexBuffer(NULL), 
	myIndexData(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MeshRenderable::~MeshRenderable()
{
	dispose();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshRenderable::initialize()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshRenderable::dispose()
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshRenderable::refresh()
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
	GpuManager* gpuMng = getClient()->getGpuContext()->getGpu();
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

	// Define color attribute for vertex buffer.
	VertexAttribute colorAttrib;
	colorAttrib.components = 4;
	colorAttrib.type = VertexAttribute::TypeFloat;
	colorAttrib.target = VertexAttribute::TargetPrimaryColor;

	MeshData* meshData = myMesh->getData();
	if(meshData == NULL)
	{
		owarn("MeshRenderable::refresh: mesh data = NULL.");
		return;
	}

	int numVertices = meshData->getNumVertices();
	int dataSize = numVertices * 3 * sizeof(float);
	int elementSize = 3 * sizeof(float);
	myVertexBuffer->addAttribute(posAttrib);

	if(meshData->hasNormals())
	{
		dataSize += numVertices * 3 * sizeof(float);
		elementSize += 3 * sizeof(float);
		myVertexBuffer->addAttribute(normalAttrib);
	}
	if(meshData->hasColors())
	{
		dataSize += numVertices * 4 * sizeof(float);
		elementSize += 4 * sizeof(float);
		if(meshData->hasNormals()) colorAttrib.offset = 24;
		else colorAttrib.offset = 12;
		myVertexBuffer->addAttribute(colorAttrib);
	}

	// Copy and organize raw vertex buffer data into temporary buffer
	float* data = new float[dataSize];
	int j = 0;
	for(int i = 0; i < numVertices; i++)
	{
		const Vector3f& pos = meshData->getVertexPosition(i);
		data[j++] = pos[0];
		data[j++] = pos[1];
		data[j++] = pos[2];
		if(meshData->hasNormals())
		{
			const Vector3f& n = meshData->getVertexNormal(i);
			data[j++] = n[0];
			data[j++] = n[1];
			data[j++] = n[2];
		}
		if(meshData->hasColors())
		{
			const Color& c = meshData->getVertexColor(i);
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
	dataSize = meshData->getNumTriangles() * 3;
	elementSize = 3 * sizeof(int);
	unsigned int* indexData = new unsigned int[dataSize];
	j = 0;
	for(int i = 0; i < meshData->getNumTriangles(); i++)
	{
		const Triangle& t = meshData->getTriangle(i);
		indexData[j++] = t[0];
		indexData[j++] = t[1];
		indexData[j++] = t[2];
	}
	// Write data to index buffer.
	myIndexData = indexData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshRenderable::draw(RenderState* state)
{
	if(state->isFlagSet(RenderPass::RenderOpaque))
	{
		pushNodeTransform();
		if(myMesh->getData() != NULL)
		{
			getRenderer()->drawPrimitives(
				myVertexBuffer,
				myIndexData,
				myMesh->getData()->getNumTriangles() * 3,
				DrawInterface::DrawTriangles);
		}
		popNodeTransform();
	}
}

