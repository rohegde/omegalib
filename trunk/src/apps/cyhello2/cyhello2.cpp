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
 *-------------------------------------------------------------------------------------------------
 *	cyhello2
 *		A slightly more advanced cyclops application. Loads a mesh and lets the user manipulate it
 **************************************************************************************************/
#include <omega.h>
#include <cyclops.h>

using namespace omega;
using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
class HelloApplication: public ServerModule
{
public:
	HelloApplication();
	virtual void initialize();
	virtual void update(const UpdateContext& context);

private:
	SceneManager* mySceneManager;
	StaticObject* myObject;
	//SceneEditorModule* myEditor;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
HelloApplication::HelloApplication():
	mySceneManager(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HelloApplication::initialize()
{
	// Create and initialize the cyclops scene manager.
	mySceneManager = SceneManager::createAndInitialize();

	// Load some basic model from the omegalib meshes folder.
	// Force the size of the object to be 0.8 meters
	mySceneManager->loadModel(ModelInfo("simpleModel", "meshes/t600arm.fbx", 1.0f));

	// Create a new object using the loaded model (referenced using its name, 'simpleModel')
	myObject = new StaticObject(mySceneManager, "simpleModel");
	// Apply a rendering effect to the loade object.
	myObject->setEffect("blueprint");

	// Create a plane for reference.
	PlaneShape* plane = new PlaneShape(mySceneManager, 4, 4);
	plane->setEffect("colored -d #88888888");
	plane->pitch(-90 * Math::DegToRad);
	plane->setPosition(0, -1, -2);

	// Setup a light for the scene.
	Light* light = mySceneManager->getLight(0);
	light->enabled = true;
	light->position = Vector3f(0, 50, 1);
	light->color = Color(1.0f, 1.0f, 0.7f);
	light->ambient = Color(0.2f, 0.2f, 0.3f);
	mySceneManager->setMainLight(light);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HelloApplication::update(const UpdateContext& context)
{
	// Animate our loaded object.
	myObject->setPosition(0, Math::sin(context.time) * 0.5f + 0.5f, -2);
	myObject->pitch(context.dt);
	myObject->yaw(context.dt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<HelloApplication> app("cyhello");
	return omain(app, argc, argv);
}

