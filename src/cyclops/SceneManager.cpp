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

#include "cyclops/SceneManager.h"
#include "cyclops/SceneLoader.h"

using namespace cyclops;

SceneManager* SceneManager::mysInstance = NULL;

#ifdef OMEGA_OS_LINUX
	const int SceneManager::MaxLights;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(SceneManager* mng, ModelAsset* asset, int id):
mySceneManager(mng), myAsset(asset), myId(id), myOsgSwitch(NULL), myCurrentModelIndex(0)
{
	ServerEngine* engine = mng->getEngine();

	mySceneNode = new SceneNode(engine);
	mySceneNode->setSelectable(true);
	engine->getScene()->addChild(mySceneNode);

	OsgSceneObject* oso = NULL;
	if(asset->numNodes == 1)
	{
		// Single model asset
		oso = new OsgSceneObject(myAsset->nodes[0]);
		myOsgNode = oso->getTransformedNode();
	}
	else
	{
		// Multi model asset
		myOsgSwitch = new osg::Switch();
		int i = 0;
		foreach(osg::Node* n, myAsset->nodes)
		{
			myOsgSwitch->addChild(n);
			myOsgSwitch->setChildValue(n, i++);
		}
		oso = new OsgSceneObject(myOsgSwitch);
		myOsgNode = oso->getTransformedNode();
	}

	mySceneNode->addObject(oso);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setCurrentModelIndex(int index)
{
	if(myOsgSwitch != NULL && index < getNumModels())
	{
		myCurrentModelIndex = index;
		myOsgSwitch->setSingleChildOn(index);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Entity::getCurrentModelIndex()
{
	if(myOsgSwitch != NULL)
	{
		return myCurrentModelIndex;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager():
	myOsg(NULL),
	myShadowedScene(NULL),
	mySoftShadowMap(NULL)
{
	mysInstance = this;
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

	mySceneFilename = Config::getStringValue("scene", s, "");

	omsg("SceneManager configuration loaded");
	ofmsg("::    Shadow mode: %1%", %shadowMode);
	ofmsg("::  Default scene: %1%", %mySceneFilename);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initialize()
{
	myMainLight = NULL;

	myEngine = getServer();

	// Make sure the osg module is initialized.
	if(!myOsg->isInitialized()) myOsg->initialize();

	loadConfiguration();

	mySceneRoot = new osg::Group();

	myTabletManager = myEngine->getServiceManager()->findService<PortholeTabletService>("PortholeTabletService");

	//if(myTabletManager != NULL)
	//{
	//	myTabletManager->beginGui();
	//	myTabletManager->addGuiElement(TabletGuiElement::createButton(0, "View", "Click to switch view", "View"));
	//	myTabletManager->addGuiElement(TabletGuiElement::createSlider(1, "Local Zoom", "Select the zoom level of the tablet view", 0, 100, 30));
	//	myTabletManager->addGuiElement(TabletGuiElement::createSlider(2, "Remote Zoom", "Select the zoom level of the main view", 10, 200, 30));
	//	myTabletManager->addGuiElement(TabletGuiElement::createSwitch(3, "Rotate", "Toggle to enable object rotation", 0));
	//	myTabletManager->finishGui();
	//}

	if(myShadowMode == ShadowsSoft)
	{
		setShaderMacroToFile("use setupShadow", "cyclops/common/softShadows/setupShadow.vert");
		setShaderMacroToFile("use computeShadow", "cyclops/common/softShadows/computeShadow.frag");
	}
	else
	{
		setShaderMacroToFile("use setupShadow", "cyclops/common/noShadows/setupShadow.vert");
		setShaderMacroToFile("use computeShadow", "cyclops/common/noShadows/computeShadow.frag");
	}

	setShaderMacroToFile("use setupStandardShading", "cyclops/common/standardShading/setupStandardShading.vert");
	setShaderMacroToFile("use computeStandardShading", "cyclops/common/standardShading/computeStandardShading.frag");

	// If a default scene file has been specified, load it.
	if(mySceneFilename != "")
	{
		load(mySceneFilename);
	}

	//frontView = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::update(const UpdateContext& context) 
{
	if(myTabletManager != NULL)
	{
		myTabletManager->update(context);
	}
	updateLights();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::updateLights()
{
	for(int i = 0; i < MaxLights; i++)
	{
		Light& l = myLights[i];
		if(l.enabled)
		{
			if(l.osgLight == NULL)
			{
				l.osgLight = new osg::Light();
				l.osgLightSource = new osg::LightSource();
				mySceneRoot->addChild(l.osgLightSource);
			}

			l.osgLight->setLightNum(i);
			l.osgLight->setPosition(osg::Vec4(l.position[0], l.position[1], l.position[2], 1.0));
			l.osgLight->setAmbient(COLOR_TO_OSG(l.ambient));
			l.osgLight->setDiffuse(COLOR_TO_OSG(l.color));
			l.osgLight->setSpecular(COLOR_TO_OSG(l.color));
			l.osgLight->setConstantAttenuation(l.constAttenuation);
			l.osgLight->setLinearAttenuation(l.linearAttenuation);
			l.osgLight->setQuadraticAttenuation(l.quadAttenuation);

			l.osgLightSource->setLight(l.osgLight);

			osg::StateSet* sState = mySceneRoot->getOrCreateStateSet();
			l.osgLightSource->setStateSetModes(*sState,osg::StateAttribute::ON);
		}
		else
		{
			if(l.osgLightSource != NULL)
			{
				l.osgLightSource->setLocalStateSetModes(osg::StateAttribute::OFF); 
			}
		}
	}

	if(myMainLight != NULL)
	{
		if(mySoftShadowMap != NULL)
		{
			mySoftShadowMap->setLight(myMainLight->osgLight);
		}

		// Set ambient light uniform.
		osg::Uniform* unifAmbient = mySceneRoot->getOrCreateStateSet()->getUniform("unif_Ambient");
		if(unifAmbient == NULL)
		{
			unifAmbient = new osg::Uniform("unif_Ambient", COLOR_TO_OSG(myMainLight->ambient));
			mySceneRoot->getStateSet()->addUniform(unifAmbient, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}
		else
		{
			unifAmbient->set(COLOR_TO_OSG(myMainLight->ambient));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::handleEvent(const Event& evt) 
{
	if(myTabletManager != NULL)
	{
		myTabletManager->handleEvent(evt);
	}
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
void SceneManager::addNode(osg::Node* node)
{
	mySceneRoot->addChild(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addNode(osg::Node* node, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
{
	osg::PositionAttitudeTransform* xf = new osg::PositionAttitudeTransform();
	xf->addChild(node);

	xf->setPosition(osg::Vec3d(position[0], position[1], position[2]));
	xf->setAttitude(osg::Quat(
		rotation[0] * Math::DegToRad, osg::Vec3d(1, 0, 0),
		rotation[1] * Math::DegToRad, osg::Vec3d(0, 1, 0),
		rotation[2] * Math::DegToRad, osg::Vec3d(0, 0, 1)
		));
	xf->setScale(osg::Vec3d(scale[0], scale[1], scale[2]));

	addNode(xf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addStaticObject(int assetId, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
{
	ModelAsset* asset = getModelAsset(assetId);
	if(asset == NULL)
	{
		ofwarn("SceneManager::addStaticObject: could not locate static object asset %1%", %assetId);
	}
	else
	{
		if(asset->numNodes > 1)
		{
			ofwarn("SceneManager::addStaticObject: asset %1% has multiple models. Adding model 0 by default", %assetId);
		}
		addNode(asset->nodes[0], position, rotation, scale);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addEntity(int assetId, int entityId, const String& tag, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
{
	ModelAsset* asset = getModelAsset(assetId);
	if(asset == NULL)
	{
		ofwarn("SceneManager::addEntity: could not locate entity asset %1%", %assetId);
	}
	else
	{
		Entity* e = new Entity(this, asset, entityId);
		e->setTag(tag);
		addNode(e->getOsgNode());
		e->getSceneNode()->setPosition(position);
		e->getSceneNode()->yaw(rotation[0] * Math::DegToRad);
		e->getSceneNode()->pitch(rotation[1] * Math::DegToRad);
		e->getSceneNode()->roll(rotation[2] * Math::DegToRad);
		e->getSceneNode()->setScale(scale);
		myEntities.push_back(e);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneManager::findResource(const String& name, String& outPath)
{
	if(DataManager::findFile(name, outPath)) return true;
	//if(DataManager::findFile("images/" + name, outPath)) return true;
	//if(DataManager::findFile("../" + name, outPath)) return true;
	return false;
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
	String filename;
	String extension;
	StringUtils::splitFullFilename(name, filename, extension, path);

	if(findResource(filename + "." + extension, path))
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
			ofwarn("Image not valid: %1%", %filename);
		}
	}
	else
	{
		ofwarn("Could not find texture file %1%", %filename);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::StateSet* SceneManager::createMaterialPass(TiXmlElement* xdata, const String& type)
{
	osg::StateSet* ss = new osg::StateSet();

	osg::Program* prog = NULL;

	if(type == "textured")
	{
		prog = getProgram("solid", "cyclops/common/Textured.vert", "cyclops/common/Textured.frag");
	}
	if(type == "bump")
	{
		prog = getProgram("bump", "cyclops/common/Bump.vert", "cyclops/common/Bump.frag");
		prog->addBindAttribLocation ("Tangent", 6);
	}
	else if(type == "colored")
	{
		prog = getProgram("vertexcolor", "cyclops/common/Colored.vert", "cyclops/common/Colored.frag");
	}

	ss->setAttributeAndModes(prog, osg::StateAttribute::ON);

	TiXmlElement* xmain = xdata->FirstChildElement("Colors");
	if(xmain != NULL)
	{
		Vector4f diffuse = SceneLoader::readVector4f(xmain, "Diffuse");

		osg::Material* mat = new osg::Material();
		mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
		mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
		mat->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
		mat->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
		mat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
		ss->setAttributeAndModes(mat, osg::StateAttribute::ON);
	}

	TiXmlElement* xtextures = xdata->FirstChildElement("TextureUnits");
	if(xtextures != NULL)
	{
		TiXmlElement* xchild = xtextures->FirstChildElement();
		while(xchild)
		{
			String type = xchild->Value();
			String file = xchild->Attribute("File");

			if(file != "")
			{
				if(type == "Diffuse")
				{
					ss->addUniform( new osg::Uniform("unif_ColorMap", 0) );
					ss->setTextureAttribute(0, getTexture(file));
				}
				else if(type == "NMap")
				{
					ss->addUniform( new osg::Uniform("unif_NormalMap", 1) );
					ss->setTextureAttribute(1, getTexture(file));
				}
				else if(type == "Specular")
				{
					//mySpecular = mySceneManager->getTexture(file);
				}
				else if(type == "Height")
				{
					ss->addUniform( new osg::Uniform("unif_HeightMap", 2) );
					ss->setTextureAttribute(2, getTexture(file));
				}
			}
			xchild = xchild->NextSiblingElement();
		}
	}

	return ss;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::StateSet* SceneManager::loadMaterialPass(const String& materialName)
{
	// If material has been loaded already return it.
	if(myMaterialPasses.find(materialName) != myMaterialPasses.end())
	{
		return myMaterialPasses[materialName];
	}

	if(StringUtils::endsWith(materialName, ".material"))
	{
		String materialPath;
		if(findResource(materialName, materialPath))
		{
			// Open the material XML file.
			TiXmlDocument doc(materialPath.c_str());
			if(doc.LoadFile())
			{
				// Update the current path to the material path for texture loading
				String filename;
				String path;
				StringUtils::splitFilename(materialPath, filename, path);
				omega::DataManager::getInstance()->setCurrentPath(path);

				String type = doc.RootElement()->FirstChildElement("Main")->Attribute("Type");
				StringUtils::toLowerCase(type);
				osg::StateSet* ss = createMaterialPass(doc.RootElement(), type);
				myMaterialPasses[materialName] = ss;
				return ss;
			}
			else
			{
				ofwarn("Could not open material file %1%", %materialName);
			}
		}
		else
		{
			//ofwarn("Could not find material file %1%", %materialName);
		}
	}
	else
	{
		// We are loading something with a different extension, let's assume it's an image file.
		// First, lets try to substitute the extension with .material, and load the corresponding file.
		String newMaterialName = StringUtils::replaceAll(materialName, ".dds", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".tga", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".TGA", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".jpg", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".JPG", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".jpeg", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".JPEG", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".png", ".material");
		newMaterialName = StringUtils::replaceAll(newMaterialName, ".PNG", ".material");

		osg::StateSet* material = loadMaterialPass(newMaterialName);
		if(material == NULL)
		{
			// We could not load a material: just create a basic textured material
			// and put the texture as its diffuse map.
			osg::StateSet* ss = new osg::StateSet();
			osg::Program* prog = getProgram("solid", "cyclops/common/Textured.vert", "cyclops/common/Textured.frag");
			ss->addUniform( new osg::Uniform("unif_ColorMap", 0) );
			ss->setTextureAttribute(0, getTexture(materialName));
			ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
			//ss->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(1, 1)));

			osg::Material* mat = new osg::Material();
			mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
			mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
			mat->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1,1, 1));
			mat->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
			mat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
			ss->setAttributeAndModes(mat, osg::StateAttribute::ON);

			myMaterialPasses[materialName] = ss;
			return ss;
		}
		return material;
	}
	return NULL;
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
		myShadowedScene = new osgShadow::ShadowedScene();
		myShadowedScene->setReceivesShadowTraversalMask(SceneManager::ReceivesShadowTraversalMask);
		myShadowedScene->setCastsShadowTraversalMask(SceneManager::CastsShadowTraversalMask);

		mySoftShadowMap = new osgShadow::SoftShadowMap;
		mySoftShadowMap->setTextureSize(osg::Vec2s(1024, 1024));
		mySoftShadowMap->setAmbientBias(osg::Vec2(0.0f, 1.0f));
		mySoftShadowMap->setTextureUnit(4);
		mySoftShadowMap->setJitterTextureUnit(5);
		mySoftShadowMap->setSoftnessWidth(0.001);
		mySoftShadowMap->setJitteringScale(32);

		myShadowedScene->addChild(mySceneRoot);
		myShadowedScene->setShadowTechnique(mySoftShadowMap);
		
		mySceneRoot = myShadowedScene;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addAsset(ModelAsset* asset, AssetType type)
{
	if(type == ModelAssetType)
	{
		myModelFiles.push_back(asset);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ModelAsset* SceneManager::getModelAsset(int fileIndex)
{
	return myModelFiles[fileIndex];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Entity* SceneManager::findEntity(int id)
{
	foreach(Entity* e, myEntities)
	{
		if(e->getId() == id) return e;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
List<Entity*>::Range SceneManager::getEntities()
{
	return List<Entity*>::Range(myEntities.begin(), myEntities.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Light* SceneManager::getLight(int id)
{
	if(id >= 0 && id < MaxLights)
	{
		return &myLights[id];
	}

	oferror("SceneManager::getLight: id > MaxLights (%1% > %2%)", %id %MaxLights);
	return NULL;
}
