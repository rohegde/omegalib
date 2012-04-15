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

#include "cyclops/SceneLoader.h"

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneLoader::SceneLoader(TiXmlDocument& doc, const String& path):
	myDoc(doc),
	myPath(path),
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
		TiXmlElement* xEntityFiles = xMapContents->FirstChildElement("Files");
		if(xEntityFiles != NULL)
		{
			loadAssets(xEntityFiles, SceneManager::ModelAssetType);
		}

		// We are looking for objects under variousy-named tags, for compatibility reasons.
		const char* sectionNames[] = {
			"Objects",
			"StaticObjects",
			"Primitives",
			"Entities"
		};

		foreach(const char* sectionName, sectionNames)
		{
			TiXmlElement* xObjects = xMapContents->FirstChildElement(sectionName);
			if(xObjects != NULL)
			{
				createObjects(root, xObjects);
			}
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
Vector4f SceneLoader::readVector4f(TiXmlElement* elem, const String& attributeName)
{
	const char* cstr = elem->Attribute(attributeName.c_str());
	if(cstr != NULL)
	{
		String str(cstr);
		std::vector<String> args = StringUtils::split(str, " ");
		return Vector4f(atof(args[0].c_str()), atof(args[1].c_str()), atof(args[2].c_str()), atof(args[3].c_str()));
	}
	return Vector4f::Zero();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f SceneLoader::readVector3f(TiXmlElement* elem, const String& attributeName)
{
	const char* cstr = elem->Attribute(attributeName.c_str());
	if(cstr != NULL)
	{
		String str(cstr);
		std::vector<String> args = StringUtils::split(str, " ");
		return Vector3f(atof(args[0].c_str()), atof(args[1].c_str()), atof(args[2].c_str()));
	}
	return Vector3f::Zero();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f SceneLoader::readVector2f(TiXmlElement* elem, const String& attributeName)
{
	const char* cstr = elem->Attribute(attributeName.c_str());
	if(cstr != NULL)
	{
		String str(cstr);
		std::vector<String> args = StringUtils::split(str, " ");
		return Vector2f(atof(args[0].c_str()), atof(args[1].c_str()));
	}
	return Vector2f::Zero();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float SceneLoader::readFloat(TiXmlElement* elem, const String& attributeName, float defaultValue)
{
	const char* cstr = elem->Attribute(attributeName.c_str());
	if(cstr != NULL)
	{
		return atof(cstr);
	}
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int SceneLoader::readInt(TiXmlElement* elem, const String& attributeName, int defaultValue)
{
	const char* cstr = elem->Attribute(attributeName.c_str());
	if(cstr != NULL)
	{
		return atoi(cstr);
	}
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLoader::loadAssets(TiXmlElement* xStaticObjectFiles, SceneManager::AssetType type)
{
	TiXmlElement* xchild = xStaticObjectFiles->FirstChildElement();
	while(xchild)
	{
		String filePath = xchild->Attribute("Path");

		// In the path, substitute ./ occurrences with the path of the xml scene file, so assets
		// in the local directory can be correctly referenced.
		// Split the path and get the file name.
		String path;
		String filename;
		String extension;
		StringUtils::splitFullFilename(myPath, filename, extension, path);
		filePath = StringUtils::replaceAll(filePath, "./", path);

		int index = atoi(xchild->Attribute("Id"));
		ofmsg("Loading asset %1%", %filePath);

		String assetPath;
		if(DataManager::findFile(filePath, assetPath))
		{ 
			osgDB::Options* options = new osgDB::Options; 
			options->setOptionString("noTesselateLargePolygons noTriStripPolygons"); 

			osg::Node* node = osgDB::readNodeFile(assetPath, options);
			if(node != NULL)
			{
				if(xchild->Attribute("Material") != NULL)
				{
					String material = xchild->Attribute("Material");
					osg::StateSet* fx = mySceneManager->loadMaterialPass(material);
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

				ModelAsset* asset = new ModelAsset();
				asset->id = index;
				asset->filename = filePath;
				asset->nodes.push_back(node);
				asset->numNodes = 1;
				mySceneManager->addAsset(asset, type);

				if(xchild->Attribute("Description") != NULL)
				{
					asset->description = xchild->Attribute("Description");
				}
				else
				{
					asset->description = asset->filename;
				}
			}
			else
			{
				ofwarn("loading failed: %1%", %assetPath);
			}
		}
		else
		{
			ofwarn("could not find file: %1%", %filePath);
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
			int id = atoi(xchild->Attribute("Id"));
			const char* tag = xchild->Attribute("Tag");
			String sTag;
			if(tag != NULL) sTag = tag;

			mySceneManager->addEntity(fileIndex, id, sTag, position, rotation, scale);
		}
		else if(objtype == "plane")
		{
			osg::Node* node = createPlane(xchild);
			mySceneManager->addNode(node, position, rotation, scale);
		}
		else if(objtype == "sphere")
		{
			osg::Node* node = createSphere(xchild);
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
	Vector2f tiling = readVector2f(xchild, "Tiling");
	String material = xchild->Attribute("Material");

	Vector3f c1(startCorner[0], startCorner[1], endCorner[2]);
	Vector3f c2(endCorner[0], startCorner[1], startCorner[2]);
	Vector3f u = (c1 - startCorner);
	Vector3f v = (c2 - startCorner);

	osg::Geometry* plane = osgwTools::makePlane(
		OOSG_VEC3(startCorner), 
		OOSG_VEC3(u), 
		OOSG_VEC3(v));
	plane->setColorArray(NULL);
	plane->setColorBinding(osg::Geometry::BIND_OFF);

	osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
	tsg->generate(plane, 0);
	osg::Vec4Array* a_tangent = tsg->getTangentArray();
	plane->setVertexAttribArray (6, a_tangent);
	plane->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* fx = mySceneManager->loadMaterialPass(material);
	fx->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(tiling[0], tiling[1])));

	node->addDrawable(plane);
	plane->setStateSet(fx);


	tsg->unref();

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* SceneLoader::createSphere(TiXmlElement* xchild)
{
	osg::Geode* node = new osg::Geode();

	float radius = readFloat(xchild, "Radius", 1.0);
	int subdivisions = readInt(xchild, "Subdivisions", 4);
	Vector2f tiling = readVector2f(xchild, "Tiling");
	String material = xchild->Attribute("Material");

	osg::Geometry* sphere = osgwTools::makeGeodesicSphere(radius, subdivisions);
	sphere->setColorArray(NULL);
	sphere->setColorBinding(osg::Geometry::BIND_OFF);

	osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
	tsg->generate(sphere, 0);
	osg::Vec4Array* a_tangent = tsg->getTangentArray();
	sphere->setVertexAttribArray (6, a_tangent);
	sphere->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* fx = mySceneManager->loadMaterialPass(material);
	fx->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(tiling[0], tiling[1])));

	node->addDrawable(sphere);
	sphere->setStateSet(fx);

	tsg->unref();

	return node;
}



