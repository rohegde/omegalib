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
#include "omega/EngineServer.h"
#include "omega/EngineApplication.h"
#include "omega/Renderable.h"
//#include "omega/PortholeTabletService.h"
#include "omega/Observer.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"

using namespace omega;


List<IEngineModule*> EngineModuleServices::mysModules;

///////////////////////////////////////////////////////////////////////////////////////////////////
EngineServer::EngineServer(Application* app):
	ApplicationServer(app),
	myActivePointerTimeout(2.0f),
	myDefaultCamera(NULL),
	myConsoleEnabled(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::initialize()
{
	for(int i = 0; i < MaxScenes; i++)
	{
		myScene[i] = new SceneNode(this, "root");
	}

	myDefaultCamera = new Camera();

	Config* cfg = getSystemManager()->getAppConfig();
	if(cfg->exists("config/camera"))
	{
		Setting& s = cfg->lookup("config/camera");
		if(Config::getBoolValue("enableNavigation", s, false))
		{
			myDefaultCamera->setNavigationMode(Camera::NavFreeFly);
		}
		Vector3f camPos = cfg->getVector3fValue("position", s); 
		myDefaultCamera->setPosition(camPos);
	}

	// Create console.
	myConsole = new Console();
	myConsole->initialize(this);
	ologaddlistener(myConsole);

	// Setup the console default font
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

	// Initialize modules
	EngineModuleServices::initialize(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* EngineServer::createCamera(uint flags)
{
	Camera* cam = new Camera(flags);
	myCameras.push_back(cam);
	return cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::destroyCamera(Camera* cam)
{
	oassert(cam != NULL);
	myCameras.remove(cam);
	delete cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EngineServer::CameraCollection::Range EngineServer::getCameras()
{
	return CameraCollection::Range(myCameras.begin(), myCameras.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EngineServer::CameraCollection::ConstRange EngineServer::getCameras() const
{
	return CameraCollection::ConstRange(myCameras.begin(), myCameras.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::addRenderPass(String renderPass, void* userData, bool addToFront)
{
	RenderPassFactory rpNew = myRenderPassFactories[renderPass];
	if(rpNew != NULL)
	{
		ofmsg("EngineServer: adding render pass %1%", %renderPass);
		foreach(EngineClient* client, myClients)
		{
			RenderPass* rp = rpNew(client);
			rp->setUserData(userData);
			client->addRenderPass(rp, addToFront);
		}
	}
	else
	{
		ofwarn("EngineServer::addRenderPass - render pass not found: %1%", %renderPass);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::removeAllRenderPasses()
{
	foreach(EngineClient* client, myClients)
	{
		client->removeAllRenderPasses();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::removeRenderPass(String renderPass)
{
	oassert(false); // Not implemented.
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::addClient(EngineClient* client)
{
	oassert(client != NULL);
	myClients.push_back(client);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::addActor(Actor* actor)
{
	myActors.push_back(actor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::removeActor(Actor* actor)
{
	myActors.remove(actor);
}

///////////////////////////////////////////////////////////////////////////////////////////////
Pointer* EngineServer::createPointer()
{
	Pointer* p = new Pointer();
	p->initialize(this);
	myPointers.push_back(p);
	return p;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::destroyPointer(Pointer* p)
{
	myPointers.remove(p);
	p->dispose();
	delete p;
}

///////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* EngineServer::getScene(int id)
{
	oassert(id >= 0 && id < EngineServer::MaxScenes);
	return myScene[id];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::update(const UpdateContext& context)
{
	EngineModuleServices::update(context);

	// Update pointers
	for(int i = 0; i < MaxActivePointers; i++)
	{
		if(myActivePointers[i].first != NULL)
		{
			myActivePointers[i].second -= context.dt;
			if(myActivePointers[i].second <= 0)
			{
				ofmsg("Destroying pointer %1%", %i);
				destroyPointer(myActivePointers[i].first);
				myActivePointers[i].first = 0;
			}
		}
	}

	// Update actors.
	foreach(Actor* a, myActors)
	{
		a->update(context);
	}

	// Update the default camera and use it to update the default omegalib observer.
	myDefaultCamera->update(context);
	Observer* obs = getSystemManager()->getDisplaySystem()->getObserver(0);
	myDefaultCamera->updateObserver(obs);

	// Update scene.
	for(int i = 0; i < MaxScenes; i++)
	{
		myScene[i]->update(false, false);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::handleEvent(const Event& evt)
{
	if( evt.getServiceType() == Service::Keyboard )
    {
		// Esc = force exit
        if(evt.getSourceId() == 256) exit(0);
		// Tab = toggle on-screen console.
		if(evt.getSourceId() == 259 && evt.getType() == Event::Down) 
		{
			myConsoleEnabled = !myConsoleEnabled;
		}
	}

	EngineModuleServices::handleEvent(evt);
	if(evt.isProcessed()) return;

	foreach(Actor* a, myActors)
	{
		a->handleEvent(evt);
	}
	if(evt.isProcessed()) return;

	myDefaultCamera->handleEvent(evt);

	// Update pointers.
	if(evt.getServiceType() == Service::Pointer && evt.getSourceId() > 0)
	{
		int pointerId = evt.getSourceId() - 1;
		if(myActivePointers[pointerId].first == NULL)
		{
			ofmsg("Creating pointer %1%", %pointerId);
			myActivePointers[pointerId].first = createPointer();
		}
		myActivePointers[pointerId].second = myActivePointerTimeout;
		if(evt.getType() == Event::Update)
		{
			myActivePointers[pointerId].first->setText(evt.getExtraDataString());
			myActivePointers[pointerId].first->setColor(
				Color(evt.getPosition()[0], evt.getPosition()[1], evt.getPosition()[2]));
		}
		else
		{
			myActivePointers[pointerId].first->setPosition(evt.getPosition().x(), evt.getPosition().y());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const SceneQueryResultList& EngineServer::querySceneRay(int sceneId, const Ray& ray, uint flags)
{
	myRaySceneQuery.clearResults();
	myRaySceneQuery.setSceneNode(myScene[sceneId]);
	myRaySceneQuery.setRay(ray);
	return myRaySceneQuery.execute(flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EngineServer::drawPointers(EngineClient* client, RenderState* state)
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
