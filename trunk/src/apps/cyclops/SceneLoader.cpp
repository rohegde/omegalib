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
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/OcclusionQueryNode>
#include <osg/Depth>
#include <osg/ColorMask>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osgUtil/TangentSpaceGenerator>
#include <osgFX/Effect>
#include <osgDB/ReadFile>
#include <osgwTools/Shapes.h>

#include "SceneLoader.h"

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneLoader::SceneLoader(TiXmlDocument& doc):
	myDoc(doc),
	mySceneManager(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLoader::startLoading(SceneManager* sm)
{
	mySceneManager = sm;

	osg::Group* root = new osg::Group();
	TiXmlElement* xroot = myDoc.RootElement();
	TiXmlElement* xlevel = xroot->FirstChildElement("MapData");
	TiXmlElement* xMapContents = xlevel->FirstChildElement("MapContents");
	if(xMapContents != NULL)
	{
		TiXmlElement* xStaticObjectFiles = xMapContents->FirstChildElement("FileIndex_StaticObjects");
		if(xStaticObjectFiles != NULL)
		{
			loadAssets(xStaticObjectFiles, SceneManager::ModelAssetType);
		}

		TiXmlElement* xEntityFiles = xMapContents->FirstChildElement("FileIndex_Entities");
		if(xEntityFiles != NULL)
		{
			loadAssets(xEntityFiles, SceneManager::EntityAssetType);
		}

		TiXmlElement* xStaticObjects = xMapContents->FirstChildElement("StaticObjects");
		if(xStaticObjects != NULL)
		{
			createObjects(root, xStaticObjects);
		}

		TiXmlElement* xPrimitives = xMapContents->FirstChildElement("Primitives");
		if(xPrimitives != NULL)
		{
			createObjects(root, xPrimitives);
		}

		TiXmlElement* xEntities = xMapContents->FirstChildElement("Entities");
		if(xEntities != NULL)
		{
			createObjects(root, xEntities);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneLoader::isLoadingComplete()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int SceneLoader::getLoadProgress()
{
	return 100;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLoader::loadStep()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f SceneLoader::readVector3f(TiXmlElement* elem, const String& attributeName)
{
	String str(elem->Attribute(attributeName.c_str()));
	std::vector<String> args = StringUtils::split(str, " ");
	return Vector3f(atof(args[0].c_str()), atof(args[1].c_str()), atof(args[2].c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f SceneLoader::readVector2f(TiXmlElement* elem, const String& attributeName)
{
	String str(elem->Attribute(attributeName.c_str()));
	std::vector<String> args = StringUtils::split(str, " ");
	return Vector2f(atof(args[0].c_str()), atof(args[1].c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLoader::loadAssets(TiXmlElement* xStaticObjectFiles, SceneManager::AssetType type)
{
	TiXmlElement* xchild = xStaticObjectFiles->FirstChildElement();
	while(xchild)
	{
		const char* filePath = xchild->Attribute("Path");
		int index = atoi(xchild->Attribute("Id"));
		//ofmsg("Loading static object %1%", %index);
		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(String(filePath));
		if(!cfgInfo.isNull())
		{
			osg::Node* node = osgDB::readNodeFile(cfgInfo.path);

			if(xchild->Attribute("Material") != NULL)
			{
				String material = xchild->Attribute("Material");
				osg::StateSet* fx = mySceneManager->loadMaterial(material);
				node->setStateSet(fx);
			}

			if(xchild->Attribute("Size") != NULL)
			{
				float size = atof(xchild->Attribute("Size"));

				float r = node->getBound().radius() * 2;

				float scale = size / r;

				osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
				pat->setScale(osg::Vec3(scale, scale, scale));
				pat->addChild(node);

				node = pat;
			}

			if(node != NULL)
			{
				ModelAsset* asset = NULL;
				if(type == SceneManager::ModelAssetType)
				{
					asset = new ModelAsset();
				}
				else
				{
					asset = new EntityAsset();
				}
				asset->id = index;
				asset->filename = filePath;
				asset->node = node;
				mySceneManager->addAsset(asset, type);
			}
		}

		xchild = xchild->NextSiblingElement();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLoader::createObjects(osg::Group* root, TiXmlElement* xObjects)
{
	TiXmlElement* xchild = xObjects->FirstChildElement();
	while(xchild)
	{
		String objtype = xchild->Value();
		StringUtils::toLowerCase(objtype);

		Vector3f rotation = readVector3f(xchild, "Rotation");
		Vector3f position = readVector3f(xchild, "WorldPos");
		Vector3f scale = readVector3f(xchild, "Scale");

		if(objtype == "staticobject")
		{
			int fileIndex = atoi(xchild->Attribute("FileIndex"));
			mySceneManager->addStaticObject(fileIndex, position, rotation, scale);
		}
		else if(objtype == "entity")
		{
			int fileIndex = atoi(xchild->Attribute("FileIndex"));
			mySceneManager->addEntity(fileIndex, position, rotation, scale);
		}
		else if(objtype == "plane")
		{
			osg::Node* node = createPlane(xchild);
			mySceneManager->addNode(node, position, rotation, scale);
		}
		
		xchild = xchild->NextSiblingElement();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* SceneLoader::createPlane(TiXmlElement* xchild)
{
	osg::Geode* node = new osg::Geode();

	Vector3f startCorner = readVector3f(xchild, "StartCorner");
	Vector3f endCorner = readVector3f(xchild, "EndCorner");
	String material = xchild->Attribute("Material");

	Vector3f c1(startCorner[0], startCorner[1], endCorner[2]);
	Vector3f c2(endCorner[0], startCorner[1], startCorner[2]);
	Vector3f u = (c1 - startCorner);
	Vector3f v = (c2 - startCorner);

	osg::Geometry* plane = osgwTools::makePlane(
		OOSG_VEC3(startCorner), 
		OOSG_VEC3(u), 
		OOSG_VEC3(v));

	osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
	tsg->generate(plane, 0);
	osg::Vec4Array* a_tangent = tsg->getTangentArray();
	plane->setVertexAttribArray (6, a_tangent);
	plane->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* fx = mySceneManager->loadMaterial(material);
	node->setStateSet(fx);

	node->addDrawable(plane);

	tsg->unref();

	return node;
}




