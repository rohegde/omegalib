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
#include "osgviewer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::initialize()
{
	Config* cfg = getSystemManager()->getAppConfig();

	// Load entity library
	if(cfg->exists("config/scene"))
	{
		Setting& sscene = cfg->lookup("config/scene");
		String filename = (String)sscene["filename"];

		myEntity = onew(OsgEntity)();
		myEntity->load(filename);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::finalize()
{
	odelete(myEntity);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::update(const UpdateContext& context) 
{
	if(myEntity != NULL)
	{
		myEntity->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::handleEvent(const Event& evt) 
{ 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::initialize()
{
	EngineClient::initialize();

	// Add an osg render pass to the scene manager so osg objects can be rendered
	myRenderPass = onew(OsgRenderPass)();
	getSceneManager()->addRenderPass(myRenderPass, true);

	myEntityNode = onew(SceneNode)(getSceneManager());
	getSceneManager()->getRootNode()->addChild(myEntityNode);

	myServer->getEntity()->addToScene(myEntityNode);

	// Set the interactor style used to manipulate meshes.
	Config* cfg = getSystemManager()->getAppConfig();
	String interactorStyle = cfg->lookup("config/interactorStyle");
	if(interactorStyle == "Mouse")
	{
		DefaultMouseInteractor* interactor = onew(DefaultMouseInteractor)();
		myCurrentInteractor = interactor;
	}
	else
	{
		DefaultTwoHandsInteractor* interactor = onew(DefaultTwoHandsInteractor)();
		interactor->initialize("ObserverUpdateService");
		myCurrentInteractor = interactor;
	}

	myCurrentInteractor->setSceneNode(myEntityNode);

	getSceneManager()->addActor(myCurrentInteractor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::finalize()
{
	odelete(myEntityNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::draw( const DrawContext& context)
{
    EngineClient::draw( context );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::handleEvent(const Event& evt)
{
    EngineClient::handleEvent(evt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::update(const UpdateContext& context)
{
	return EngineClient::update( context );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	OsgViewerApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "osgviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "osgviewer.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
