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
#include "vtkviewer.h"
#include "VtkRenderPass.h"

// Vtk includes
#include <vtkCylinderSource.h>
#include <vtkContourFilter.h>
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkImageData.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
void VtkViewerClient::initialize()
{
	EngineClient::initialize();

	Config* cfg = getSystemManager()->getAppConfig();

	// Setup the system font.
	if(cfg->exists("config/defaultFont"))
	{
		Setting& fontSetting = cfg->lookup("config/defaultFont");
		getFontManager()->createFont("default", fontSetting["filename"], fontSetting["size"]);
	}

	VtkDrawable* vdw = initVtk();

	// Create a reference box around the scene.
	myReferenceBox = new ReferenceBox();
	getSceneManager()->getRootNode()->addDrawable(myReferenceBox);
	myReferenceBox->setSize(Vector3f(4.0f, 4.0f, 4.0f));

	// Set the interactor style used to manipulate meshes.
	String interactorStyle = cfg->lookup("config/interactorStyle");
	if(interactorStyle == "Mouse")
	{
		DefaultMouseInteractor* interactor = new DefaultMouseInteractor();
		myCurrentInteractor = interactor;
	}
	else
	{
		DefaultTwoHandsInteractor* interactor = new DefaultTwoHandsInteractor();
		interactor->initialize("ObserverUpdateService");
		myCurrentInteractor = interactor;
	}
	getSceneManager()->addActor(myCurrentInteractor);

	SceneNode* sn = new SceneNode(getSceneManager());
	getSceneManager()->getRootNode()->addChild(sn);

	BoundingSphereDrawable* ss = new BoundingSphereDrawable();
	ss->setVisible(false);
	ss->setDrawOnSelected(true);

	sn->addDrawable(vdw);
	sn->addDrawable(ss);

	VtkRenderPass* vrp = new VtkRenderPass();
	vrp->initialize();
	getSceneManager()->addRenderPass(vrp);

	myCurrentInteractor->setSceneNode(sn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
VtkDrawable* VtkViewerClient::initVtk()
{
	vtkCylinderSource* source						= NULL;
	vtkPolyDataMapper* mapper						= NULL;
	vtkActor* actor									= NULL;

	// create the quadric function definition
	vtkQuadric *quadric = vtkQuadric::New();
	quadric->SetCoefficients(.5,1,.3,0,.1,0,0,.2,0,0);

	// sample the quadric function
	vtkSampleFunction *sample = vtkSampleFunction::New();
	sample->SetSampleDimensions(50,50,50);
	sample->SetImplicitFunction(quadric);

	// Create five surfaces F(x,y,z) = constant between range specified
	vtkContourFilter *contours = vtkContourFilter::New();
	contours->SetInput(sample->GetOutput());
	contours->GenerateValues(3, 0.0, 1.2);

	// map the contours to graphical primitives
	vtkPolyDataMapper *contMapper = vtkPolyDataMapper::New();
	contMapper->SetInput(contours->GetOutput());
	contMapper->SetScalarRange(0.0, 1.2); 
  
	// Actor in scene
	actor = vtkActor::New();
	actor->SetScale(0.2f);
	actor->SetMapper(contMapper);

	VtkDrawable* vdw = new VtkDrawable();
	vdw->setActor(actor);

	return vdw;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	VtkViewerApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "vtkviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "vtkviewer.log", new FilesystemDataSource("./../../data/"));

	return 0;
}
