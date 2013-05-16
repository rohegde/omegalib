/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __MODEL_LOADER__
#define __MODEL_LOADER__

#include "cyclopsConfig.h"
#include "Skybox.h"
#include "Shapes.h"
#include "Uniforms.h"

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct ModelInfo: public ReferenceType
	{
		ModelInfo(): numFiles(1), size(0.0f), generateNormals(false), normalizeNormals(false), optimize(false), usePowerOfTwoTextures(true), buildKdTree(false), generateTangents(false)
		{}

		ModelInfo(const String name, const String path, float size = 0.0f, int numFiles = 1, bool generateNormals = false, bool normalizeNormals = false, bool generateTangents = false)
		{
			this->name = name;
			this->path = path;
			this->size = size;
			this->numFiles = numFiles;
			this->generateNormals = generateNormals;
			this->generateTangents = generateTangents;
			this->normalizeNormals = normalizeNormals;
		}

		String name;
		String path;
		String description;
		uint numFiles;
		float size;
		bool generateNormals;
		bool generateTangents;
		bool optimize;

		bool usePowerOfTwoTextures;
		bool buildKdTree;
		
		bool normalizeNormals;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class ModelAsset: public ReferenceType
	{
	public:
		ModelAsset(): id(0), numNodes(0) {}
		String description;
		String name;
		Vector< Ref<osg::Node> > nodes;
		//! Number of nodes in this model (used for multimodel assets)
		int numNodes;
		int id;

		Ref<ModelInfo> info;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct ModelLoader: public ReferenceType
	{
	public:
		ModelLoader(const String& name): myName(name) {}

		virtual bool load(ModelAsset* model) = 0;
		virtual bool supportsExtension(const String& ext) { return false; }

		const String& getName() { return myName; }
	
	protected:
		osg::Node* processDefaultOptions(osg::Node* Node, ModelAsset* asset);

	private:
		String myName;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct DefaultModelLoader: public ModelLoader
	{
	public:
		DefaultModelLoader(): ModelLoader("default") {}

		virtual bool load(ModelAsset* model);
		virtual bool supportsExtension(const String& ext) { return true; }
	};
};

#endif
