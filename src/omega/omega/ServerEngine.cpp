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
#include "omega/ServerEngine.h"
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

List<ServerModule*> ModuleServices::mysModules;
ServerEngine* ServerEngine::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
ServerEngine::ServerEngine(ApplicationBase* app, bool master):
    ServerBase(app, master),
    myActivePointerTimeout(2.0f),
    myDefaultCamera(NULL),
    myConsoleEnabled(false),
	myPointerMode(PointerModeWand)
{
    mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ServerEngine::~ServerEngine()
{
    ImageUtils::internalDispose();
	ModuleServices::cleanup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::initialize()
{
    myLock.lock();
    ImageUtils::internalInitialize();

    myScene = new SceneNode(this, "root");

    // Create console.
    myConsole = new Console();
    myConsole->initialize(this);
    ologaddlistener(myConsole.get());

    // Setup the console default font
    Config* cfg = getSystemManager()->getAppConfig();
    if(cfg->exists("config/console/font"))
    {
        Setting& fontSetting = cfg->lookup("config/console/font");
        myConsole->setFont(FontInfo("default", fontSetting["filename"], fontSetting["size"]));
    }
    else
    {
        myConsole->setFont(FontInfo("console", "fonts/arial.ttf", 12));
    }

    // Setup the system font.
    if(cfg->exists("config/defaultFont"))
    {
        Setting& fontSetting = cfg->lookup("config/defaultFont");
        setDefaultFont(FontInfo("default", fontSetting["filename"], fontSetting["size"]));
    }

    myDefaultCamera = new Camera();
	Setting& scfg = cfg->lookup("config");
    if(cfg->exists("config/camera"))
    {
        Setting& s = cfg->lookup("config/camera");
        if(Config::getBoolValue("enableNavigation", s, false))
        {
            myDefaultCamera->setController(new KeyboardMouseCameraController());
        }
        Vector3f camPos = Config::getVector3fValue("position", s); 
        myDefaultCamera->setPosition(camPos);
    }

	myEventSharingEnabled = Config::getBoolValue("enableEventSharing", scfg, true);

	for(int i = 0; i < MaxPointers; i++)
	{
		myPointers[i] = new Pointer();
		myPointers[i]->initialize(this);
	}
	
	myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::clientInitialize(Renderer* client)
{
    myLock.lock();
	ModuleServices::initializeRenderer(this, client);
    myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::preDraw(Renderer* r, const DrawContext& context)
{
    myLock.lock();
	ModuleServices::preDraw(this, r, context);
    myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::postDraw(Renderer* r, const DrawContext& context)
{
    myLock.lock();
	ModuleServices::postDraw(this, r, context);
    myLock.unlock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::addClient(Renderer* client)
{
    oassert(client != NULL);
    myClients.push_back(client);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::refreshPointer(int pointerId, const Event& evt)
{
	Pointer* ptr = myPointers[pointerId];

	ptr->setVisible(true);

	//if(myActivePointers[pointerId].first == NULL)
	//{
	//	ofmsg("Creating pointer %1%", %pointerId);
	//	myActivePointers[pointerId].first = createPointer();
	//}
	//myActivePointers[pointerId].second = myActivePointerTimeout;
	//if(evt.getType() == Event::Update)
	//{
	//    myActivePointers[pointerId].first->setText(evt.getExtraDataString());
	//    myActivePointers[pointerId].first->setColor(
	//        Color(evt.getPosition()[0], evt.getPosition()[1], evt.getPosition()[2]));
	//}
	//else
	//{
	//	myActivePointers[pointerId].first->setPosition(x, y);
	//}

	// Set pointer mode.
	if(myPointerMode == PointerModeMouse) ptr->setPointerMode(Pointer::ModeMouse);
	else if(myPointerMode == PointerModeWand) ptr->setPointerMode(Pointer::ModeWand);
	else
	{
		if(evt.getType() == Event::Down && evt.isFlagSet(Event::Middle))
		{
			if(ptr->getPointerMode() == Pointer::ModeMouse)
			{
				ofmsg("Pointer %1%: switching to wand mode", %pointerId);
				ptr->setPointerMode(Pointer::ModeWand);
			}
			else
			{
				ofmsg("Pointer %1%: switching to mouse mode", %pointerId);
				ptr->setPointerMode(Pointer::ModeMouse);
			}
		}
	}

	ptr->setPosition(evt.getPosition(0), evt.getPosition(1));
	// If pointer contains ray information, pass it to the pointer.
	if(evt.getExtraDataItems() == 2 && 
		evt.getExtraDataType() == Event::ExtraDataVector3Array)
	{
		ptr->setRay(Ray(evt.getExtraDataVector3(0), evt.getExtraDataVector3(1)));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* ServerEngine::getScene()
{
    return myScene.get();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::handleEvent(const Event& evt)
{
    ModuleServices::handleEvent(this, evt);

	getSystemManager()->getScriptInterpreter()->handleEvent(evt);

    if(!evt.isProcessed())
	{
		if( evt.getServiceType() == Service::Keyboard )
		{
			// Esc = force exit
			if(evt.getSourceId() == 256) exit(0);
			// Tab = toggle on-screen console.
			if(evt.getSourceId() == 259 && evt.getType() == Event::Down) 
			{
				setConsoleEnabled(!isConsoleEnabled());
			}
		}

		// Update pointers.
		// NOTE: 0 is reserved for the local mouse pointer.
		if(evt.getServiceType() == Service::Pointer && evt.getSourceId() > 0)
		{
			int pointerId = evt.getSourceId() - 1;
			refreshPointer(pointerId, evt);
		}
		if(!evt.isProcessed()) 
		{
			myDefaultCamera->handleEvent(evt);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::update(const UpdateContext& context)
{
    ModuleServices::update(this, context);

	getSystemManager()->getScriptInterpreter()->update(context);

    myScene->update(false, false);

    // Update the default camera and use it to update the default omegalib observer.
    myDefaultCamera->update(context);
    Observer* obs = getSystemManager()->getDisplaySystem()->getObserver(0);
    myDefaultCamera->updateObserver(obs);}

///////////////////////////////////////////////////////////////////////////////////////////////////
const SceneQueryResultList& ServerEngine::querySceneRay(const Ray& ray, uint flags)
{
    myRaySceneQuery.clearResults();
	myRaySceneQuery.setSceneNode(myScene.get());
    myRaySceneQuery.setRay(ray);
    return myRaySceneQuery.execute(flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::drawPointers(Renderer* client, RenderState* state)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* ServerEngine::createCamera(uint flags)
{
    Camera* cam = new Camera(flags);
    myCameras.push_back(cam);
    return cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ServerEngine::destroyCamera(Camera* cam)
{
    oassert(cam != NULL);
    myCameras.remove(cam);
    delete cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ServerEngine::CameraCollection::Range ServerEngine::getCameras()
{
    return CameraCollection::Range(myCameras.begin(), myCameras.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ServerEngine::CameraCollection::ConstRange ServerEngine::getCameras() const
{
    return CameraCollection::ConstRange(myCameras.begin(), myCameras.end());
}
