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
#include "omega/Config.h"
#include "omega/Event.h"
#include "omega/EqualizerDisplaySystem.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"
#include "omega/MouseService.h"
#include "omega/StringUtils.h"
#include "omega/GpuManager.h"

#include "eqinternal/eqinternal.h"

using namespace omega;
using namespace co::base;
using namespace std;

extern omega::Vector2i sCanvasSize;
extern omega::Vector2i sCanvasChannels;
extern ChannelImpl* sCanvasChannelPointers[ConfigImpl::MaxCanvasChannels][ConfigImpl::MaxCanvasChannels];

///////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::EqualizerDisplaySystem():
	mySys(NULL),
	myConfig(NULL),
	myNodeFactory(NULL),
	mySetting(NULL),
	myDebugMouse(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::~EqualizerDisplaySystem()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setup(Setting& setting) 
{
	setting.lookupValue("config", myDisplayConfig);
	setting.lookupValue("debugMouse", myDebugMouse);
	mySetting = &setting;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initialize(SystemManager* sys)
{
	Log::level = LOG_WARN;

	mySys = sys;
	const char* appName = mySys->getApplication()->getName();

	DataManager* dm = mySys->getDataManager();
	DataInfo cfgInfo = dm->getInfo(myDisplayConfig);

	oassert(!cfgInfo.isNull())
	oassert(cfgInfo.local);

	const char* argv[] =
	{
		appName,
		"--eq-config",
		cfgInfo.path.c_str()
	};

	myNodeFactory = onew(EqualizerNodeFactory)();

	omsg(":: Equalizer Initialization ::");
	if( !eq::init( 3, (char**)argv, myNodeFactory ))
	{
		oerror("Equalizer init failed");
	}

	bool error  = false;
	myConfig = static_cast<ConfigImpl*>(eq::getConfig( 3, (char**)argv ));
	omsg(":: Equalizer initialization DONE ::");

	// Create observers.
	int numObservers = myConfig->getObservers().size();
	for( unsigned int i = 0; i < numObservers; i++) myObservers.push_back(new Observer());
	ofmsg("initialized %1% observer(s).", %numObservers);

	// Initialize layers and observers from configuration settings.
	initLayers();

	initObservers();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initObservers()
{
	if(mySetting->exists("observers"))
	{
		Setting& stObservers = (*mySetting)["observers"];
		for(int i = 0; i < stObservers.getLength(); i++)
		{
			Setting& stObserver = stObservers[i];
			Observer* obs = getObserver(i);
			obs->load(stObserver);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initLayers()
{
	if(mySetting->exists("views"))
	{
		Setting& stViews = (*mySetting)["views"];
		for(int i = 0; i < stViews.getLength(); i++)
		{
			Setting& stView = stViews[i];

			// Set draw statistics flag
			ViewImpl* view = myConfig->findView(stView.getName());
			if(view != NULL)
			{
				if(stView.exists("drawStatistics"))
					view->drawStatistics(stView["drawStatistics"]);
				if(stView.exists("drawFps"))
					view->drawFps(stView["drawFps"]);
			}

			// Set enabled layers
			if(stView.exists("layer"))
			{
				String layer = stView["layer"];
				setLayer(stView.getName(), Layer::fromString(layer));
			}
			else
			{
				ofwarn("Config: no layer section defined in config/views/%1%", %stView.getName());
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::run()
{
	bool error = false;
	if( myConfig )
	{
		omsg(":: Equalizer display system startup ::");
		if( myConfig->init( 0 ))
		{
			omsg(":: Equalizer display system startup DONE ::");

			uint32_t spin = 0;
			while( myConfig->isRunning( ))
			{
				myConfig->startFrame( spin );
				myConfig->finishFrame();
				spin++;
			}
		
			// 5. exit config
			myConfig->exit();
		}
		else
		{
			oerror("Config initialization failed!");
			error = true;
		}

		eq::releaseConfig( myConfig );
	}
	else
	{
		oerror("Cannot get config");
		error = true;
	}    

	eq::exit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::cleanup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setLayer(const char* viewName, Layer::Enum layer) 
{
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::setLayerEnabled - must be called AFTER EqualizerDisplaySystem::initialize");
		return;
	}

	ViewImpl* view = myConfig->findView(viewName);
	if(view != NULL)
	{
		view->setLayer(layer);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Layer::Enum EqualizerDisplaySystem::getLayer(const char* viewName) 
{ 
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::GetLayerEnabled - must be called AFTER EqualizerDisplaySystem::initialize");
		return Layer::Null;
	}

	ViewImpl* view = myConfig->findView(viewName);
	if(view != NULL)
	{
		return view->getLayer();
	}
	return Layer::Null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Observer* EqualizerDisplaySystem::getObserver(int observerId)
{
	oassert(myObservers.size() > observerId);
	return myObservers[observerId];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector2i EqualizerDisplaySystem::getCanvasSize()
{
	return sCanvasSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ray EqualizerDisplaySystem::getViewRay(Vector2i position)
{
	int channelWidth = (sCanvasSize[0] / sCanvasChannels[0]);
	int channelHeight = (sCanvasSize[1] / sCanvasChannels[1]);

	int channelX = position[0] / channelWidth;
	int channelY = position[1] / channelHeight;

	ChannelImpl* ch = sCanvasChannelPointers[channelX][channelY];
	if(ch != NULL)
	{
		const DrawContext& dc = ch->getLastDrawContext();
		int x = position[0] % channelWidth;
		int y = position[1] % channelHeight;
	ofmsg("Channel X,Y: %1% %2%", %x %y);
		return Math::unproject(Vector2f(x, y), dc.modelview, dc.projection, dc.viewport, 1.0);
	}
	return Ray();
}
