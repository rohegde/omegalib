/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omega/ImageUtils.h"
#include "omega/SystemManager.h"
#include "omega/PythonInterpreter.h"
#include "omega/CameraController.h"

using namespace omega;

Engine* Engine::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::Engine(ApplicationBase* app):
    //myActivePointerTimeout(2.0f),
    myDefaultCamera(NULL),
    myConsoleEnabled(false),
	//myPointerMode(PointerModeWand)
	myDrawPointers(false),
	myDebugWand(false)
{
    mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::~Engine()
{
	omsg("~Engine");
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

    myDefaultCamera = new Camera(this);
	// By default attach camera to scene root.
	myScene->addChild(myDefaultCamera);

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

	// Initialize the default camera using the 
	//Observer* obs = getDisplaySystem()->getObserver(0);
	//myDefaultCamera->setPosition(obs->getHeadPosition());
	myDefaultCamera->setPosition(Vector3f::Zero());

	myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::dispose()
{
	omsg("Engine::dispose");

    ImageUtils::internalDispose();
	ModuleServices::disposeAll();

	// Clear renderer list.
	myClients.clear();

	// Clear root scene node.
	myScene = NULL;

	ofmsg("Engine::dispose: cleaning up %1% cameras", %myCameras.size());
	myCameras.clear();
	myDefaultCamera = NULL;

	// clean up Pointers.
	for(int i = 0; i < MaxPointers; i++) myPointers[i] = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::addClient(Renderer* client)
{
    oassert(client != NULL);
    myClients.push_back(client);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void Engine::removeRenderPass(const String& renderPassName)
{
	ofmsg("Engine: removing render pass %1%", %renderPassName);
	foreach(Renderer* r, myClients)
	{
		RenderPass* rp = r->getRenderPass(renderPassName);
		if(rp != NULL)
		{
			r->removeRenderPass(rp);
		}
		else
		{
			ofwarn("Engine::removeRenderPass: could not find render pass %1%", %renderPassName);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
void Engine::refreshPointer(int pointerId, const Event& evt)
{
	Pointer* ptr = myPointers[pointerId];

	ptr->setVisible(true);
	ptr->setPointerMode(Pointer::ModeWand);
	ptr->setPosition(evt.getPosition().x(), evt.getPosition().y());

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
	if(myDebugWand)
	{
		if(evt.getServiceType() == Event::ServiceTypeWand)
		{
			ofmsg("|Wand position: %1% flags %2%", %evt.getPosition() %evt.getFlags());
		}
	}

    ModuleServices::handleEvent(evt);

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
			//refreshPointer(pointerId, evt);
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
	myDefaultCamera->update(context);
	foreach(Camera* c, myCameras)
	{
		c->update(context);
	}

    ModuleServices::update(this, context);

	getSystemManager()->getScriptInterpreter()->update(context);

    myScene->update(false, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const SceneQueryResultList& Engine::querySceneRay(const Ray& ray, uint flags)
{
    myRaySceneQuery.clearResults();
	myRaySceneQuery.setSceneNode(myScene.get());
    myRaySceneQuery.setRay(ray);
    return myRaySceneQuery.execute(flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::drawPointers(Renderer* client, const DrawContext& context)
{
	if(myDrawPointers)
	{
		foreach(Pointer* p, myPointers)
		{
			if(p->getVisible())
			{
				Renderable* r = p->getRenderable(client);
				r->draw(context);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* Engine::createCamera(uint flags)
{
    Camera* cam = new Camera(this, flags);
	// By default attach camera to scene root.
	myScene->addChild(cam);
    myCameras.push_back(cam);
    return cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::destroyCamera(Camera* cam)
{
    oassert(cam != NULL);
    myCameras.remove(cam);
    //delete cam;
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

///////////////////////////////////////////////////////////////////////////////////////////////////
int Engine::getCanvasWidth() 
{
	return getDisplaySystem()->getCanvasSize().x(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Engine::getCanvasHeight()
{
	return getDisplaySystem()->getCanvasSize().y(); 
}
