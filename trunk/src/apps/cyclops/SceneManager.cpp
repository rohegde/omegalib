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
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgUtil/Optimizer>
#include <osgDB/Archive>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>

#include "SceneManager.h"
#include "SceneLoader.h"

using namespace cyclops;

SceneManager* SceneManager::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(SceneManager* mng, EntityAsset* asset):
	mySceneManager(mng), myAsset(asset)
{
	EngineServer* engine = mng->getEngine();

	mySceneNode = new SceneNode(engine);
	mySceneNode->setSelectable(true);
	engine->getScene(0)->addChild(mySceneNode);
	OsgSceneObject* oso = new OsgSceneObject(myAsset->node);

	myOsgNode = oso->getTransformedNode();

	mySceneNode->addObject(oso);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager():
	myOsg(NULL)
{
	mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initialize(EngineServer* engine)
{
	myEngine = engine;
	myOsg = new OsgModule();
	myOsg->initialize(myEngine);

	myEditor = new SceneEditorModule();
	myEditor->initialize(myEngine);

	mySceneRoot = new osg::Group();

	myTabletManager = new TabletManagerModule();
	myTabletManager->initialize(myEngine);

	myTabletManager->beginGui();
	myTabletManager->addGuiElement(TabletGuiElement::createButton(0, "Button", "Test Button", "Ok"));
	myTabletManager->addGuiElement(TabletGuiElement::createSlider(1, "Slider", "Test Slider", 0, 100, 30));
	myTabletManager->finishGui();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::update(const UpdateContext& context) 
{
	myOsg->update(context);
	myEditor->update(context);
	myTabletManager->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::handleEvent(const Event& evt) 
{
	myEditor->handleEvent(evt);
	myTabletManager->handleEvent(evt);
	if(evt.isKeyDown('l'))
    {
		Vector3f pos = myEngine->getDefaultCamera()->getPosition();
		if(myLight2 != NULL)
		{
			myLight2->setPosition(Vec4(pos[0], pos[1], pos[2], 1.0f));
			ofmsg("Light Position: %1%", %pos);
		}
	}
	if(evt.getServiceType() == Service::Ui)
	{
		if(evt.getSourceId() == 1)
		{
			float value = (float)evt.getExtraDataInt(0) / 10;
			myTabletManager->getCamera()->setPosition(Vector3f(0, 0, value));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::load(SceneLoader* loader)
{
	loader->startLoading(this);
	while(!loader->isLoadingComplete()) loader->loadStep();
	if (mySceneRoot != NULL) 
	{
		osg::setNotifyLevel(INFO);
		omsg("Optimizing scene graph...");
		// Optimize scenegraph
		osgUtil::Optimizer optOSGFile;
		//optOSGFile.optimize(node, osgUtil::Optimizer::ALL_OPTIMIZATIONS);
		osg::setNotifyLevel(WARN);

		initShading();

		myOsg->setRootNode(mySceneRoot);
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
		addNode(asset->node, position, rotation, scale);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addEntity(int assetId, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
{
	EntityAsset* asset = getEntityAsset(assetId);
	if(asset == NULL)
	{
		ofwarn("SceneManager::addEntity: could not locate entity asset %1%", %assetId);
	}
	else
	{
		omsg("AddEntity");
		Entity* e = new Entity(this, asset);
		addNode(e->getOsgNode());
		e->getSceneNode()->setPosition(position);
		e->getSceneNode()->yaw(rotation[0] * Math::DegToRad);
		e->getSceneNode()->pitch(rotation[1] * Math::DegToRad);
		e->getSceneNode()->roll(rotation[2] * Math::DegToRad);
		e->getSceneNode()->setScale(scale);
		myEditor->addNode(e->getSceneNode());
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
void SceneManager::loadShader(osg::Shader* shader, const String& name)
{
	String path;
	if(DataManager::findFile(name, path))
	{
		//ofmsg("Loading shader file %1%", %name);
	    shader->loadShaderSourceFromFile(path);
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
osg::StateSet* SceneManager::createMaterial(TiXmlElement* xdata, const String& type)
{
	osg::StateSet* ss = new osg::StateSet();

	osg::Program* prog = NULL;

	if(type == "solid")
	{
		prog = getProgram("solid", "cyclops/shaders/Solid.vert", "cyclops/shaders/Solid.frag");
		prog->addBindAttribLocation ("Tangent", 6);
	}
	else if(type == "simple")
	{
		prog = getProgram("simple", "cyclops/shaders/Solid.vert", "cyclops/shaders/Simple.frag");
	}
	else if(type == "vertexcolor")
	{
		prog = getProgram("vertexcolor", "cyclops/shaders/VertexColor.vert", "cyclops/shaders/VertexColor.frag");
	}

	ss->setAttributeAndModes(prog, osg::StateAttribute::ON);

	osg::Material* mat = new osg::Material();
	mat->setColorMode(Material::AMBIENT_AND_DIFFUSE);
	mat->setDiffuse(Material::FRONT_AND_BACK, Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	ss->setAttribute(mat, osg::StateAttribute::MATERIAL);

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
					ss->addUniform( new osg::Uniform("colorMap", 0) );
					ss->setTextureAttribute(0, getTexture(file));
					//ss->setTextureAttribute(0, getTexture("rockwall.tga"));
				}
				else if(type == "NMap")
				{
					ss->addUniform( new osg::Uniform("normalMap", 1) );
					ss->setTextureAttribute(1, getTexture(file));
					//ss->setTextureAttribute(1, getTexture("rockwall_normal.tga"));
				}
				else if(type == "Specular")
				{
					//mySpecular = mySceneManager->getTexture(file);
				}
				else if(type == "Height")
				{
					ss->addUniform( new osg::Uniform("heightMap", 2) );
					ss->setTextureAttribute(2, getTexture(file));
					//ss->setTextureAttribute(2, getTexture("rockwall_height.tga"));
				}
			}
			xchild = xchild->NextSiblingElement();
		}
	}

	return ss;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::StateSet* SceneManager::loadMaterial(const String& materialName)
{
	// If material has been loaded already return it.
	if(myMaterials.find(materialName) != myMaterials.end())
	{
		return myMaterials[materialName];
	}

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
			if(type == "soliddiffuse" || type == "solid")
			{
				return createMaterial(doc.RootElement(), "solid");
			}
			else if(type == "simple")
			{
				return createMaterial(doc.RootElement(), "simple");
			}
			else if(type == "vertexcolor")
			{
				return createMaterial(doc.RootElement(), "vertexcolor");
			}
			else
			{
				ofwarn("Unknown material type %1%", %type);
			}
		}
		else
		{
			ofwarn("Could not open material file %1%", %materialName);
		}
	}
	else
	{
		ofwarn("Could not find material file %1%", %materialName);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::initShading()
{
	myEngine->getDisplaySystem()->setBackgroundColor(Color(0.8f, 0.8f, 0.8f, 1.0f));


	osgShadow::ShadowedScene* ss = new osgShadow::ShadowedScene();
	ss->setReceivesShadowTraversalMask(SceneManager::ReceivesShadowTraversalMask);
	ss->setCastsShadowTraversalMask(SceneManager::CastsShadowTraversalMask);

	osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
	sm->setTextureSize(osg::Vec2s(512, 512));
	sm->setAmbientBias(osg::Vec2(0.1f, 0.9f));
	sm->setTextureUnit(4);
	sm->setJitterTextureUnit(5);
	sm->setSoftnessWidth(0.01f);
	sm->setJitteringScale(32);

	ss->addChild(mySceneRoot);
	ss->setShadowTechnique(sm.get());

	osg::StateSet *sState = ss->getOrCreateStateSet();

	myLight2 = new osg::Light;
    myLight2->setLightNum(0);
    myLight2->setPosition(osg::Vec4(0.0, 10, 0, 1.0));
    myLight2->setAmbient(osg::Vec4(0.5f,0.5f,0.5f,1.0f));
    myLight2->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	myLight2->setSpecular(osg::Vec4(0.8f,0.8f,0.8f,1.0f));
    myLight2->setLinearAttenuation(1.0f);
    /*myLight2->setConstantAttenuation(1.0f);
    myLight2->setLinearAttenuation(2.0f/70.0f);
    myLight2->setQuadraticAttenuation(2.0f/osg::square(70.0f));*/

    osg::LightSource* lightS2 = new osg::LightSource;  

	myLight2->setPosition(Vec4(0, 6.0f, 0, 1.0f));

    lightS2->setLight(myLight2);
    lightS2->setLocalStateSetModes(osg::StateAttribute::ON); 

    lightS2->setStateSetModes(*sState,osg::StateAttribute::ON);
	ss->addChild(lightS2);

	mySceneRoot = ss;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::addAsset(ModelAsset* asset, AssetType type)
{
	if(type == ModelAssetType)
	{
		myStaticObjectFiles.push_back(asset);
	}
	else
	{
		myEntityFiles.push_back((EntityAsset*)asset);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ModelAsset* SceneManager::getModelAsset(int fileIndex)
{
	return myStaticObjectFiles[fileIndex];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EntityAsset* SceneManager::getEntityAsset(int fileIndex)
{
	return myEntityFiles[fileIndex];
}
