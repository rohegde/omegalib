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
#include "Skybox.h"
#include "Shapes.h"

#include <osg/Texture2D>
#include <osg/Light>
#include <osg/Group>
#include <osg/Switch>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgAnimation/BasicAnimationManager>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

class ModelLoaderThread;
namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;

	class SceneLoader;
	class SceneManager;
	class AnimatedObject;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! PYAPI
	class CY_API Light: public Node
	{
	friend class SceneManager;
	public:
		//! PYAPI Convenience method for creating Light instances
		static Light* create();
	
	public:
		Light(SceneManager* scene);
		virtual ~Light();

		//! PYAPI
		void setColor(const Color& value) { myColor = value; }
		const Color& getColor() { return myColor; }

		//! PYAPI
		void setAmbient(const Color& value) { myAmbient = value; }
		const Color& getAmbient() { return myAmbient; }

		//!PYAPI
		void setEnabled(bool value) { myEnabled = value; }
		//! PYAPI
		bool isEnabled() { return myEnabled; }

		void setAttenuation(const Vector3f value) { myAttenuation = value; }
		void setAttenuation(float constant, float linear, float quadratic) 
		{ 
			myAttenuation[0] = constant; 
			myAttenuation[1] = linear; 
			myAttenuation[2] = quadratic; 
		}
		const Vector3f& getAttenuation() { return myAttenuation; }

		//! PYAPI
		void setSoftShadowWidth(float value) { mySoftShadowWidth = value; }
		//! PYAPI
		float getSoftShadowWidth() { return mySoftShadowWidth; }

		//! PYAPI
		void setSoftShadowJitter(int value) { mySoftShadowJitter = value; }
		//! PYAPI
		float getSoftShadowJitter() { return mySoftShadowJitter; }

	private:
		Ref<SceneManager> mySceneManager;

		Color myColor;
		Color myAmbient;
		bool myEnabled;
		Vector3f myAttenuation;
		float mySoftShadowWidth;
		int mySoftShadowJitter;

		// osg light stuff.
		Ref<osg::Light> myOsgLight;
		Ref<osg::LightSource> myOsgLightSource;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! PYAPI
	struct ModelInfo: public ReferenceType
	{
		//! PYAPI
		ModelInfo(): numFiles(1), size(0.0f), generateNormals(false), normalizeNormals(false), optimize(false), usePowerOfTwoTextures(true), buildKdTree(false)
		{}

		ModelInfo(const String name, const String path, float size = 0.0f, int numFiles = 1, bool generateNormals = false, bool normalizeNormals = false)
		{
			this->name = name;
			this->path = path;
			this->size = size;
			this->numFiles = numFiles;
			this->generateNormals = generateNormals;
			this->normalizeNormals = normalizeNormals;
		}

		//! PYAPI
		String name;
		//! PYAPI
		String path;
		//! PYAPI
		String description;
		//! PYAPI
		uint numFiles;
		//! PYAPI
		float size;
		//! PYAPI
		bool generateNormals;
		bool optimize;

		bool usePowerOfTwoTextures;
		bool buildKdTree;
		
		bool normalizeNormals;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! PYAPI
	struct ShadowSettings
	{
		//! PYAPI
		bool shadowsEnabled;
		//! PYAPI
		float shadowResolutionRatio;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class ModelAsset: public ReferenceType
	{
	public:
		ModelAsset(): id(0), numNodes(0) {}
		String description;
		String filename;
		Vector< Ref<osg::Node> > nodes;
		//! Number of nodes in this model (used for multimodel assets)
		int numNodes;
		int id;

		Ref<ModelInfo> info;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class ProgramAsset: public ReferenceType
	{
	public:
		ProgramAsset():
			program(NULL), vertexShader(NULL), fragmentShader(NULL)
		{}
	
		String name;
		String vertexShaderName;
		String fragmentShaderName;

		Ref<osg::Program> program;
		Ref<osg::Shader> vertexShader;
		Ref<osg::Shader> fragmentShader;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Interface for scene manager listeners
	class SceneManagerListener
	{
	public: 
		//! Called when an object is added to the scene manager. By reimplementing this method, 
		//! users can modify the object before insertion in the scenegraph or insert intermediate osg
		//! nodes.
		virtual osg::Node* onObjectAdded(Entity* obj)
		{
			return obj->getOsgNode();
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! PYAPI
	class CY_API SceneManager: public EngineModule
	{
	friend class Entity;
	friend class Light;
	public:
		typedef AsyncTask< pair< Ref<ModelInfo>, bool > > LoadModelAsyncTask;
		typedef Dictionary<String, String> ShaderMacroDictionary;
		enum AssetType { ModelAssetType };

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

		//! Sets the scene manager listener
		void setListener(SceneManagerListener* listener);
		//! Gets the scene manager listener
		SceneManagerListener* getListener();

		virtual void initialize();
		virtual void dispose();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const Event& evt);

		//! #PYAPI sets the background color
		void setBackgroundColor(const Color& color);

		//! Model Management
		//@{
		bool loadModel(ModelInfo* info);
		LoadModelAsyncTask* loadModelAsync(ModelInfo* info);
		void loadModelAsync(ModelInfo* info, const String& callback);
		ModelAsset* getModel(const String& name);
		const List< Ref<ModelAsset> >& getModels();
		//@}

		//! Wand Management
		//@{
		void displayWand(uint wandId, uint trackableId);
		void setWandEffect(uint wandId, const String& effect);
		void setWandSize(float width, float length);
		//@}

		//! Scene creation methods
		//@{
		void load(SceneLoader* loader);
		//! #PYAPI Utility method: loads a scene file using the standard cyclops scene loader.
		void loadScene(const String& file);
		void setSkyBox(Skybox* skyBox);
		void unload();
		//@}

		//! Light management methods
		//@{
		Light* getMainLight() { return myMainLight; }
		void setMainLight(Light* light) { myMainLight = light; }
		const ShadowSettings& getCurrentShadowSettings();
		void resetShadowSettings(const ShadowSettings& settings);
		int getNumActiveLights();
		//@}

		//! Object management
		//@{
		Entity* getEntityByName(const String& name);
		Entity* getEntityByIndex(int index);
		int getNumEntities();
		//List<Entity*>::Range getObjects();
		//@}

		osg::Node* getOsgRoot() { return myScene; }
		osg::Texture2D* getTexture(const String& name);
		ProgramAsset* getProgram(const String& name, const String& vertexShaderName, const String& fragmentShaderName);
		void initShading();

		void setShaderMacroToString(const String& macroName, const String& macroString);
		void setShaderMacroToFile(const String& macroName, const String& path);

	private:
		SceneManager();
		virtual ~SceneManager();

		void addLight(Light* l);
		void removeLight(Light* l);
		void addEntity(Entity* obj);
		void removeEntity(Entity* obj);
		void updateLights();
		void loadConfiguration();
		void loadShader(osg::Shader* shader, const String& name);
		void recompileShaders(ProgramAsset* program, const String& variationName);
		void recompileShaders();

	private:
		static SceneManager* mysInstance;
		Ref<OsgModule> myOsg;

		// Scene manager listener
		SceneManagerListener* myListener;

		// The scene root. This may be linked directly to myRoot or have some intermediate nodes inbetween
		// (i.e. for shadow map management)
		Ref<osg::Group> myScene;

		// Model data (stored as dictionary and list for convenience)
		Dictionary<String, Ref<ModelAsset> > myModelDictionary;
		List< Ref<ModelAsset> > myModelList;
		ModelLoaderThread* myModelLoaderThread;

		Dictionary<String, Ref<osg::Texture2D> > myTextures;
		Dictionary<String, Ref<ProgramAsset> > myPrograms;
		Dictionary<String, Ref<osg::Shader> > myShaders;

		ShaderMacroDictionary myShaderMacros;

		Vector< Entity* > myObjectVector;
		Dictionary<Entity*, osg::Node*> myEntityNodeMap;
		
		Ref<Skybox> mySkyBox;

		// Lights and shadows
		List< Ref<Light> > myLights;
		Ref<Light> myMainLight;
		
		Ref<osgShadow::ShadowedScene> myShadowedScene;
		Ref<osgShadow::SoftShadowMap> mySoftShadowMap;
		ShadowSettings myShadowSettings;
		int myNumActiveLights;
		String myShaderVariationName;

		// Wand
		Ref<omegaToolkit::TrackedObject> myWandTracker;
		Ref<CylinderShape> myWandEntity;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline  Entity* SceneManager::getEntityByIndex(int index)
	{ return myObjectVector[index]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int SceneManager::getNumEntities()
	{ return myObjectVector.size(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneManager::setListener(SceneManagerListener* listener)
	{
		myListener = listener;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneManagerListener* SceneManager::getListener()
	{
		return myListener;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//inline List<Entity*>::Range SceneManager::getObjects()
	//{ return List<Entity*>::Range(myObjectList.begin(), myObjectList.end()); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int SceneManager::getNumActiveLights()
	{ return myNumActiveLights; }
};

#endif
