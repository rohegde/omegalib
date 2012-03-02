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
#include "omega/Observer.h"

#include "eqinternal.h"

using namespace omega;
using namespace co::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////
void EventUtils::serializeEvent(Event& evt, co::DataOStream& os)
{
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
void EventUtils::deserializeEvent(Event& evt, co::DataIStream& is)
{
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
    ofmsg("[EQ] ConfigImpl::ConfigImpl");
	SharedDataServices::setSharedData(&mySharedData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ConfigImpl::init()
{
    omsg("[EQ] ConfigImpl::init");

	registerObject(&mySharedData);
	//mySharedData.setAutoObsolete(getLatency());

	return eq::Config::init(mySharedData.getID());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigImpl::mapSharedData(const uint128_t& initID)
{
    omsg("[EQ] ConfigImpl::mapSharedData");
    if(!mySharedData.isAttached( ))
    {
        if(!mapObject( &mySharedData, initID))
		{
			oferror("ConfigImpl::mapSharedData: maoPobject failed (object id = %1%)", %initID);
		}
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ConfigImpl::exit()
{
    //deregisterObject( &myFrameData );
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
bool ConfigImpl::handleEvent(const eq::ConfigEvent* event)
{
    switch( event->data.type )
    {
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
    case eq::Event::WINDOW_POINTER_MOTION:
        {
            MouseService::mouseMotionCallback(event->data.pointer.x, event->data.pointer.y);
            return true;
        }
    case eq::Event::WINDOW_POINTER_BUTTON_PRESS:
        {
            uint buttons = processMouseButtons(event->data.pointerButtonPress.buttons);
            MouseService::mouseButtonCallback(buttons, 1, event->data.pointer.x, event->data.pointer.y);
            return true;
        }
    case eq::Event::WINDOW_POINTER_BUTTON_RELEASE:
        {
            uint buttons = processMouseButtons(event->data.pointerButtonPress.buttons);
            MouseService::mouseButtonCallback(buttons, 0, event->data.pointer.x, event->data.pointer.y);
            return true;
        }
    case eq::Event::WINDOW_POINTER_WHEEL:
        {
            int wheel = event->data.pointerWheel.xAxis;
            uint buttons = processMouseButtons(event->data.pointerButtonPress.buttons);
            MouseService::mouseWheelCallback(buttons, wheel, event->data.pointer.x, event->data.pointer.y);
            return true;
        }
    }
    return Config::handleEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t ConfigImpl::startFrame( const uint128_t& version )
{
    return eq::Config::startFrame( version );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigImpl::updateSharedData( )
{
	if(mySharedData.isMaster())
	{
		mySharedData.commit();
	}
	else
	{
		mySharedData.sync();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t ConfigImpl::finishFrame()
{
    DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();

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
            ofmsg("ApplicationBase exit request (reason: %1%) successful", %ereason);
        }
        else
        {
            oferror("ApplicationBase exit request (reason: %1%) FAILED!", %ereason);
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
//const FrameData& ConfigImpl::getFrameData() 
//{ 
//    return myFrameData; 
//}

