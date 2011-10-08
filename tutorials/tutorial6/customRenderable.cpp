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
#include "TexturedQuad.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleScene: public EngineServer
{
public:
	SimpleScene(Application* app): EngineServer(app) {}
	virtual void initialize();

private:
	SceneNode* mySceneNode;
	TexturedQuad* myTexturedQuad;
	ImageData myImageData;
	BoundingSphere* mySelectionSphere;
	DefaultMouseInteractor* myMouseInteractor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleScene::initialize()
{
	EngineServer::initialize();

	SceneNode* scene = getScene(0);

	mySceneNode = new SceneNode(this);

	myTexturedQuad = new TexturedQuad();
	
	mySelectionSphere = new BoundingSphere();
	mySelectionSphere->setDrawOnSelected(true);
	mySelectionSphere->setVisible(false);

	scene->addChild(mySceneNode);
	mySceneNode->addObject(myTexturedQuad);
	mySceneNode->addObject(mySelectionSphere);

	myMouseInteractor = new DefaultMouseInteractor();
	myMouseInteractor->setSceneNode(mySceneNode);
	addActor(myMouseInteractor);

	Config* cfg = getSystemManager()->getAppConfig();
	if(cfg->exists("config"))
	{
		Setting& sCfg = cfg->lookup("config");
		String imageName = sCfg["imageName"];
		// Load the image to be used for texturing the quad
		if(ImageUtils::loadImage(imageName, &myImageData))
		{
			myTexturedQuad->setImage(&myImageData);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<SimpleScene> app;
	omain(
		app, 
		"../tutorials/tutorial6/customRenderable.cfg", 
		"customRenderable.log", 
		new FilesystemDataSource(OMEGA_DATA_PATH));
	return 0;
}
