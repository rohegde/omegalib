/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/SageManager.h"

using namespace omega;

Engine* Engine::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::Engine(ApplicationBase* app):
    //myActivePointerTimeout(2.0f),
    myDefaultCamera(NULL),
    myConsoleEnabled(false),
	//myPointerMode(PointerModeWand)
	myDrawPointers(false),
	myDebugWand(false),
	myPrimaryButton(Event::Button3)
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

	ModuleServices::addModule(new EventSharingModule());

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
	// camera section = default camera only
	if(syscfg->exists("config/camera"))
	{
        Setting& s = syscfg->lookup("config/camera");
		myDefaultCamera->setup(s);
	}

	// Load camera config form system config file
	// cameras section = multiple camera specifications
	if(syscfg->exists("config/cameras"))
	{
        Setting& s = syscfg->lookup("config/cameras");
		for(int i = 0; i < s.getLength(); i++)
		{
			Setting& sc = s[i];
			String camName = sc.getName();
			if(camName == "default") 
			{
				myDefaultCamera->setup(sc);
			}
			else
			{
				Camera* cam = getCamera(camName);
				if(cam == NULL) cam = createCamera(camName);
				cam->setup(sc);
			}
		}
	}

	// Load sound config from system config file
	// On distributed systems, this is executed only on the master node
	if(syscfg->exists("config/sound"))
	{
        Setting& s = syscfg->lookup("config/sound");

		String soundServerIP = Config::getStringValue("soundServerIP", s, "localhost");
		int soundServerPort = Config::getIntValue("soundServerPort", s, 57120);

		soundManager = new SoundManager(soundServerIP,soundServerPort);
		soundManager->startSoundServer();

		soundEnv = soundManager->getSoundEnvironment();
		soundEnv->setListenerPosition( getDefaultCamera()->getPosition() );
		soundEnv->setListenerOrientation( getDefaultCamera()->getOrientation() );

		soundEnv->setUserPosition( Vector3f(0.0,1.8f,0.0) );
	}

	// Load input mapping
	if(syscfg->exists("config/inputMap"))
	{
        Setting& s = syscfg->lookup("config/inputMap");
		myPrimaryButton = Event::parseButtonName(Config::getStringValue("confirmButton", s, "Button3"));
	}


	// Load camera config form application config file (if it is different from system configuration)
    if(cfg != syscfg && cfg->exists("config/camera"))
    {
        Setting& s = cfg->lookup("config/camera");
		myDefaultCamera->setup(s);
    }


	Setting& scfg = cfg->lookup("config");
	myEventSharingEnabled = Config::getBoolValue("enableEventSharing", scfg, true);

	// Initialize the default camera using the 
	//Observer* obs = getDisplaySystem()->getObserver(0);
	//myDefaultCamera->setPosition(obs->getHeadPosition());
	myDefaultCamera->setPosition(Vector3f::Zero());

	ModuleServices::setNonCoreMode();

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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::reset()
{
	// dispose non-core modules
	ModuleServices::disposeNonCoreModules();

	// Remove all children from the scene root.
	myScene->removeAllChildren();

	myDefaultCamera->resetOrientation();
	myDefaultCamera->setPosition(Vector3f::Zero());
	myDefaultCamera->getController()->reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::addRenderer(Renderer* client)
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
	Vector3f pos = evt.getPosition();

	Pointer* ptr = NULL;
	if(myPointers.find(pointerId) == myPointers.end())
	{
		ofmsg("Engine::refreshPointer: creating pointer %1%", %pointerId);
		ptr = new Pointer();
		myPointers[pointerId] = ptr;
		ptr->initialize(this);
	}
	else
	{
		ptr = myPointers[pointerId];
}
	ptr->setVisible(true);
	ptr->setPosition(pos[0], pos[1]);
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

	if( evt.getServiceType() == Service::Keyboard )
	{
		// Tab = toggle on-screen console.
		if(evt.getSourceId() == 259 && evt.getType() == Event::Down) 
		{
			setConsoleEnabled(!isConsoleEnabled());
		}
	}

	// Update pointers.
	if(myDrawPointers)
	{
		if(evt.getServiceType() == Service::Pointer) 
		{
			refreshPointer(evt.getSourceId(), evt);
		}
	}
	if(!evt.isProcessed()) 
	{
		myDefaultCamera->handleEvent(evt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Engine::update(const UpdateContext& context)
{
	// First update the script
	getSystemManager()->getScriptInterpreter()->update(context);

	// Then run update on modules
    ModuleServices::update(this, context);
	
	// Update cameras after modules, to make sure the camera view transform keeps into account 
	// changes applied during script or module execution.
	//myDefaultCamera->updateCamera(context);
	//foreach(Camera* c, myCameras)
	//{
	//	c->updateCamera(context);
	//}
	
	// Run update on the scene graph.
	myScene->update(context);

	if( soundEnv != NULL )
	{
		// Update the listener position with the camera's 'navigative' position
		soundEnv->setListenerPosition( getDefaultCamera()->getPosition() );
		soundEnv->setListenerOrientation( getDefaultCamera()->getOrientation() );

		// Update the user position with the head tracker's position
		soundEnv->setUserPosition( getDefaultCamera()->getHeadOffset() );
	}
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
		typedef pair<int, Ref<Pointer> > PointerItem;
		foreach(PointerItem i, myPointers)
		{
			Pointer* p = i.second;
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
Camera* Engine::createCamera(const String& name, uint flags)
{
	Camera* cam = createCamera(flags);
	cam->setName(name);
	return cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* Engine::getCamera(const String& name)
{
	foreach(Camera* cam, myCameras)
	{
		if(cam->getName() == name) return cam;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::CameraCollection Engine::getCameras()
{
    return myCameras;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine::CameraCollection Engine::getCameras() const
{
    return myCameras;
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

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderer* Engine::getRendererByContextId(int id)
{
	foreach(Renderer* r, myClients)
	{
		if(r->getGpuContext()->getId() == id) return r;
	}
	return NULL;
}

