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
#include "omega/EqualizerDisplaySystem.h"
#include "omega/MouseService.h"
#include "omega/KeyboardService.h"
#include "omega/StringUtils.h"
#include "omega/Observer.h"

#include "eqinternal.h"

using namespace omega;
using namespace co::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
ConfigImpl::ConfigImpl( co::base::RefPtr< eq::Server > parent): 
	eq::Config(parent) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ConfigImpl::init(const uint128_t& initID)
{
	registerObject(&myFrameData);
	return eq::Config::init(initID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ConfigImpl::exit()
{
	deregisterObject( &myFrameData );
	const bool ret = eq::Config::exit();
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ConfigImpl::handleEvent(const eq::ConfigEvent* event)
{
	static int x;
	static int y;

	switch( event->data.type )
	{
#ifdef OMEGA_USE_KEYBOARD
            
        case eq::Event::KEY_PRESS:
        {
            KeyboardService::keyboardButtonCallback( event->data.key.key , Event::Down);
            return true;   
        }
        case eq::Event::KEY_RELEASE:
        {
            KeyboardService::keyboardButtonCallback( event->data.key.key , Event::Up);
			return true;   
        }
#endif //OMEGA_USE_KEYBOARD

#ifdef OMEGA_USE_MOUSE
	case eq::Event::WINDOW_POINTER_MOTION:
		{
			eq::Window* w = this->find<eq::Window>(event->data.originator);
			const eq::fabric::PixelViewport& pvp = w->getPixelViewport();

			x = event->data.pointerMotion.x + pvp.x;
			y = event->data.pointerMotion.y + pvp.y;
			MouseService::mouseMotionCallback(x, y);
			return true;
		}
	case eq::Event::WINDOW_POINTER_BUTTON_PRESS:
		{
			eq::Window* w = this->find<eq::Window>(event->data.originator);
			const eq::fabric::PixelViewport& pvp = w->getPixelViewport();

			x = event->data.pointerButtonPress.x + pvp.x;
			y = event->data.pointerButtonPress.y + pvp.y;
			uint buttons = 0;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON1) == eq::PTR_BUTTON1) buttons |= Event::Left;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON2) == eq::PTR_BUTTON2) buttons |= Event::Middle;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON3) == eq::PTR_BUTTON3) buttons |= Event::Right;
			MouseService::mouseButtonCallback(buttons, 1, x, y);
			return true;
		}
	case eq::Event::WINDOW_POINTER_BUTTON_RELEASE:
		{
			eq::Window* w = this->find<eq::Window>(event->data.originator);
			const eq::fabric::PixelViewport& pvp = w->getPixelViewport();

			x = event->data.pointerButtonPress.x + pvp.x;
			y = event->data.pointerButtonPress.y + pvp.y;
			uint buttons = 0;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON1) == eq::PTR_BUTTON1) buttons |= Event::Left;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON2) == eq::PTR_BUTTON2) buttons |= Event::Middle;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON3) == eq::PTR_BUTTON3) buttons |= Event::Right;
			MouseService::mouseButtonCallback(buttons, 0, x, y);
			return true;
		}
#else
	case eq::Event::WINDOW_POINTER_WHEEL:
		{
			//x = event->data.pointerWheel.x;
			//y = event->data.pointerWheel.y;
			int wheel = event->data.pointerWheel.xAxis;
			uint buttons = 0;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON1) == eq::PTR_BUTTON1) buttons |= Event::Left;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON2) == eq::PTR_BUTTON2) buttons |= Event::Middle;
			if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON3) == eq::PTR_BUTTON3) buttons |= Event::Right;
			MouseService::mouseWheelCallback(wheel, x, y);
			return true;
		}
#endif
	}
	return Config::handleEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t ConfigImpl::startFrame( const uint128_t& version )
{
	myFrameData.commit();
	return eq::Config::startFrame( version );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t ConfigImpl::finishFrame()
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	ServiceManager* im = SystemManager::instance()->getServiceManager();
	im->poll();

	// Process events.
	int av = im->getAvailableEvents();
	myFrameData.setNumEvents(av);
	myFrameData.setDirtyEvents();
	if(av != 0)
	{
		im->lockEvents();
		// Dispatch events to application server.
		for( int evtNum = 0; evtNum < av; evtNum++)
		{
			Event* evt = im->getEvent(evtNum);
			//myServer->handleEvent(*evt);
			// Copy events to frame data.
			myFrameData.getEvent(evtNum) = *evt;
		}
		im->unlockEvents();
	}
	im->clearEvents();

	// update observer head matrices.
	for( unsigned int i = 0; i < getObservers().size(); i++) 
	{
		Observer* obs  = ds->getObserver(i);
		eq::fabric::Matrix4f om;
		const AffineTransform3& ht = obs->getHeadTransform();
		om.set(ht.data(), ht.data() + 16 * sizeof(float), false);
		getObservers().at(i)->setHeadMatrix(om);
	}

	// Process exit requests.
	if(SystemManager::instance()->isExitRequested())
	{
		const char* ereason = SystemManager::instance()->getExitReason().c_str();
		if(this->exit())
		{
			ofmsg("Application exit request (reason: %1%) successful", %ereason);
		}
		else
		{
			oferror("Application exit request (reason: %1%) FAILED!", %ereason);
		}
	}
	return eq::Config::finishFrame();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ViewImpl* ConfigImpl::findView(const String& viewName)
{
	eq::Layout* layout = this->getLayouts()[0];
	return static_cast< ViewImpl* >(layout->findView(viewName));
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigImpl::setLayerEnabled(const String& viewName, Layer::Enum layer)
{
	ViewImpl* view  = findView(viewName);
	if(view != NULL)
	{
		view->setLayer(layer);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const FrameData& ConfigImpl::getFrameData() 
{ 
	return myFrameData; 
}

