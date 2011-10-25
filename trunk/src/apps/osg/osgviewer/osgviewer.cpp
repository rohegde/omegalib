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

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>
#include <oosg.h>

using namespace omega;
using namespace oengine;
using namespace oosg;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewer: public EngineServer
{
public:
	OsgViewer(Application* app): EngineServer(app) {}
	virtual void initialize();
	virtual void update(const UpdateContext& context);

private:
	OsgModule* myOsg;
	SceneNode* mySceneNode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewer::initialize()
{
	EngineServer::initialize();
	Config* cfg = getSystemManager()->getAppConfig();

	myOsg = new OsgModule();
	myOsg->initialize(this);

	// Load osg object
	if(cfg->exists("config/scene"))
	{
		Setting& sscene = cfg->lookup("config/scene");
		String filename = String((const char*)sscene["filename"]);

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(filename);
		if(!cfgInfo.isNull())
		{
			osg::Node* node = osgDB::readNodeFile(cfgInfo.path);
			if (node == NULL) 
			{
				ofwarn("!Failed to load model: %1%", %filename);
			}
			else
			{
				//Optimize scenegraph
				osgUtil::Optimizer optOSGFile;
				optOSGFile.optimize(node);

				myOsg->setRootNode(node);
				getDefaultCamera()->focusOn(getScene(0), 0.2f);
			}
		}
		else
		{
			ofwarn("!File not found: %1%", %filename);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewer::update(const UpdateContext& context) 
{
	EngineServer::update(context);
	myOsg->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<OsgViewer> app;

	// Read config file name from command line or use default one.
	const char* cfgName = "osgviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "osgviewer.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
