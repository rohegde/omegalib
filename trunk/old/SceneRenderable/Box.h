/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __BOX_H__
#define __BOX_H__

#include "RenderableSceneObject.h"
#include "SceneRenderable.h"

namespace omegaToolkit {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OTK_API Box: public RenderableSceneObject
	{
	public:
		Box();
		virtual Renderable* createRenderable();
		virtual const AlignedBox3* getBoundingBox() { return &myBBox; }
		virtual bool hasBoundingBox() { return true; }

	private:
		AlignedBox3 myBBox;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class BoxRenderable: public SceneRenderable
	{
	public:
		BoxRenderable(Box* box);
		virtual ~BoxRenderable();
		void initialize();
		void draw(const DrawContext& context);

	private:
		Box* myBox;

		Vector3f myNormals[6];
		Vector4i myFaces[6]; 
		Vector3f myVertices[8];
		Color myFaceColors[6];
	};
}; // namespace omegaToolkit

#endif