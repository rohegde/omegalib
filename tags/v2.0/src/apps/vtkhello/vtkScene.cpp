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
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>

#include <omega.h>
#include <omegaToolkit.h>
#include <omegaVtk.h>

using namespace omega;
using namespace omegaToolkit;
using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VtkScene: public ServerModule
{
public:
	VtkScene();
	virtual void initialize();
	virtual void initializeRenderer(Renderer* r);

private:
	VtkModule* myVtkModule;
	SceneNode* mySceneNode;
	DefaultMouseInteractor* myMouseInteractor;
	vtkSphereSource* mySphere;

	// Client objecs.
	RendererObject<vtkPolyDataMapper*> myPolyDataMapper;
	RendererObject<vtkActor*> myActor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkScene::VtkScene() 
{
	// Create and register the omegalib vtk module.
	myVtkModule = new VtkModule();
	ModuleServices::addModule(myVtkModule);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkScene::initialize()
{
	// As simple as it gets: create a sphere in vtk.
	mySphere = vtkSphereSource::New(); 
	mySphere->SetRadius(0.20); 
	mySphere->SetThetaResolution(18); 
	mySphere->SetPhiResolution(18);

	// Create an omegalib scene node. We will attach our vtk objects to it.
	mySceneNode = new SceneNode(getServer(), "vtkRoot");
	mySceneNode->setPosition(0, 0, -1);
	mySceneNode->setBoundingBoxVisible(true);
	getServer()->getScene()->addChild(mySceneNode);

	// Create a mouse interactor and associate it with our scene node.
	myMouseInteractor = new DefaultMouseInteractor();
	myMouseInteractor->setSceneNode(mySceneNode);
	ModuleServices::addModule(myMouseInteractor);

	// Setup the camera
	getServer()->getDefaultCamera()->focusOn(getServer()->getScene());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkScene::initializeRenderer(Renderer* r)
{
	// For each omegalib renderer thread, create vtk polydata mappers and actors.
	myPolyDataMapper[r] = vtkPolyDataMapper::New();
	myPolyDataMapper[r]->SetInput(mySphere->GetOutput());
	myActor[r] = vtkActor::New(); 
	myActor[r]->SetMapper(myPolyDataMapper[r]); 
	myActor[r]->GetProperty()->SetColor(0,0,1);
	//myActor[r]->SetPosition(0,0,-1);

	// Here is where the magic happens: use the omegalib vtk module to attach the vtk actor to the
	// node we created in initialize. 
	myVtkModule->beginClientInitialize(r);
	myVtkModule->attachProp(myActor[r], mySceneNode);
	myVtkModule->endClientInitialize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<VtkScene> app("vtkhello");
	return omain(app, argc, argv);
}
