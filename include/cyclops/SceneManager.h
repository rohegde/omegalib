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
#ifndef __CY_SCENE_MANAGER__
#define __CY_SCENE_MANAGER__

#include "cyclopsConfig.h"

#include <osg/Texture2D>
#include <osg/Light>
#include <osg/Group>
#include <osg/Switch>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

#include "omega/PortholeTabletService.h"


namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;

	class SceneLoader;
	class SceneManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct Light
	{
		Light():
			position(Vector3f::Zero()),
			color(Color::White),
			ambient(Color::Gray),
			constAttenuation(0), linearAttenuation(0), quadAttenuation(0),
			enabled(false),
			osgLight(NULL), osgLightSource(NULL)
			{}

		Vector3f position;
		Color color;
		Color ambient;
		bool enabled;
		float constAttenuation;
		float linearAttenuation;
		float quadAttenuation;

		// osg light stuff.
		osg::Light* osgLight;
		osg::LightSource* osgLightSource;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct ModelAsset
	{
		ModelAsset(): id(0), numNodes(0) {}
		String description;
		String filename;
		Vector<osg::Node*> nodes;
		//! Number of nodes in this model (used for multimodel assets)
		int numNodes;
		int id;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API Entity
	{
	public:
		Entity(SceneManager* mng, ModelAsset* asset, int id);

		osg::Node* getOsgNode() { return myOsgNode; }
		SceneNode* getSceneNode() { return mySceneNode; }
		ModelAsset* getAsset() { return myAsset; }
		int getId() { return myId; }

		int getNumModels() { return myAsset->numNodes; }
		void setCurrentModelIndex(int index);
		int getCurrentModelIndex();

		void setTag(const String& value) { myTag = value; }
		const String& getTag() { return myTag; } 

	private:
		SceneManager* mySceneManager;

		osg::Node* myOsgNode;
		osg::Switch* myOsgSwitch;
		int myCurrentModelIndex;

		SceneNode* mySceneNode;
		ModelAsset* myAsset;
		int myId;
		String myTag;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API SceneManager: public ServerModule
	{
	public:
		typedef Dictionary<String, String> ShaderMacroDictionary;
		enum AssetType { ModelAssetType };

		enum ShadowMode { ShadowsDisabled, ShadowsSoft };

		static const int MaxLights = 16;

	public:
		static const int ReceivesShadowTraversalMask = 0x1;
		static const int CastsShadowTraversalMask = 0x2;

		static bool findResource(const String& name, String& outPath);

	public:
		static SceneManager* instance();
		SceneManager();

		ServerEngine* getEngine() { return myEngine; }

		virtual void initialize();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt);

		void load(SceneLoader* loader);
		//! Utility method: loads a scene file using the standard cyclops scene loader.
		void load(const String& file);

		//! Scene creation methods
		//@{
		void addNode(osg::Node* node);
		void addNode(osg::Node* node, const Vector3f& position, const Vector3f& rotation = Vector3f::Zero(), const Vector3f& scale = Vector3f::Ones());
		void addStaticObject(int assetId, const Vector3f& position, const Vector3f& rotation = Vector3f::Zero(), const Vector3f& scale = Vector3f::Ones());
		void addEntity(int assetId, int entityId, const String& tag, const Vector3f& position, const Vector3f& rotation = Vector3f::Zero(), const Vector3f& scale = Vector3f::Ones());
		//@}

		//! Light management methods
		//@{
		Light* getLight(int id);
		Light* getMainLight() { return myMainLight; }
		void setMainLight(Light* light) { myMainLight = light; }
		//@}

		void addAsset(ModelAsset* asset, AssetType type);
		ModelAsset* getModelAsset(int fileIndex);
		Entity* findEntity(int id);
		int getNumEntities() { return myEntities.size(); }
		List<Entity*>::Range getEntities();

		osg::Texture2D* getTexture(const String& name);
		osg::Program* getProgram(const String& name, const String& vertexShaderName, const String& fragmentShaderName);
		osg::StateSet* loadMaterialPass(const String& filename);
		void initShading();

		void setShaderMacroToString(const String& macroName, const String& macroString);
		void setShaderMacroToFile(const String& macroName, const String& path);

	private:
		void updateLights();
		void loadConfiguration();

	private:
		void loadShader(osg::Shader* shader, const String& name);
		osg::StateSet* createMaterialPass(TiXmlElement* xdata, const String& type);

	private:
		static SceneManager* mysInstance;

		// Initialization options
		ShadowMode myShadowMode;
		bool myEditorEnabled;
		String mySceneFilename;

		ServerEngine* myEngine;

		OsgModule* myOsg;
		omegaToolkit::SceneEditorModule* myEditor;
		PortholeTabletService* myTabletManager;

		osg::Group* mySceneRoot;

		Vector<ModelAsset*> myModelFiles;

		Dictionary<String, osg::Texture2D*> myTextures;
		Dictionary<String, osg::Program*> myPrograms;
		Dictionary<String, osg::StateSet*> myMaterialPasses;

		ShaderMacroDictionary myShaderMacros;

		List<Entity*> myEntities;

		// Lights and shadows
		Light myLights[MaxLights];
		Light* myMainLight;
		osgShadow::ShadowedScene* myShadowedScene;
		osgShadow::SoftShadowMap* mySoftShadowMap;
		//bool frontView;
		//float localZoom;
		//float remoteZoom;
		//int myRotate;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneManager* SceneManager::instance() 
	{ return mysInstance; }
};

#endif
