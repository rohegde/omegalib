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

extern omega::Vector2i sCanvasSize;
extern omega::Vector2i sCanvasChannels;
extern ChannelImpl* sCanvasChannelPointers[ConfigImpl::MaxCanvasChannels][ConfigImpl::MaxCanvasChannels];

///////////////////////////////////////////////////////////////////////////////////////////////
void EqUtils::serializeEvent(Event& evt, co::DataOStream& os)
{
	os << evt.myProcessed;
	os << evt.myTimestamp;
	os << evt.mySourceId;
	os << evt.myServiceId;
	os << evt.myServiceType;
	os << evt.myType;
	os << evt.myFlags;
	os << evt.myPosition[0] << evt.myPosition[1] << evt.myPosition[2];
	os << evt.myOrientation.x() << evt.myOrientation.y() << evt.myOrientation.z() << evt.myOrientation.w();

	// Serialize extra data
	os << evt.myExtraDataType;
	os << evt.myExtraDataLength;
	if(evt.myExtraDataType != Event::ExtraDataNull)
	{
		os << evt.myExtraDataValidMask;
		os.write(evt.myExtraData, evt.getExtraDataSize());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
void EqUtils::deserializeEvent(Event& evt, co::DataIStream& is)
{
	is >> evt.myProcessed;
	is >> evt.myTimestamp;
	is >> evt.mySourceId;
	is >> evt.myServiceId;
	is >> evt.myServiceType;
	is >> evt.myType;
	is >> evt.myFlags;
	is >> evt.myPosition[0] >> evt.myPosition[1] >> evt.myPosition[2];
	is >> evt.myOrientation.x() >> evt.myOrientation.y() >> evt.myOrientation.z() >> evt.myOrientation.w();

	// Deserialize extra data
	is >> evt.myExtraDataType;
	is >> evt.myExtraDataLength;
	if(evt.myExtraDataType != Event::ExtraDataNull)
	{
		is >> evt.myExtraDataValidMask;
		is.read(evt.myExtraData, evt.getExtraDataSize());
	}
	if(evt.myExtraDataType == Event::ExtraDataString)
	{
		evt.myExtraData[evt.myExtraDataLength] = '\0';
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ConfigImpl::ConfigImpl( co::base::RefPtr< eq::Server > parent): 
	eq::Config(parent) 
{
}

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
uint ConfigImpl::processMouseButtons(uint btns)
{
	uint buttons = 0;
	if((btns & eq::PTR_BUTTON1) == eq::PTR_BUTTON1) buttons |= Event::Left;
	if((btns & eq::PTR_BUTTON2) == eq::PTR_BUTTON2) buttons |= Event::Middle;
	if((btns & eq::PTR_BUTTON3) == eq::PTR_BUTTON3) buttons |= Event::Right;
	return buttons;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigImpl::processMousePosition(eq::Window* source, int x, int y, Vector2i& outPosition, Ray& ray)
{
	WindowImpl* wi = (WindowImpl*)source;
	if(wi->isInitialized())
	{
		int channelX = wi->getIndex()[0];
		int channelY = wi->getIndex()[1];
		ChannelImpl* sch = sCanvasChannelPointers[channelX][channelY];

		if(sch != NULL)
		{
			Vector2i mousePosition(x, y);

			outPosition = sch->windowToCanvas(mousePosition);
			EqualizerDisplaySystem* eds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();
			if(eds->isDebugMouseEnabled())
			{
				const DrawContext& dc = sch->getLastDrawContext();
				ofmsg("MOUSE  Channel=%1%  ChannelVP=%2%,%3%,%4%,%5%  ChannelPos=%6%,%7%  GlobalPos=%8%", 
					%sch->getName() 
					%dc.viewport.x() %dc.viewport.y() %dc.viewport.width() %dc.viewport.height()
					%x %y
					%outPosition
					);
			}

			ray = eds->getViewRay(mousePosition, channelX, channelY);
		}
	}
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
			Vector2i pos;
			Ray ray;
			processMousePosition(
				this->find<eq::Window>(event->data.originator),
				event->data.pointerMotion.x,
				event->data.pointerMotion.y,
				pos, ray);
			MouseService::mouseMotionCallback(pos[0], pos[1]);
			MouseService::instance()->setPointerRay(ray);
			return true;
		}
	case eq::Event::WINDOW_POINTER_BUTTON_PRESS:
		{
			Vector2i pos;
			Ray ray;
			processMousePosition(
				this->find<eq::Window>(event->data.originator),
				event->data.pointerButtonPress.x,
				event->data.pointerButtonPress.y,
				pos, ray);
			uint buttons = processMouseButtons(event->data.pointerButtonPress.buttons);
			MouseService::mouseButtonCallback(buttons, 1, pos[0], pos[1]);
			MouseService::instance()->setPointerRay(ray);
			return true;
		}
	case eq::Event::WINDOW_POINTER_BUTTON_RELEASE:
		{
			Vector2i pos;
			Ray ray;
			processMousePosition(
				this->find<eq::Window>(event->data.originator),
				event->data.pointerButtonPress.x,
				event->data.pointerButtonPress.y,
				pos, ray);
			uint buttons = processMouseButtons(event->data.pointerButtonPress.buttons);
			MouseService::mouseButtonCallback(buttons, 0, x, y);
			MouseService::instance()->setPointerRay(ray);
			return true;
		}
	case eq::Event::WINDOW_POINTER_WHEEL:
		{
			int wheel = event->data.pointerWheel.xAxis;
			uint buttons = processMouseButtons(event->data.pointerButtonPress.buttons);
			MouseService::mouseWheelCallback(buttons, wheel, 0, 0);
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

