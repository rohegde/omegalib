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
#include <osg/Light>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaToolkit.h>
#include <omegaOsg.h>

using namespace omega;
using namespace omegaToolkit;
using namespace omegaOsg;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewer: public ServerModule
{
public:
	OsgViewer()
	{
		myOsg = new OsgModule();
		ModuleServices::addModule(myOsg); 
	}

	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt) {}

private:
	OsgModule* myOsg;
	SceneNode* mySceneNode;
	Actor* myInteractor;
	osg::Light* myLight;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewer::initialize()
{
	Config* cfg = getServer()->getSystemManager()->getAppConfig();

	osg::Node* node = NULL;

	// Load osg object
	if(cfg->exists("config/scene"))
	{
		Setting& sscene = cfg->lookup("config/scene");
		String filename = String((const char*)sscene["filename"]);

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(filename);
		if(!cfgInfo.isNull())
		{
			node = osgDB::readNodeFile(cfgInfo.path);

			// If config specifies size parameter, use it to resize the passed object.
			if(sscene.exists("size"))
			{
				float size = Config::getFloatValue("size", sscene);
				float r = node->getBound().radius() * 2;
				float scale = size / r;
				osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
				pat->setScale(osg::Vec3(scale, scale, scale));
				pat->addChild(node);
				node = pat;
			}

			if (node == NULL) 
			{
				ofwarn("!Failed to load model: %1%", %filename);
			}
			else
			{
				//Optimize scenegraph
				osgUtil::Optimizer optOSGFile;
				optOSGFile.optimize(node);
			}
		}
		else
		{
			ofwarn("!File not found: %1%", %filename);
		}
	}

    // Set the interactor style used to manipulate meshes.
    String interactorStyle = cfg->lookup("config/interactorStyle");
    if(interactorStyle == "Mouse")
    {
        DefaultMouseInteractor* interactor = new DefaultMouseInteractor();
        interactor->setMoveButtonFlag(Event::Left);
        interactor->setRotateButtonFlag(Event::Right);
        myInteractor = interactor;
    }
    else if(interactorStyle == "Controller")
    {
        ControllerManipulator* interactor = new ControllerManipulator();
        myInteractor = interactor;
    }
    else
    {
        DefaultTwoHandsInteractor* interactor =  new DefaultTwoHandsInteractor();
        interactor->initialize("ObserverUpdateService");
        myInteractor = interactor;
    }
	ModuleServices::addModule(myInteractor);

	// Create an omegalib scene node and attach the osg node to it. This is used to interact with the 
	// osg object through omegalib interactors.
	OsgSceneObject* oso = new OsgSceneObject(node);
	mySceneNode = new SceneNode(getServer());
	mySceneNode->addObject(oso);
	getServer()->getScene()->addChild(mySceneNode);
	getServer()->getDefaultCamera()->focusOn(getServer()->getScene());
	myInteractor->setSceneNode(mySceneNode);

	// Set the osg node as the root node
	myOsg->setRootNode(oso->getTransformedNode());

	// Setup shading
	myLight = new osg::Light;
    myLight->setLightNum(0);
    myLight->setPosition(osg::Vec4(100.0, 100, 0, 0.0));
    myLight->setAmbient(osg::Vec4(0.4f,0.4f,0.4f,1.0f));
    myLight->setDiffuse(osg::Vec4(0.4f,0.4f,0.4f,1.0f));
	myLight->setSpotExponent(0);
	myLight->setSpecular(osg::Vec4(0.0f,0.0f,0.0f,1.0f));



	node->getOrCreateStateSet()->setAttribute(myLight);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewer::update(const UpdateContext& context) 
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<OsgViewer> app("osgviewer");

	// Read config file name from command line or use default one.
	const char* cfgName = "osgviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "osgviewer.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
