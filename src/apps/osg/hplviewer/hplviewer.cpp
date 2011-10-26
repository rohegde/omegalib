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
	OsgModule* myOsg;
	SceneNode* mySceneNode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* HplViewer::loadMapXml(TiXmlDocument& doc)
{
	Vector<osg::Node*> staticObjectFiles;
	osg::Group* root = new osg::Group();
	TiXmlElement* xroot = doc.RootElement();
	TiXmlElement* xlevel = xroot->FirstChildElement("MapData");
	TiXmlElement* xMapContents = xlevel->FirstChildElement("MapContents");
	if(xMapContents != NULL)
	{
		TiXmlElement* xStaticObjectFiles = xMapContents->FirstChildElement("FileIndex_StaticObjects");
		if(xStaticObjectFiles != NULL)
		{
			TiXmlElement* xchild = xStaticObjectFiles->FirstChildElement();
			while(xchild)
			{
				const char* filePath = xchild->Attribute("Path");
				int index = atoi(xchild->Attribute("Id"));
				ofmsg("Loading static object %1%", %index);
				//staticObjectFiles[index] = String(filePath) + ".fbx.obj";
				//staticObjectFiles.push_back(String(filePath) + ".fbx.obj");

				DataManager* dm = SystemManager::instance()->getDataManager();
				DataInfo cfgInfo = dm->getInfo(String(filePath) + ".fbx.obj");
				if(!cfgInfo.isNull())
				{
					osg::Node* node = osgDB::readNodeFile(cfgInfo.path);
					if(node != NULL)
					{
						staticObjectFiles.push_back(node);
					}
				}

				xchild = xchild->NextSiblingElement();
			}
		}

		TiXmlElement* xStaticObjects = xMapContents->FirstChildElement("StaticObjects");
		if(xStaticObjects)
		{
			TiXmlElement* xchild = xStaticObjects->FirstChildElement();
			while(xchild)
			{
				int fileIndex = atoi(xchild->Attribute("FileIndex"));
				int id = atoi(xchild->Attribute("ID"));

				ofmsg("Placing object %1%", %id);

				String strRotation(xchild->Attribute("Rotation"));
				String strPos(xchild->Attribute("WorldPos"));

				std::vector<String> rotArgs = StringUtils::split(strRotation, " ");
				std::vector<String> posArgs = StringUtils::split(strPos, " ");

				Vector3f rotation(atof(rotArgs[0].c_str()), atof(rotArgs[1].c_str()), atof(rotArgs[2].c_str()));
				Vector3f position(atof(posArgs[0].c_str()), atof(posArgs[1].c_str()), atof(posArgs[2].c_str()));

				osg::Node* node = staticObjectFiles[fileIndex];
				if(node != NULL)
				{
					osg::PositionAttitudeTransform* xf = new osg::PositionAttitudeTransform();
					xf->addChild(node);
					root->addChild(xf);

					xf->setPosition(osg::Vec3d(position[0], position[2], position[1]));
					xf->setAttitude(osg::Quat(
						rotation[0], osg::Vec3d(1, 0, 0),
						rotation[1], osg::Vec3d(0, 0, 1),
						rotation[2], osg::Vec3d(0, 1, 0)
						));
				}
				xchild = xchild->NextSiblingElement();
			}
		}
	}


	return root;
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
					//Optimize scenegraph
					osgUtil::Optimizer optOSGFile;
					optOSGFile.optimize(node);

					myOsg->setRootNode(node);
					getDefaultCamera()->focusOn(getScene(0));
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
