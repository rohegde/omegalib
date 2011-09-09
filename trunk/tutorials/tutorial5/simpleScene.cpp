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
#include <oengine.h>

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimpleScene: public EngineServer
{
public:
	SimpleScene(Application* app): EngineServer(app) {}
	virtual void initialize();
	virtual void update(const UpdateContext& context);

private:
	SceneNode* myCubeNode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleScene::initialize()
{
	EngineServer::initialize();

	SceneNode* scene = getScene(0);

	myCubeNode = new SceneNode(this);
	//Box* cube = new Box();

	PlyDataReader* meshData = new PlyDataReader();
	meshData->readPlyFile("meshes/walker.ply");
	meshData->scale(0.8f);

	Mesh* mesh = new Mesh();
	
	scene->addChild(myCubeNode);
	myCubeNode->addObject(mesh);

	mesh->setData(meshData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleScene::update(const UpdateContext& context)
{
	myCubeNode->roll(0.1f * context.dt);
	myCubeNode->yaw(0.2f * context.dt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<SimpleScene> app;

	// Read config file name from command line or use default one.
	// NOTE: being a simple application, ohello does not have any application-specific configuration option. 
	// So, we are going to load directly a system configuration file.
	const char* cfgName = "system/simple-480p.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "simpleScene.log", new FilesystemDataSource(OMEGA_DATA_PATH));
	
	return 0;
}
