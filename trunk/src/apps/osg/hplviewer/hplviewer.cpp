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
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgwTools/Shapes.h>

#include <osgFX/Cartoon>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>
#include <oosg.h>

using namespace omega;
using namespace oengine;
using namespace oosg;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HplViewer: public EngineServer
{
public:
	HplViewer(Application* app): EngineServer(app) {}
	virtual void initialize();
	virtual void update(const UpdateContext& context);

	osg::Node* loadMapXml(TiXmlDocument& doc);

private:
	void loadStaticObjectFiles(TiXmlElement* xStaticObjectFiles);
	void createStaticObjects(osg::Group* root, TiXmlElement* xStaticObjectFiles);
	void createPrimitives(osg::Group* root, TiXmlElement* xStaticObjectFiles);

	Vector3f readVector3f(TiXmlElement* elem, const String& attributeName);
	Vector2f readVector2f(TiXmlElement* elem, const String& attributeName);

private:
	OsgModule* myOsg;
	SceneNode* mySceneNode;
	Vector<osg::Node*> myStaticObjectFiles;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f HplViewer::readVector3f(TiXmlElement* elem, const String& attributeName)
{
	String str(elem->Attribute(attributeName.c_str()));
	std::vector<String> args = StringUtils::split(str, " ");
	return Vector3f(atof(args[0].c_str()), atof(args[1].c_str()), atof(args[2].c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Vector2f HplViewer::readVector2f(TiXmlElement* elem, const String& attributeName)
{
	String str(elem->Attribute(attributeName.c_str()));
	std::vector<String> args = StringUtils::split(str, " ");
	return Vector2f(atof(args[0].c_str()), atof(args[1].c_str()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* HplViewer::loadMapXml(TiXmlDocument& doc)
{
	osg::Group* root = new osg::Group();
	TiXmlElement* xroot = doc.RootElement();
	TiXmlElement* xlevel = xroot->FirstChildElement("MapData");
	TiXmlElement* xMapContents = xlevel->FirstChildElement("MapContents");
	if(xMapContents != NULL)
	{
		TiXmlElement* xStaticObjectFiles = xMapContents->FirstChildElement("FileIndex_StaticObjects");
		if(xStaticObjectFiles != NULL)
		{
			loadStaticObjectFiles(xStaticObjectFiles);
		}

		TiXmlElement* xStaticObjects = xMapContents->FirstChildElement("StaticObjects");
		if(xStaticObjects != NULL)
		{
			createStaticObjects(root, xStaticObjects);
		}

		TiXmlElement* xPrimitives = xMapContents->FirstChildElement("Primitives");
		if(xPrimitives != NULL)
		{
			createPrimitives(root, xPrimitives);
		}
	}

	return root;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HplViewer::loadStaticObjectFiles(TiXmlElement* xStaticObjectFiles)
{
	TiXmlElement* xchild = xStaticObjectFiles->FirstChildElement();
	while(xchild)
	{
		const char* filePath = xchild->Attribute("Path");
		int index = atoi(xchild->Attribute("Id"));
		ofmsg("Loading static object %1%", %index);
		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(String(filePath) + ".fbx.obj");
		if(!cfgInfo.isNull())
		{
			osg::Node* node = osgDB::readNodeFile(cfgInfo.path);
			if(node != NULL)
			{
				//osgFX::Effect* fx = new osgFX::Cartoon();
				//fx->addChild(node);
				myStaticObjectFiles.push_back(node);
			}
		}

		xchild = xchild->NextSiblingElement();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HplViewer::createStaticObjects(osg::Group* root, TiXmlElement* xStaticObjects)
{
	TiXmlElement* xchild = xStaticObjects->FirstChildElement();
	while(xchild)
	{
		int fileIndex = atoi(xchild->Attribute("FileIndex"));
		int id = atoi(xchild->Attribute("ID"));

		Vector3f rotation = readVector3f(xchild, "Rotation");
		Vector3f position = readVector3f(xchild, "WorldPos");
		Vector3f scale = readVector3f(xchild, "Scale");

		ofmsg("Object %1% position %2% rotation %3% scale %4%", %id %position %rotation %scale);

		osg::Node* node = myStaticObjectFiles[fileIndex];
		if(node != NULL)
		{
			osg::PositionAttitudeTransform* xf = new osg::PositionAttitudeTransform();
			xf->addChild(node);
			root->addChild(xf);

			xf->setPosition(osg::Vec3d(position[0], -position[1], position[2]));
			xf->setAttitude(osg::Quat(
				rotation[0] - Math::Pi / 2, osg::Vec3d(1, 0, 0),
				rotation[1], osg::Vec3d(0, 1, 0),
				rotation[2], osg::Vec3d(0, 0, 1)
				));
			xf->setScale(osg::Vec3d(scale[0], scale[1], -scale[2]));
		}
		xchild = xchild->NextSiblingElement();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HplViewer::createPrimitives(osg::Group* root, TiXmlElement* xStaticObjects)
{
	TiXmlElement* xchild = xStaticObjects->FirstChildElement();
	while(xchild)
	{
		osg::Geode* node = new osg::Geode();
		String primType = xchild->Value();
		if(primType == "Plane")
		{
			Vector3f startCorner = readVector3f(xchild, "StartCorner");
			Vector3f endCorner = readVector3f(xchild, "EndCorner");

			Vector3f c1(startCorner[0], startCorner[1], endCorner[2]);
			Vector3f c2(endCorner[0], startCorner[1], startCorner[2]);
			Vector3f u = (startCorner - c1);
			Vector3f v = (startCorner - c2);
			//u.normalize();
			//v.normalize();

			node->addDrawable(osgwTools::makePlane(
				OOSG_VEC3(startCorner), 
				OOSG_VEC3(u), 
				OOSG_VEC3(v)));
		}

		int id = atoi(xchild->Attribute("ID"));

		Vector3f rotation = readVector3f(xchild, "Rotation");
		Vector3f position = readVector3f(xchild, "WorldPos");
		Vector3f scale = readVector3f(xchild, "Scale");

		ofmsg("Primitive %1% position %2% rotation %3% scale %4%", %id %position %rotation %scale);

		if(node != NULL)
		{
			osg::PositionAttitudeTransform* xf = new osg::PositionAttitudeTransform();
			xf->addChild(node);
			root->addChild(xf);

			xf->setPosition(osg::Vec3d(position[0], -position[1], position[2]));
			xf->setAttitude(osg::Quat(
				rotation[0], osg::Vec3d(1, 0, 0),
				rotation[1], osg::Vec3d(0, 1, 0),
				rotation[2], osg::Vec3d(0, 0, 1)
				));
			xf->setScale(osg::Vec3d(scale[0], scale[1], -scale[2]));
		}
		xchild = xchild->NextSiblingElement();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HplViewer::initialize()
{
	EngineServer::initialize();
	Config* cfg = getSystemManager()->getAppConfig();

	myOsg = new OsgModule();
	myOsg->initialize(this);

	// Load osg object
	if(cfg->exists("config/dataPath"))
	{
		String dataPath = String((const char*)cfg->lookup("config/dataPath"));
		DataManager* dm = SystemManager::instance()->getDataManager();
		dm->addSource(new FilesystemDataSource(dataPath));
	}

	// Load osg object
	if(cfg->exists("config/mapName"))
	{
		String mapName = String((const char*)cfg->lookup("config/mapName"));

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(mapName);
		if(!cfgInfo.isNull())
		{
			TiXmlDocument doc(cfgInfo.path.c_str());
			if(doc.LoadFile())
			{
				ofmsg("Loading map: %1%...", %mapName);
				osg::Node* node = loadMapXml(doc);
				if (node == NULL) 
				{
					ofwarn("!Failed to load map: %1%", %mapName);
				}
				else
				{
					// Optimize scenegraph
					//osgUtil::Optimizer optOSGFile;
					//optOSGFile.optimize(node, osgUtil::Optimizer::ALL_OPTIMIZATIONS);

					myOsg->setRootNode(node);
				}
			}
		}
		else
		{
			ofwarn("!File not found: %1%", %mapName);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HplViewer::update(const UpdateContext& context) 
{
	EngineServer::update(context);
	myOsg->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<HplViewer> app;

	// Read config file name from command line or use default one.
	const char* cfgName = "hplviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "hplviewer.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
