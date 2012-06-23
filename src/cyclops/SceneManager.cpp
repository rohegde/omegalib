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
#include <osgUtil/Optimizer>
#include <osgDB/Archive>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osgAnimation/Animation>

#include "cyclops/Entity.h"
#include "cyclops/SceneManager.h"
#include "cyclops/SceneLoader.h"

using namespace cyclops;

void cyclopsPythonApiInit();

SceneManager* SceneManager::mysInstance = NULL;

#ifdef OMEGA_OS_LINUX
	// On linux we need to define all static variables, even if they have been assigned in the header file.
	const int SceneManager::MaxLights;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct AnimationManagerFinder : public osg::NodeVisitor 
{ 
    osgAnimation::BasicAnimationManager* am; 
    
    AnimationManagerFinder(): am(NULL) {setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN); } 
    
    void apply(osg::Node& node) { 
    
        if (am != NULL) 
            return; 
    
        if (node.getUpdateCallback()) {        
            am = dynamic_cast<osgAnimation::BasicAnimationManager*>(node.getUpdateCallback()); 
            return; 
        } 
        
        traverse(node); 
    } 
}; 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager* SceneManager::instance() 
{ 
	if(mysInstance == NULL)
	{
		createAndInitialize();
	}
	return mysInstance; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager* SceneManager::createAndInitialize()
{
	if(mysInstance == NULL)
	{
		mysInstance = new SceneManager();
		ModuleServices::addModule(mysInstance);
		mysInstance->doInitialize(ServerEngine::instance());
	}
	return mysInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager():
	myOsg(NULL),
	myShadowedScene(NULL),
	mySoftShadowMap(NULL),
	mySkyBox(NULL),
	myShadowMapQuality(3)
{
#ifdef OMEGA_USE_PYTHON
	cyclopsPythonApiInit();
#endif

	myOsg = new OsgModule();
	ModuleServices::addModule(myOsg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::loadConfiguration()
{
	Config* cfg = SystemManager::instance()->getAppConfig();
	Setting& s = cfg->lookup("config");

	String shadowMode = Config::getStringValue("shadowMode", s, "noshadows");
	StringUtils::toLowerCase(shadowMode);
	if(shadowMode == "noshadows") myShadowMode = ShadowsDisabled;
	else if(shadowMode == "softshadows") myShadowMode = ShadowsSoft;

	myShadowMapQuality = Config::getIntValue("shadowQuality", s, myShadowMapQuality);

	mySceneFilename = Config::getStringValue("scene", s, "");

	omsg("SceneManager configuration loaded");
	ofmsg("::    Shadow mode: %1%", %shadowMode);
	ofmsg(":: Shadow quality: %1%", %myShadowMapQuality);
	ofmsg("::  Default scene: %1%", %mySceneFilename);

	mySceneRoot = new osg::Group();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initialize()
{
	myMainLight = NULL;

	myEngine = getServer();

	// Make sure the osg module is initialized.
	if(!myOsg->isInitialized()) myOsg->initialize();

	loadConfiguration();

	// If a default scene file has been specified, load it.
	if(mySceneFilename != "")
	{
		load(mySceneFilename);
	}
	else
	{
		// No scene file specified: just initialize an empty scene.
		initShading();
		myOsg->setRootNode(mySceneRoot);
	}

	if(myShadowMode == ShadowsSoft)
	{
		setShaderMacroToFile("use setupShadow", "cyclops/common/softShadows/setupShadow.vert");
		setShaderMacroToFile("use computeShadow", "cyclops/common/softShadows/computeShadow.frag");
		omsg("Soft shadow shaders enabled");
	}
	else
	{
		setShaderMacroToFile("use setupShadow", "cyclops/common/noShadows/setupShadow.vert");
		setShaderMacroToFile("use computeShadow", "cyclops/common/noShadows/computeShadow.frag");
		omsg("Soft shadow shaders disabled");
	}

	if(mySkyBox != NULL)
	{
		setShaderMacroToFile("use setupEnvMap", "cyclops/common/cubeEnvMap/setupEnvMap.vert");
		setShaderMacroToFile("use computeEnvMap", "cyclops/common/cubeEnvMap/computeEnvMap.frag");
		omsg("Environment cube map shaders enabled");
		mySkyBox->initialize(mySceneRoot->getOrCreateStateSet());
		mySceneRoot->addChild(mySkyBox->getNode());
	}
	else
	{
		setShaderMacroToFile("use setupEnvMap", "cyclops/common/noEnvMap/setupEnvMap.vert");
		setShaderMacroToFile("use computeEnvMap", "cyclops/common/noEnvMap/computeEnvMap.frag");
		omsg("Environment cube map shaders disabled");
	}

	setShaderMacroToFile("use setupStandardShading", "cyclops/common/standardShading/setupStandardShading.vert");
	setShaderMacroToFile("use computeStandardShading", "cyclops/common/standardShading/computeStandardShading.frag");

	//frontView = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addObject(DrawableObject* obj)
{
	myObjectList.push_back(obj);
	myObjectDictionary[obj->getName()] = obj;
	mySceneRoot->addChild(obj->getOsgNode());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::update(const UpdateContext& context) 
{
	updateLights();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addLight(Light* l)
{
	myLights.push_back(l);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::removeLight(Light* l)
{
	myLights.remove(l);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::updateLights()
{
	int i = 0;
	foreach(Light* l, myLights)
	{
		if(l->isEnabled())
		{
			if(l->myOsgLight == NULL)
			{
				l->myOsgLight = new osg::Light();
				l->myOsgLightSource = new osg::LightSource();
				mySceneRoot->addChild(l->myOsgLightSource);
			}

			osg::Light* ol = l->myOsgLight;
			osg::LightSource* ols = l->myOsgLightSource;

			const Vector3f pos = l->getPosition();
			const Vector3f& att = l->getAttenuation();

			ol->setLightNum(i++);
			ol->setPosition(osg::Vec4(pos[0], pos[1], pos[2], 1.0));
			ol->setAmbient(COLOR_TO_OSG(l->getAmbient()));
			ol->setDiffuse(COLOR_TO_OSG(l->getColor()));
			ol->setSpecular(COLOR_TO_OSG(l->getColor()));
			ol->setConstantAttenuation(att[0]);
			ol->setLinearAttenuation(att[1]);
			ol->setQuadraticAttenuation(att[2]);

			ols->setLight(ol);

			osg::StateSet* sState = mySceneRoot->getOrCreateStateSet();
			ols->setStateSetModes(*sState,osg::StateAttribute::ON);
		}
		else
		{
			if(l->myOsgLightSource != NULL)
			{
				l->myOsgLightSource->setLocalStateSetModes(osg::StateAttribute::OFF); 
			}
		}
	}

	if(myMainLight != NULL)
	{
		if(mySoftShadowMap != NULL)
		{
			mySoftShadowMap->setLight(myMainLight->myOsgLight);
			mySoftShadowMap->setSoftnessWidth(myMainLight->getSoftShadowWidth());
			mySoftShadowMap->setJitteringScale(myMainLight->getSoftShadowJitter());
		}

		// Set ambient light uniform.
		osg::Uniform* unifAmbient = mySceneRoot->getOrCreateStateSet()->getUniform("unif_Ambient");
		if(unifAmbient == NULL)
		{
			unifAmbient = new osg::Uniform("unif_Ambient", COLOR_TO_OSG(myMainLight->getAmbient()));
			mySceneRoot->getStateSet()->addUniform(unifAmbient, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}
		else
		{
			unifAmbient->set(COLOR_TO_OSG(myMainLight->getAmbient()));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::handleEvent(const Event& evt) 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::load(SceneLoader* loader)
{
	loader->startLoading(this);
	while(!loader->isLoadingComplete()) loader->loadStep();
	if (mySceneRoot != NULL) 
	{
		osg::setNotifyLevel(osg::INFO);
		omsg("Optimizing scene graph...");
		// Optimize scenegraph
		osgUtil::Optimizer optOSGFile;
		//optOSGFile.optimize(node, osgUtil::Optimizer::ALL_OPTIMIZATIONS);
		osg::setNotifyLevel(osg::WARN);

		initShading();

		myOsg->setRootNode(mySceneRoot);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::load(const String& relativePath)
{
	String fullPath;
	//If able to get full path
	if(DataManager::findFile( relativePath , fullPath))
	{
		//Stores the XML file to be parsed
		TiXmlDocument doc(fullPath.c_str());

		//Loads the XML file
		if(doc.LoadFile())
		{
			ofmsg("Loading scene: %1%...", %relativePath);

			//Instantiate a sceneLoader to load the entites in the XML file
			SceneLoader* sl = new SceneLoader(doc, fullPath);

			//SceneManager::instance()
			//	Gets the sceneManager if you do not have a pointer to the singleton sceneManager
			//Load the scene into the SceneManager via the SceneLoader
			load(sl);
		}
		else
		{
			//Error loading the XML
			ofwarn("sceneLoad Xml error at %1%:%2%.%3%: %4%", %relativePath %doc.ErrorRow() %doc.ErrorCol() %doc.ErrorDesc());
		}
	}
	else
	{
		//Error loacation the file
		ofwarn("!File not found: %1%", %relativePath);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Texture2D* SceneManager::getTexture(const String& name)
{
	// If texture has been loaded already return it.
	if(myTextures.find(name) != myTextures.end())
	{
		return myTextures[name];
	}

	// Split the path and get the file name.
	String path;
	//String filename;
	//String extension;
	//StringUtils::splitFullFilename(name, filename, extension, path);

	if(DataManager::findFile(name, path))
	{
		//ofmsg("Loading texture file %1%", %filename);

        osg::ref_ptr< osg::Image > image;

		image = osgDB::readRefImageFile(path);

        if ( image.valid() )
        {
            osg::Texture2D* texture = new osg::Texture2D( image.get() );
            osg::Texture::WrapMode textureWrapMode;
            textureWrapMode = osg::Texture::REPEAT;

            texture->setWrap(osg::Texture2D::WRAP_R, textureWrapMode);
            texture->setWrap(osg::Texture2D::WRAP_S, textureWrapMode);
            texture->setWrap(osg::Texture2D::WRAP_T, textureWrapMode);

			myTextures[name] = texture;
			return texture;
        }
		else
		{
			ofwarn("Image not valid: %1%", %path);
		}
	}
	else
	{
		ofwarn("Could not find texture file %1%", %name);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::setShaderMacroToString(const String& macroName, const String& macroString)
{
	myShaderMacros[macroName] = macroString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::setShaderMacroToFile(const String& macroName, const String& name)
{
	String path;
	if(DataManager::findFile(name, path))
	{
		std::ifstream t(path.c_str());
		std::stringstream buffer;
		buffer << t.rdbuf();
		setShaderMacroToString(macroName, buffer.str());
	}
	else
	{
		ofwarn("SceneManager::setShaderMacroToFile: could not find file %1%", %name);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::loadShader(osg::Shader* shader, const String& name)
{
	String path;
	if(DataManager::findFile(name, path))
	{
		std::ifstream t(path.c_str());
		std::stringstream buffer;
		buffer << t.rdbuf();

		// Replace shader macros.
		String shaderSrc = buffer.str();
		foreach(ShaderMacroDictionary::Item macro, myShaderMacros)
		{
			String macroName = ostr("@%1%", %macro.getKey());
			shaderSrc = StringUtils::replaceAll(shaderSrc, macroName, macro.getValue());
		}

		//ofmsg("Loading shader file %1%", %name);
		shader->setShaderSource(shaderSrc);
	}
	else
	{
		ofwarn("Could not find shader file %1%", %name);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Program* SceneManager::getProgram(const String& name, const String& vertexShaderName, const String& fragmentShaderName)
{
	// If program has been loaded already return it.
	if(myPrograms.find(name) != myPrograms.end())
	{
		return myPrograms[name];
	}

	osg::Program* prog = new osg::Program();
	prog->setName(name);

	osg::Shader* vs = new osg::Shader( osg::Shader::VERTEX );
	osg::Shader* fs = new osg::Shader( osg::Shader::FRAGMENT );
	prog->addShader(vs);
	prog->addShader(fs);

	// Load shader sources.
	loadShader(vs, vertexShaderName);
	loadShader(fs, fragmentShaderName);

	myPrograms[name] = prog;

	return prog;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initShading()
{
	myShadowedScene = NULL;
	mySoftShadowMap = NULL;

	myEngine->getDisplaySystem()->setBackgroundColor(Color(0.3f, 0.3f, 0.8f, 1.0f));

	bool myShadingEnabled = (myShadowMode == ShadowsSoft);

	if(myShadingEnabled)
	{
		// compute the shadow map resolution. (trivial method: use shadow map quality index to
		// compute the size of a square, power-of-two texture)
		// We assume shadow map quality index is in the range [1, 10]
		// Shadow map size is in the range [16, 16384]
		int smHeight = 1 << (4 + myShadowMapQuality);
		int smWidth = smHeight;

		ofmsg("SceneManager::initShading: Shadow map size = (%1%x%2%)", %smWidth %smHeight);

		myShadowedScene = new osgShadow::ShadowedScene();
		myShadowedScene->setReceivesShadowTraversalMask(SceneManager::ReceivesShadowTraversalMask);
		myShadowedScene->setCastsShadowTraversalMask(SceneManager::CastsShadowTraversalMask);

		mySoftShadowMap = new osgShadow::SoftShadowMap;
		mySoftShadowMap->setTextureSize(osg::Vec2s(smWidth, smHeight));
		// Hardcoded ambient bias for shadow map. Shadowed areas receive zero light. 
		// Unshadowed areas receive full light.
		mySoftShadowMap->setAmbientBias(osg::Vec2(0.0f, 1.0f));
		// Hardcoded texture unit arguments for shadow map.
		mySoftShadowMap->setTextureUnit(4);
		mySoftShadowMap->setJitterTextureUnit(5);

		//mySoftShadowMap->setSoftnessWidth(0.005);
		//mySoftShadowMap->setJitteringScale(32);

		myShadowedScene->addChild(mySceneRoot);
		myShadowedScene->setShadowTechnique(mySoftShadowMap);
		
		mySceneRoot = myShadowedScene;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneManager::loadModel(ModelInfo info)
{
	ModelAsset* asset = new ModelAsset();
	asset->filename = info.path; /// changed filepath to filename (confirm from alassandro).
	asset->numNodes = info.numFiles;

	myModelDictionary[info.name] = asset;
	myModelList.push_back(asset);

	// Replace * with %1% so we can use our string formatting routines to add a number to the path strings.
	String orfp = StringUtils::replaceAll(info.path, "*", "%1%");
	String filePath = info.path;

	for(int iterator=1; iterator <= info.numFiles; iterator++)
	{
		// If present in the string, this line will substitute %1% with the iterator number.
		if(info.numFiles != 1)
		{
			filePath = ostr(orfp, %iterator);
		}

		String assetPath;
		if(DataManager::findFile(filePath, assetPath))
		{ 
			osgDB::Options* options = new osgDB::Options; 
			options->setOptionString("noTesselateLargePolygons noTriStripPolygons noRotation"); 

			osg::Node* node = osgDB::readNodeFile(assetPath, options);
			if(node != NULL)
			{

				if(info.size != 0.0f)
				{
					float r = node->getBound().radius() * 2;
					float scale = info.size / r;

					osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
					pat->setScale(osg::Vec3(scale, scale, scale));
					pat->addChild(node);

					node = pat;
				}

				asset->nodes.push_back(node);
				asset->description = info.description;
			}
			else
			{
				ofwarn("loading failed: %1%", %assetPath);
				// NEED TO CLEAN UP ASSET!
			}
		}
		else
		{
			ofwarn("could not find file: %1%", %filePath);
				// NEED TO CLEAN UP ASSET!
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ModelAsset* SceneManager::getModel(const String& name)
{
	return myModelDictionary[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const List<ModelAsset*>& SceneManager::getModels()
{
	return myModelList;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::createSkyBox(const String& cubeMapDir, const String& cubeMapExt)
{
	if(mySkyBox == NULL)
	{
		mySkyBox = new SkyBox();
		if(!mySkyBox->loadCubeMap(cubeMapDir, cubeMapExt))
		{
			ofwarn("SceneManager::createSkyBox: could not create skybox %1% (ext: %2%)", %cubeMapDir %cubeMapExt);
		}
	}
}

