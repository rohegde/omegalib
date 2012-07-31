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
#include "omega/Engine.h"
#include "omega/Application.h"
#include "omega/Renderable.h"
#include "omega/Observer.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omega/ImageUtils.h"
#include "omega/SystemManager.h"
#include "omega/PythonInterpreter.h"
#include "omega/CameraController.h"

using namespace omega;

List<EngineModule*> ModuleServices::mysModules;
Engine* Engine::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::Engine(ApplicationBase* app):
    ServerBase(app),
    //myActivePointerTimeout(2.0f),
    myDefaultCamera(NULL),
    myConsoleEnabled(false),
	//myPointerMode(PointerModeWand)
	myDrawPointers(false)
{
    mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::~Engine()
{
    ImageUtils::internalDispose();
	ModuleServices::cleanup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::initialize()
{
    myLock.lock();
    ImageUtils::internalInitialize();

    myScene = new SceneNode(this, "root");

    // Create console.
    myConsole = new Console();
    myConsole->initialize(this);
    ologaddlistener(myConsole.get());

	// Then in the system config
	Config* syscfg = getSystemManager()->getSystemConfig();
    // Setup the console default font
    Config* cfg = getSystemManager()->getAppConfig();

    if(syscfg->exists("config/console/font"))
    {
        Setting& fontSetting = syscfg->lookup("config/console/font");
        myConsole->setFont(FontInfo("console", fontSetting["filename"], fontSetting["size"]));
    }
    else
    {
        myConsole->setFont(FontInfo("console", "fonts/arial.ttf", 12));
    }

    // Setup the system font.
	// Look in the app config first
    if(cfg->exists("config/defaultFont"))
    {
        Setting& fontSetting = cfg->lookup("config/defaultFont");
        setDefaultFont(FontInfo("default", fontSetting["filename"], fontSetting["size"]));
    }
	else
	{
		if(syscfg->exists("config/defaultFont"))
		{
			Setting& fontSetting = syscfg->lookup("config/defaultFont");
			setDefaultFont(FontInfo("default", fontSetting["filename"], fontSetting["size"]));
		}
		else
		{
			// If all else fails, set a default fallback font.
			setDefaultFont(FontInfo("console", "fonts/arial.ttf", 12));
		}
	}

	// Read draw pointers option.
	myDrawPointers = syscfg->getBoolValue("config/drawPointers", myDrawPointers);
	myDrawPointers = cfg->getBoolValue("config/drawPointers", myDrawPointers);

    myDefaultCamera = new Camera();

	// Load camera config form system config file
	if(syscfg->exists("config/camera"))
	{
        Setting& s = syscfg->lookup("config/camera");
		myDefaultCamera->setup(s);
	}

	// Load camera config form application config file
    if(cfg->exists("config/camera"))
    {
        Setting& s = cfg->lookup("config/camera");
		myDefaultCamera->setup(s);
    }

	Setting& scfg = cfg->lookup("config");
	myEventSharingEnabled = Config::getBoolValue("enableEventSharing", scfg, true);

	for(int i = 0; i < MaxPointers; i++)
	{
		myPointers[i] = new Pointer();
		myPointers[i]->initialize(this);
	}
	
	myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::clientInitialize(Renderer* client)
{
    myLock.lock();
	ModuleServices::initializeRenderer(this, client);
    myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::preDraw(Renderer* r, const DrawContext& context)
{
    myLock.lock();
	ModuleServices::preDraw(this, r, context);
    myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::postDraw(Renderer* r, const DrawContext& context)
{
    myLock.lock();
	ModuleServices::postDraw(this, r, context);
    myLock.unlock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::addClient(Renderer* client)
{
    oassert(client != NULL);
    myClients.push_back(client);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void Engine::refreshPointer(int pointerId, const Event& evt)
{
	Pointer* ptr = myPointers[pointerId];

	ptr->setVisible(true);
	ptr->setPointerMode(Pointer::ModeWand);
	ptr->setPosition(evt.getPosition(0), evt.getPosition(1));

	// Generate a view ray for the event.
	Ray ray;
	if(getDisplaySystem()->getViewRayFromEvent(evt, ray))
	{
		ptr->setRay(ray);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* Engine::getScene()
{
    return myScene.get();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::handleEvent(const Event& evt)
{
    ModuleServices::handleEvent(this, evt);

	getSystemManager()->getScriptInterpreter()->handleEvent(evt);

    if(!evt.isProcessed())
	{
		if( evt.getServiceType() == Service::Keyboard )
		{
			// Tab = toggle on-screen console.
			if(evt.getSourceId() == 259 && evt.getType() == Event::Down) 
			{
				setConsoleEnabled(!isConsoleEnabled());
			}
		}

		// Update pointers.
		// NOTE: 0 is reserved for the local mouse pointer.
		if(evt.getServiceType() == Service::Pointer  || evt.getServiceType() == Service::Wand/*&& evt.getSourceId() > 0*/)
		{
			int pointerId = evt.getSourceId();
			refreshPointer(pointerId, evt);
		}
		if(!evt.isProcessed()) 
		{
			myDefaultCamera->handleEvent(evt);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::update(const UpdateContext& context)
{
    ModuleServices::update(this, context);

	getSystemManager()->getScriptInterpreter()->update(context);

    myScene->update(false, false);

    // Update the default camera and use it to update the default omegalib observer.
    myDefaultCamera->update(context);
    Observer* obs = getSystemManager()->getDisplaySystem()->getObserver(0);
    myDefaultCamera->updateObserver(obs);}

///////////////////////////////////////////////////////////////////////////////////////////////////
const SceneQueryResultList& Engine::querySceneRay(const Ray& ray, uint flags)
{
    myRaySceneQuery.clearResults();
	myRaySceneQuery.setSceneNode(myScene.get());
    myRaySceneQuery.setRay(ray);
    return myRaySceneQuery.execute(flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::drawPointers(Renderer* client, RenderState* state)
{
	if(myDrawPointers)
	{
		foreach(Pointer* p, myPointers)
		{
			if(p->getVisible())
			{
				Renderable* r = p->getRenderable(client);
				r->draw(state);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* Engine::createCamera(uint flags)
{
    Camera* cam = new Camera(flags);
    myCameras.push_back(cam);
    return cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::destroyCamera(Camera* cam)
{
    oassert(cam != NULL);
    myCameras.remove(cam);
    delete cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::CameraCollection::Range Engine::getCameras()
{
    return CameraCollection::Range(myCameras.begin(), myCameras.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::CameraCollection::ConstRange Engine::getCameras() const
{
    return CameraCollection::ConstRange(myCameras.begin(), myCameras.end());
}
