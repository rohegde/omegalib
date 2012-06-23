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
#include "SkyBox.h"

#include <osg/Texture2D>
#include <osg/Light>
#include <osg/Group>
#include <osg/Switch>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgAnimation/BasicAnimationManager>

#include "cyclops/DrawableObject.h"

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;

	class SceneLoader;
	class SceneManager;
	class Entity;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class Light: public Node
	{
	friend class SceneManager;
	public:
		Light(SceneManager* scene);
		virtual ~Light();

		void setColor(const Color& value) { myColor = value; }
		const Color& getColor() { return myColor; }

		void setAmbient(const Color& value) { myAmbient = value; }
		const Color& getAmbient() { return myAmbient; }

		void setEnabled(bool value) { myEnabled = value; }
		bool isEnabled() { return myEnabled; }

		void setAttenuation(const Vector3f value) { myAttenuation = value; }
		void setAttenuation(float constant, float linear, float quadratic) 
		{ 
			myAttenuation[0] = constant; 
			myAttenuation[1] = linear; 
			myAttenuation[2] = quadratic; 
		}
		const Vector3f& getAttenuation() { return myAttenuation; }

		void setSoftShadowWidth(float value) { mySoftShadowWidth = value; }
		float getSoftShadowWidth() { return mySoftShadowWidth; }

		void setSoftShadowJitter(int value) { mySoftShadowJitter = value; }
		float getSoftShadowJitter() { return mySoftShadowJitter; }

	private:
		SceneManager* mySceneManager;

		Color myColor;
		Color myAmbient;
		bool myEnabled;
		Vector3f myAttenuation;
		float mySoftShadowWidth;
		int mySoftShadowJitter;

		// osg light stuff.
		osg::Light* myOsgLight;
		osg::LightSource* myOsgLightSource;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct ModelInfo
	{
		ModelInfo(): numFiles(1), size(1) 
		{}

		ModelInfo(const String name, const String path, float size = 1.0f, int numFiles = 1)
		{
			this->name = name;
			this->path = path;
			this->size = size;
			this->numFiles = numFiles;
		}

		String name;
		String path;
		String description;
		uint numFiles;
		float size;
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

		ModelInfo info;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API SceneManager: public ServerModule
	{
	friend class DrawableObject;
	friend class Light;
	public:
		typedef Dictionary<String, String> ShaderMacroDictionary;
		enum AssetType { ModelAssetType };

		enum ShadowMode { ShadowsDisabled, ShadowsSoft };

		static const int MaxLights = 16;

	public:
		static const int ReceivesShadowTraversalMask = 0x1;
		static const int CastsShadowTraversalMask = 0x2;

	public:
		//! Creates and initializes the scene manager singleton.
		//! If called multiple times, subsequent calls will do nothing.
		static SceneManager* createAndInitialize();
		//! Returns an instance of the SceneManager singleton instance If no
		// Scene manager exists before this call, createAndInitialize will be called internally.
		static SceneManager* instance();

		ServerEngine* getEngine() { return myEngine; }

		virtual void initialize();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt);

		//! Model Management
		//@{
		bool loadModel(ModelInfo info);
		ModelAsset* getModel(const String& name);
		const List<ModelAsset*>& getModels();
		//@}


		//! Scene creation methods
		//@{
		void load(SceneLoader* loader);
		//! Utility method: loads a scene file using the standard cyclops scene loader.
		void load(const String& file);
		void createSkyBox(const String& cubeMapDir, const String& cubeMapExt);
		//@}

		//! Light management methods
		//@{
		Light* getMainLight() { return myMainLight; }
		void setMainLight(Light* light) { myMainLight = light; }
		//! Shadow map control
		int getShadowMapQuality() { return myShadowMapQuality; }
		//@}

		//! Object management
		//@{
		template<typename T> T* getObject(const String& name);
		int getNumObjects();
		List<DrawableObject*>::Range getObjects();
		//@}

		osg::Texture2D* getTexture(const String& name);
		osg::Program* getProgram(const String& name, const String& vertexShaderName, const String& fragmentShaderName);
		void initShading();

		void setShaderMacroToString(const String& macroName, const String& macroString);
		void setShaderMacroToFile(const String& macroName, const String& path);

	private:
		SceneManager();

		void addLight(Light* l);
		void removeLight(Light* l);
		void addObject(DrawableObject* obj);
		void updateLights();
		void loadConfiguration();
		void loadShader(osg::Shader* shader, const String& name);

	private:
		static SceneManager* mysInstance;

		// Initialization options
		ShadowMode myShadowMode;
		String mySceneFilename;

		ServerEngine* myEngine;

		OsgModule* myOsg;

		osg::Group* mySceneRoot;

		// Model data (stored as dictionary and list for convenience)
		Dictionary<String, ModelAsset*> myModelDictionary;
		List<ModelAsset*> myModelList;

		Dictionary<String, osg::Texture2D*> myTextures;
		Dictionary<String, osg::Program*> myPrograms;

		ShaderMacroDictionary myShaderMacros;

		Dictionary<String, DrawableObject*> myObjectDictionary;
		List<DrawableObject*> myObjectList;
		
		SkyBox* mySkyBox;

		// Lights and shadows
		List<Light*> myLights;
		Light* myMainLight;
		osgShadow::ShadowedScene* myShadowedScene;
		osgShadow::SoftShadowMap* mySoftShadowMap;
		int myShadowMapQuality;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> 
	inline  T* SceneManager::getObject(const String& name)
	{ return dynamic_cast<T*>(myObjectDictionary[name]); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int SceneManager::getNumObjects()
	{ return myObjectList.size(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline List<DrawableObject*>::Range SceneManager::getObjects()
	{ return List<DrawableObject*>::Range(myObjectList.begin(), myObjectList.end()); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Light::Light(SceneManager* scene):
		mySceneManager(scene),
		myColor(Color::White),
		myAmbient(Color::Gray),
		myAttenuation(Vector3f::Zero()),
		myEnabled(false),
		mySoftShadowWidth(0.005f),
		mySoftShadowJitter(32),
		myOsgLight(NULL), myOsgLightSource(NULL)
	{
		mySceneManager->addLight(this);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Light::~Light()
	{
		mySceneManager->removeLight(this);
	}
};

#endif
