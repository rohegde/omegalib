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

	// Create a reference box around the scene.
	myReferenceBox = new ReferenceBox();
	getSceneManager()->getRootNode()->addRenderable(myReferenceBox);
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

	myVtkClient = onew(VtkClient)(this);
	myVtkClient->initialize();
	//myVtkClient->runScript("vtk/Sample.py");

	Teapot* tp = onew(Teapot)();
	tp->setSize(0.1f);
	getSceneManager()->getRootNode()->addRenderable(tp);

	myCurrentInteractor->setSceneNode(getSceneManager()->getRootNode());
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
