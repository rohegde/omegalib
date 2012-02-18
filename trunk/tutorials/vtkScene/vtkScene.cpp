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
#include <omegaVtk.h>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>

using namespace omega;
using namespace omegaToolkit;
using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VtkScene: public EngineServer
{
public:
	VtkScene(Application* app);
	virtual void onInitialize();
	virtual void onClientInitialize(EngineClient* cli);

private:
	VtkModule* myVtkModule;
	SceneNode* mySceneNode;
	DefaultMouseInteractor* myMouseInteractor;
	VtkSceneObject* myVtkObject;
	vtkSphereSource* mySphere;

	// Client objecs.
	ClientObject<vtkPolyDataMapper*> myPolyDataMapper;
	ClientObject<vtkActor*> myActor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkScene::VtkScene(Application* app): 
	EngineServer(app) 
{
	myVtkModule = new VtkModule();
	EngineModuleServices::addModule(myVtkModule);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkScene::onInitialize()
{
	// As simple as it gets: create a sphere in vtk.
	mySphere = vtkSphereSource::New(); 
	mySphere->SetRadius(0.20); 
	mySphere->SetThetaResolution(18); 
	mySphere->SetPhiResolution(18);

	mySceneNode = new SceneNode(this);
	getScene(0)->addChild(mySceneNode);

	myVtkObject = new VtkSceneObject("vtk");
	mySceneNode->addObject(myVtkObject);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkScene::onClientInitialize(EngineClient* cli)
{
	myPolyDataMapper[cli] = vtkPolyDataMapper::New();
	myPolyDataMapper[cli]->SetInput(mySphere->GetOutput());
	myActor[cli] = vtkActor::New(); 
	myActor[cli]->SetMapper(myPolyDataMapper[cli]); 
	myActor[cli]->GetProperty()->SetColor(0,0,1);
	myActor[cli]->SetPosition(0, 0, -1);

	myVtkModule->beginClientInitialize(cli);
	myVtkModule->attachActor(myActor[cli], myVtkObject);
	myVtkModule->endClientInitialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<VtkScene> app;

	// Read config file name from command line or use default one.
	// NOTE: being a simple application, ohello does not have any application-specific configuration option. 
	// So, we are going to load directly a system configuration file.
	const char* cfgName = "system/desktop.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "vtkScene.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
