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
#include "Tile.h"

using namespace omega;
using namespace oengine;
using namespace cybersplash;

OMEGA_DEFINE_TYPE(Tile, OmegaObject)
OMEGA_DEFINE_TYPE(TileObject, SceneObject)
OMEGA_DEFINE_TYPE(ImageTileObject, TileObject)
OMEGA_DEFINE_TYPE(ImageTileRenderable, SceneRenderable)

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageTileObject::ImageTileObject()
{
	myBoundingBox = AlignedBox3(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f);

	myImage[EyeLeft] = NULL;
	myImage[EyeRight] = NULL;
	myImage[EyeCyclops] = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageTileObject::~ImageTileObject()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* ImageTileObject::getBoundingBox()
{
	return &myBoundingBox;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ImageTileObject::hasBoundingBox()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* ImageTileObject::createRenderable()
{
	return new TileRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageTileRenderable::ImageTileRenderable(ImageTileObject* tq): 
	myTexturedQuad(tq), 
	myVertexBuffer(NULL)
{
	myTexture[ImageTileObject::EyeLeft] = NULL;
	myTexture[ImageTileObject::EyeRight] = NULL;
	myTexture[ImageTileObject::EyeCyclops] = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageTileRenderable::~ImageTileRenderable()
{
	dispose();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImageTileRenderable::initialize()
{
	refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImageTileRenderable::dispose()
{
	if(myVertexBuffer != NULL)
	{
		delete myVertexBuffer;
		myVertexBuffer = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImageTileRenderable::refresh()
{
	if(myVertexBuffer != NULL)
	{
		delete myVertexBuffer;
		myVertexBuffer = NULL;
	}

	// Allocate index and vertex buffers.
	GpuManager* gpuMng = getClient()->getGpu();
	myVertexBuffer = new VertexBuffer(gpuMng);

	// Define position attribute for vertex buffer.
	VertexAttribute posAttrib;
	posAttrib.components = 3;
	posAttrib.offset = 0;
	posAttrib.type = VertexAttribute::TypeFloat;
	posAttrib.target = VertexAttribute::TargetPosition;

	// Define texture coords attribute for vertex buffer.
	VertexAttribute texAttrib;
	texAttrib.components = 2;
	texAttrib.offset = 12;
	texAttrib.type = VertexAttribute::TypeFloat;
	texAttrib.target = VertexAttribute::TargetTexture0;

	myVertexBuffer->addAttribute(posAttrib);
	myVertexBuffer->addAttribute(texAttrib);

	// Data format: pos(x, y, z) tex(u, v)
	int elementSize = 20;
	int numElements = 4;
	float data[] = { 
		-0.5f, -0.5f, 0.0f, 0, 0,
		0.5f, -0.5f, 0.0f, 1, 0,
		-0.5f, 0.5f, 0.0f, 0, 1,
		0.5f, 0.5f, 0.0f, 1, 1};

	// Write data to the vertex buffer.
	myVertexBuffer->initialize(elementSize * numElements, elementSize, data);

	// Setup the texture
	ImageData* img = myTexturedQuad->getImage(ImageTileObject::EyeCyclops);
	if(img != NULL)
	{
		myTexture[ImageTileObject::EyeCyclops] = gpuMng->getTexture(img->filename);
		if(myTexture == NULL)
		{
			myTexture[ImageTileObject::EyeCyclops] = gpuMng->createTexture(
				img->filename,
				img->width,
				img->height,
				img->data);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ImageTileRenderable::draw(RenderState* state)
{
	if(state->isFlagSet(RenderPass::RenderOpaque))
	{
		glEnable(GL_TEXTURE_2D);
		myTexture[ImageTileObject::EyeCyclops]->bind(GpuManager::TextureUnit0);
		glColor4ub(255, 255, 255, 255);

		glDisable(GL_LIGHTING);
		pushNodeTransform();
		getRenderer()->drawPrimitives(
			myVertexBuffer,
			NULL,
			4,
			Renderer::DrawTriangleStrip);
		popNodeTransform();

		myTexture[ImageTileObject::EyeCyclops]->unbind();
		glDisable(GL_TEXTURE_2D);
	}
}

