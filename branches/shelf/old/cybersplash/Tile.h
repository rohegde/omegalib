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
#ifndef __TILE_H__
#define __TILE_H__

#include <omega.h>
#include <oengine.h>

namespace cybersplash {
	using namespace oengine;

	class TileGroup;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class TileObject: public SceneObject
	{
	OMEGA_DECLARE_TYPE(TileObject)
	public:
		TileObject();
		~TileObject();

		const AlignedBox3* getBoundingBox();
		bool hasBoundingBox();

	private:
		AlignedBox3 myBoundingBox;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class ImageTileObject: public TileObject
	{
	OMEGA_DECLARE_TYPE(ImageTileObject)
	public:
		enum Eye {EyeLeft, EyeRight, EyeCyclops, EyeMax};
	public:
		ImageTileObject();
		~ImageTileObject();
		virtual Renderable* createRenderable();

		const AlignedBox3* getBoundingBox();
		bool hasBoundingBox();

		ImageData* getImage(Eye eye);
		void setImage(ImageData* img);
		void setImage(ImageData* left, ImageData* right);

		bool isStereo() { return myStereo; }

	private:
		bool myStereo;

		AlignedBox3 myBoundingBox;
		ImageData* myImage[EyeMax];
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class ImageTileRenderable: public SceneRenderable
	{
	OMEGA_DECLARE_TYPE(TileRenderable)
	public:
		ImageTileRenderable(ImageTileObject* tq);
		~ImageTileRenderable();
		virtual void initialize();
		virtual void dispose();
		void draw(RenderState* state);
		void refresh();

	private:
		ImageTileObject* myTexturedQuad;
		VertexBuffer* myVertexBuffer;
		Texture* myTexture[ImageTileObject::EyeMax];
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class Tile: public OmegaObject
	{
	OMEGA_DECLARE_TYPE(Tile)
	public:
		Tile(TileObject* obj);

		TileObject* getObject() { myObject; }
		SceneNode* getNode() { return myNode; }

	private:
		TileObject* myObject;
		SceneNode* myNode;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ImageData* ImageTileObject::getImage(Eye eye) 
	{ return myImage[eye]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ImageTileObject::setImage(ImageData* value) 
	{ 
		myImage[EyeCyclops] = value; 
		myStereo = false; 
		refresh(); 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ImageTileObject::setImage(ImageData* left, ImageData* right) 
	{ 
		myImage[EyeLeft] = left; 
		myImage[EyeRight] = right; 
		myStereo = false; 
		refresh(); 
	}

}; // namespace oengine

#endif