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
#include "omega/EventSharingModule.h"

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
    os << evt.myExtraDataItems;
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
    is >> evt.myExtraDataItems;
    if(evt.myExtraDataType != Event::ExtraDataNull)
    {
        is >> evt.myExtraDataValidMask;
        is.read(evt.myExtraData, evt.getExtraDataSize());
    }
    if(evt.myExtraDataType == Event::ExtraDataString)
    {
        evt.myExtraData[evt.getExtraDataSize()] = '\0';
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ConfigImpl::ConfigImpl( co::base::RefPtr< eq::Server > parent): 
    eq::Config(parent) 
{
    omsg("[EQ] ConfigImpl::ConfigImpl");
	SharedDataServices::setSharedData(&mySharedData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ConfigImpl::~ConfigImpl()
{
	if(mySharedData.isAttached())
	{
		if(mySharedData.isMaster())
		{
			deregisterObject(&mySharedData);
		}
		else
		{
			unmapObject(&mySharedData);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ConfigImpl::init()
{
    omsg("[EQ] ConfigImpl::init");

	registerObject(&mySharedData);
	//mySharedData.setAutoObsolete(getLatency());

	SystemManager* sys = SystemManager::instance();
	
	//if(!sys->renderOnMaster())
	{
		ApplicationBase* app = sys->getApplication();
		myServer = new Engine(app);
	
		EqualizerDisplaySystem* eqds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();
		eqds->finishInitialize(this);

		myServer->initialize();
	}

	StatsManager* sm = SystemManager::instance()->getStatsManager();
	myFpsStat = sm->createStat("fps");

	myGlobalTimer.start();


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
	myServer->dispose();
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
			// BEHOLD THE MIGHTY KILL BUTTON:
			// Esc key press always posts an exit request.
			if(event->data.key.key == 256) 	
			{
					SystemManager::instance()->postExitRequest();
			}
			else
			{
				KeyboardService::keyboardButtonCallback( event->data.key.key , Event::Down);
			}
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
	static float lt = 0.0f;
	static float tt = 0.0f;
	// Compute dt.
	float t = (float)myGlobalTimer.getElapsedTimeInSec();
	if(lt == 0) lt = t;
	
	UpdateContext uc;
	uc.dt = t - lt;
	tt += uc.dt;
	uc.time = tt;
	uc.frameNum = version.low();
	lt = t;

	mySharedData.setUpdateContext(uc);

	// Update fps stats every 10 frames.
	if(uc.frameNum % 10 == 0)
	{
		myFpsStat->addSample(1.0f / uc.dt);
	}

	// If enabled, broadcast events to other server nodes.
	if(SystemManager::instance()->isMaster())
	{
		ServiceManager* im = SystemManager::instance()->getServiceManager();
		im->poll();
		int av = im->getAvailableEvents();
		//ofmsg("Events: %1%", %av);
		if(av != 0)
		{
    		im->lockEvents();
    		// Dispatch events to application server.
    		for( int evtNum = 0; evtNum < av; evtNum++)
    		{
    			Event* evt = im->getEvent(evtNum);

				if(!EventSharingModule::isLocal(*evt))
				{
					EventSharingModule::share(*evt);
				}
				myServer->handleEvent(*evt);
    		}
    		im->unlockEvents();
		}
		im->clearEvents();
	}

	// Send shared data.
	mySharedData.commit();

	myServer->update(uc);

    return eq::Config::startFrame( version );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigImpl::updateSharedData( )
{
	if(!mySharedData.isMaster())
	{
		mySharedData.sync(co::VERSION_NEXT);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const UpdateContext& ConfigImpl::getUpdateContext()
{
	return mySharedData.getUpdateContext();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t ConfigImpl::finishFrame()
{
    EqualizerDisplaySystem* ds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();

	if(ds->getDisplayConfig().orientObserverToTile)
	{
		// if orientObserverToTile is enabled, we assume the observer orientation is always normal
		// to the tile. Only the observer position is updated.
		// This works only for ONE actual, tracked observer. So we just get observer 0 from the observer list.
		Camera* cam = Engine::instance()->getDefaultCamera();

		int numObservers = getObservers().size();
		for(int i = 0; i < numObservers; i++)
		{
			eq::Observer* eqo = getObservers().at(i);
			ObserverTileData& otd = myObserverTileData[i];
			// If the observer data has not been initialized yet, do it now.
			if(otd.observer == NULL)
			{
				otd.observer = eqo;
				// Get the tile index from the observer name.
				sscanf(eqo->getName().c_str(), "observer%dx%d", &otd.x, &otd.y);
				const DisplayTileConfig& dtc = ds->getDisplayConfig().tiles[otd.x][otd.y];

				// Compute the tile normal, and compute the orientation that would bring the observer from the default
				// look-at vector (0, 0, -1) to the tile normal.
				//Vector3f tileNormal = Math::calculateBasicFaceNormal(dtc.bottomLeft, dtc.topLeft, dtc.bottomRight);
				//otd.orientation = Math::buildRotation(-Vector3f::UnitZ(), tileNormal, Vector3f::UnitY());

				// CAVE2 SIMPLIFICATION: We are just interested in adjusting the observer yaw
				otd.yaw = dtc.yaw;
			}

			// Update the tile-observer head matrix, using the observer position and the per-tile orientation.
			// CAVE2 SIMPLIFICATION: We are just interested in adjusting the observer yaw
			const Vector3f& pos = cam->getHeadOffset();
			eq::fabric::Matrix4f om = eq::fabric::Matrix4f::IDENTITY;
			//om.rotate_z(Math::Pi);
			om.rotate_y(-otd.yaw * Math::DegToRad);
			om.set_translation(pos[0], pos[1], pos[2]);
			eqo->setHeadMatrix(om);
		}
	}
	else
	{
		// update observer head matrices the normal way
		for( unsigned int i = 0; i < getObservers().size(); i++) 
		{
			Camera* cam = Engine::instance()->getDefaultCamera();
			eq::fabric::Matrix4f om;
			const AffineTransform3& ht = cam->getHeadTransform();
			om.set(ht.data(), ht.data() + 16 * sizeof(float), false);
			getObservers().at(i)->setHeadMatrix(om);
		}
	}
    return eq::Config::finishFrame();
}

