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
#ifndef __SCENE_MANAGER__
#define __SCENE_MANAGER__

#include <osg/Texture2D>
#include <osg/Light>
#include <osg/Group>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>
#include <oosg.h>

namespace cyclops {
	using namespace omega;
	using namespace oengine;
	using namespace oosg;

	class SceneLoader;

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct ModelAsset
	{
		String filename;
		osg::Node* node;
		int id;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct EntityAsset: public ModelAsset
	{
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class SceneManager
	{
	public:
		enum AssetType { ModelAssetType, EntityAssetType };

	public:
		static const int ReceivesShadowTraversalMask = 0x1;
		static const int CastsShadowTraversalMask = 0x2;

		static bool findResource(const String& name, String& outPath);

	public:
		static SceneManager* getInstance();
		SceneManager();

		void initialize(EngineServer* engine);
		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);

		void load(SceneLoader* loader);

		void addAsset(ModelAsset* asset, AssetType type);
		ModelAsset* getModelAsset(int fileIndex);
		EntityAsset* getEntityAsset(int fileIndex);

		osg::Texture2D* getTexture(const String& name);
		osg::Program* getProgram(const String& name, const String& vertexShaderName, const String& fragmentShaderName);
		osg::StateSet* loadMaterial(const String& filename);
		void initShading();

	private:
		void loadShader(osg::Shader* shader, const String& name);
		osg::StateSet* createMaterial(TiXmlElement* xdata, const String& type);

	private:
		static SceneManager* mysInstance;

		EngineServer* myEngine;
		OsgModule* myOsg;
		osg::Group* mySceneRoot;

		Vector<ModelAsset*> myStaticObjectFiles;
		Vector<EntityAsset*> myEntityFiles;

		Dictionary<String, osg::Texture2D*> myTextures;
		Dictionary<String, osg::Program*> myPrograms;
		Dictionary<String, osg::StateSet*> myMaterials;

		osg::Light* myLight2;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneManager* SceneManager::getInstance() 
	{ return mysInstance; }
};

#endif
