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
#include <omega.h>
#include <omegaToolkit.h>

using namespace omega;
using namespace omegaToolkit;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleScene: public EngineServer
{
public:
	SimpleScene(Application* app): EngineServer(app) {}
	virtual void initialize();

private:
	SceneNode* mySceneNode;
	MeshData* myMeshData;
	Mesh* myMesh;
	DefaultMouseInteractor* myMouseInteractor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleScene::initialize()
{
	EngineServer::initialize();

	// Read a mesh filename from the config file and load it.
	Config* cfg = getSystemManager()->getAppConfig();
	if(cfg->exists("config"))
	{
		Setting& sCfg = cfg->lookup("config");

		myMeshData = MeshUtils::load(sCfg["mesh"]);
		myMeshData->scale(0.8f);
	}

	SceneNode* scene = getScene(0);

	mySceneNode = new SceneNode(this);

	myMesh = new Mesh();
	
	scene->addChild(mySceneNode);
	mySceneNode->addObject(myMesh);

	myMesh->setData(myMeshData);

	myMouseInteractor = new DefaultMouseInteractor();
	myMouseInteractor->setSceneNode(mySceneNode);
	addActor(myMouseInteractor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<SimpleScene> app;
	omain(
		app, 
		"../tutorials/tutorial5/simpleScene.cfg", 
		"simpleScene.log", 
		new FilesystemDataSource(OMEGA_DATA_PATH));
	return 0;
}
