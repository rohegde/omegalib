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
#include "FlockSimulator.h" 
#include "FlockObject.h" 

///////////////////////////////////////////////////////////////////////////////////////////////////
class FlockApplication: public EngineModule
{
public:
	FlockApplication();

	virtual void initialize();
	virtual void update(const UpdateContext&);
	virtual void handleEvent(const Event& evt);

private:
	SceneManager* mySceneManager;
	FlockSimulator* myFlockSimulator;
	FlockObject* myFlock;
	FlockSettings mySettings;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
FlockApplication::FlockApplication()
{
	// Read flock settings from the configuration file.
    Config* cfg = getEngine()->getSystemManager()->getAppConfig();
    if(cfg->exists("config"))
    {
        Setting& sCfg = cfg->lookup("config");
        mySettings.load(sCfg);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockApplication::initialize()
{
	// Create and initialize the cyclops scene manager.
	// If a scene file is specified in the application config file, the scene manager will
	// load it during initialization.
	mySceneManager = SceneManager::createAndInitialize();

	// Create and initialize the flock simulator
	myFlockSimulator = new FlockSimulator();
	myFlockSimulator->setup(&mySettings);
	myFlockSimulator->initialize();

	// Create and initialize the flock object. We assume 'FlockAgent' is the name of a mesh
	// specified in the .scene file for this application. We will use that mesh as the 3d model
	// for the flock agents.
	myFlock = new FlockObject(mySceneManager, myFlockSimulator, "FlockAgent", "Flock");

	// Setup lighting
	cyclops::Light* l = new cyclops::Light(mySceneManager);
	l->setEnabled(true);
	l->setPosition(Vector3f(0, 10, 0));
	l->setColor(Color(1.0f, 1.0f, 0.7f));
	l->setAmbient(Color(0.1f, 0.1f, 0.1f));
	mySceneManager->setMainLight(l);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockApplication::update(const UpdateContext& context)
{
	// Update the simulation and the flock object.
	myFlockSimulator->update(context);
	myFlock->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void FlockApplication::handleEvent(const Event& evt)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<FlockApplication> app("flock");
	return omain(app, argc, argv);
}
