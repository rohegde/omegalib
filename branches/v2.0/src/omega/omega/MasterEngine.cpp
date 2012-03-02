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
#include "omega/MasterEngine.h"
#include "omega/SystemManager.h"
#include "omega/DisplaySYstem.h"

using namespace omega;

MasterEngine* MasterEngine::mysInstance = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
MasterEngine::MasterEngine(ApplicationBase* app):
    ServerEngine(app, true),
    myDefaultCamera(NULL)
{
    mysInstance = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* MasterEngine::createCamera(uint flags)
{
    Camera* cam = new Camera(flags);
    myCameras.push_back(cam);
    return cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MasterEngine::destroyCamera(Camera* cam)
{
    oassert(cam != NULL);
    myCameras.remove(cam);
    delete cam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MasterEngine::CameraCollection::Range MasterEngine::getCameras()
{
    return CameraCollection::Range(myCameras.begin(), myCameras.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MasterEngine::CameraCollection::ConstRange MasterEngine::getCameras() const
{
    return CameraCollection::ConstRange(myCameras.begin(), myCameras.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MasterEngine::initialize()
{
	ServerEngine::initialize();
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MasterEngine::finalize()
{
	ServerEngine::finalize();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MasterEngine::update(const UpdateContext& context)
{
	ServerEngine::update(context);
    // Update the default camera and use it to update the default omegalib observer.
    myDefaultCamera->update(context);
    Observer* obs = getSystemManager()->getDisplaySystem()->getObserver(0);
    myDefaultCamera->updateObserver(obs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MasterEngine::handleEvent(const Event& evt)
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
    if(evt.getServiceType() == Service::Pointer && evt.getSourceId() > 0)
    {
        int pointerId = evt.getSourceId() - 1;
		refreshPointer(pointerId, evt.getPosition().x(), evt.getPosition().y());
    }
    if(evt.isProcessed()) return;
    myDefaultCamera->handleEvent(evt);
    if(evt.isProcessed()) return;
}

