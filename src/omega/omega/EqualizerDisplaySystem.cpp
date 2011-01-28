/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/Application.h"
#include "omega/Config.h"
#include "omega/EqualizerDisplaySystem.h"
#include "omega/SystemManager.h"

#include "omega/input/MouseService.h"

// This include is needed to use Layout::findView since equalizer code doesn't use this method, and its 
// template definition isn't compiled in the original equalizer static library.
#include "eq/../../../equalizer/libs/fabric/layout.ipp"

using namespace omega;
using namespace co::base;
using namespace std;

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
//!  Frame-specific data.
//!   The frame-specific data is used as a per-config distributed object and contains mutable, rendering-relevant data. 
//!   Each rendering thread (pipe) keeps its own instance synchronized with the frame currently being rendered. The 
//!   data is managed by the Config, which modifies it directly.
class FrameData : public eq::fabric::Serializable
{
public:
	FrameData()	{ }

	virtual ~FrameData() {};

	int getNumEvents() { return myNumEvents; }
	void setNumEvents(int value) { myNumEvents = value; }
	InputEvent& getEvent(int index) { return myEventBuffer[index]; }

	void setDirtyEvents() { setDirty( DIRTY_EVENTS ); }


protected:
	//! Serialize an instance of this class.
	virtual void serialize( co::DataOStream& os, const uint64_t dirtyBits )
	{
		eq::fabric::Serializable::serialize( os, dirtyBits );
		if( dirtyBits & DIRTY_EVENTS )
		{
			os << myNumEvents;
			for(int i = 0; i < myNumEvents; i++)
			{
				myEventBuffer[i].serialize(os);
			}
		}
	}

	//! Deserialize an instance of this class.
	virtual void deserialize( co::DataIStream& is, const uint64_t dirtyBits )
	{
		eq::fabric::Serializable::deserialize( is, dirtyBits );
		if( dirtyBits & DIRTY_EVENTS )
		{
			is >> myNumEvents;
			for(int i = 0; i < myNumEvents; i++)
			{
				myEventBuffer[i].deserialize(is);
			}
		}
	}

	enum DirtyBits
	{
		DIRTY_EVENTS   = eq::fabric::Serializable::DIRTY_CUSTOM << 1,
	};

private:
	int myNumEvents;
	InputEvent myEventBuffer[InputManager::MaxEvents];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ViewImpl: public eq::View
{
public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class Proxy : public eq::fabric::Serializable
		{
		public:
			Proxy( ViewImpl* view ) : myView( view ) {}

		protected:
			/** The changed parts of the view. */
			enum DirtyBits
			{
				DIRTY_LAYER       = eq::fabric::Serializable::DIRTY_CUSTOM << 0,
			};

			virtual void serialize( co::DataOStream& os, const uint64_t dirtyBits )
			{
				if( dirtyBits & DIRTY_LAYER )
				{
					for(int i = 0; i < Application::MaxLayers; i++)
					{
						os << myView->myEnabledLayers[i];
					}
				}
			}

			virtual void deserialize( co::DataIStream& is, const uint64_t dirtyBits )
			{
				if( dirtyBits & DIRTY_LAYER )
				{
					for(int i = 0; i < Application::MaxLayers; i++)
					{
						is >> myView->myEnabledLayers[i];
					}
				}
			}

			virtual void notifyNewVersion() { sync(); }

		private:
			ViewImpl* myView;
			friend class ViewImpl;
		};

public:

	ViewImpl(eq::Layout* parent): 
	  eq::View(parent)
#pragma warning( push )
#pragma warning( disable : 4355 )
	, myProxy( this )
#pragma warning( pop )
	{
		memset(myEnabledLayers, 0, sizeof(bool) * Application::MaxLayers);
		setUserData(&myProxy);
	}

	~ViewImpl()
	{
		this->setUserData(NULL);
	}

	bool isLayerEnabled(int layer) 
	{ 
		return myEnabledLayers[layer]; 
	}

	void setLayerEnabled(int layer, bool enabled) 
	{ 
		myEnabledLayers[layer] = enabled; 
		myProxy.setDirty( Proxy::DIRTY_LAYER );
	}

private:
	bool myEnabledLayers[Application::MaxLayers];
	Proxy myProxy;
	friend class myProxy;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
//! A Window represents an on-screen or off-screen drawable. A drawable is a 2D rendering surface, 
//! typically attached to an OpenGL context. A Window is a child of a Pipe. The task methods for all windows 
//! of a pipe are executed in the same pipe thread. The default window initialization methods initialize all windows 
//! of the same pipe with a shared context, so that OpenGL objects can be reused between them for optimal GPU memory usage.
class WindowImpl: public eq::Window
{
public:
	WindowImpl(eq::Pipe* parent) : eq::Window(parent) {}
	virtual ~WindowImpl() {}

protected:

	virtual bool configInit(const uint128_t& initID)
	{
		return Window::configInit(initID);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ConfigImpl: public eq::Config
{
public:
	ConfigImpl( co::base::RefPtr< eq::Server > parent): eq::Config(parent), myServer(NULL) {}

	virtual bool init(const uint128_t& initID)
	{
		Application* app = SystemManager::instance()->getApplication();

		myServer = app->createServer();
		myServer->initialize();

		registerObject(&myFrameData);

		return eq::Config::init(initID);
	}

	virtual bool exit()
	{
		deregisterObject( &myFrameData );
		const bool ret = eq::Config::exit();
		return ret;
	}

	virtual bool handleEvent(const eq::ConfigEvent* event)
	{
		static int x;
		static int y;
		switch( event->data.type )
		{
		case eq::Event::WINDOW_POINTER_MOTION:
			{
				x = event->data.pointerMotion.x;
				y = event->data.pointerMotion.y;
				MouseService::mouseMotionCallback(x, y);
				return true;
			}
		case eq::Event::WINDOW_POINTER_BUTTON_PRESS:
			{
				x = event->data.pointerButtonPress.x;
				y = event->data.pointerButtonPress.y;
				MouseService::mouseButtonCallback(0, 1, x, y);
				return true;
			}
		case eq::Event::WINDOW_POINTER_BUTTON_RELEASE:
			{
				x = event->data.pointerButtonPress.x;
				y = event->data.pointerButtonPress.y;
				MouseService::mouseButtonCallback(0, 0, x, y);
				return true;
			}
		}
		return false;
	}

	virtual uint32_t startFrame( const uint128_t& version )
	{
		myFrameData.commit();
		return eq::Config::startFrame( version );
	}

	virtual uint32_t finishFrame()
	{
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		InputManager* im = SystemManager::instance()->getInputManager();

		// update observer head matrices.
		for( unsigned int i = 0; i < getObservers().size(); i++) 
		{
			Observer* obs  = ds->getObserver(i);
			getObservers().at(i)->setHeadMatrix(obs->getHeadMatrix());
			getObservers().at(i)->setEyeBase(0.008f);
		}

		// Process events.
		int av = im->getAvailableEvents();
		myFrameData.setNumEvents(av);
		myFrameData.setDirtyEvents();
		if(av != 0)
		{
			InputEvent evts[InputManager::MaxEvents];
			im->getEvents(evts, InputManager::MaxEvents);

			// Dispatch events to application server.
			for( int evtNum = 0; evtNum < av; evtNum++)
			{
				myServer->handleEvent(evts[evtNum]);
				// Copy events to frame data.
				myFrameData.getEvent(evtNum) = evts[evtNum];
			}
		}

		static float lt = 0.0f;
		// Compute dt.
		float t = (float)((double)clock() / CLOCKS_PER_SEC);
		lt = t;
		UpdateContext uc;
		uc.dt = t - lt;

		myServer->update(uc);

		im->poll();

		// Process exit requests.
		if(SystemManager::instance()->isExitRequested())
		{
			const char* ereason = SystemManager::instance()->getExitReason().c_str();
			if(this->exit())
			{
				omsg("Application exit request (reason: %s) successful", ereason);
			}
			else
			{
				oerror("Application exit request (reason: %s) FAILED!", ereason);
			}
		}

		return eq::Config::finishFrame();
	}

	ViewImpl* findView(const String& viewName)
	{
		eq::Layout* layout = this->getLayouts()[0];
		return static_cast< ViewImpl* >(layout->findView(viewName));
	}
	
	void setLayerEnabled(const String& viewName, int layerId, bool enabled)
	{
		ViewImpl* view  = findView(viewName);
		if(view != NULL)
		{
			view->setLayerEnabled(layerId, enabled);
		}
	}

	const FrameData& getFrameData() { return myFrameData; }

private:
	ApplicationServer* myServer;
	FrameData myFrameData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class PipeImpl: public eq::Pipe
{
public:
	PipeImpl(eq::Node* parent): eq::Pipe(parent), myClient(NULL), myInitialized(false) {}

	ApplicationClient* getClient() { return myClient; }

protected:
	virtual ~PipeImpl() {}

	virtual bool configInit( const uint128_t& initID )
	{
		bool result = eq::Pipe::configInit(initID);

		// Map the frame data object.
		ConfigImpl* config = static_cast<ConfigImpl*>( getConfig( ));
		const bool mapped = config->mapObject( &myFrameData, config->getFrameData().getID() );
		EQASSERT( mapped );

		// Create and initialize an application client.
		Application* app = SystemManager::instance()->getApplication();
		if(app)
		{
			myClient = app->createClient();
			myClient->setup();
		}
		return result;
	}

	virtual bool configExit()
	{
		eq::Config* config = getConfig();
		config->unmapObject( &myFrameData );

		return eq::Pipe::configExit();
	}

	virtual void frameStart( const uint128_t& frameID, const uint32_t frameNumber )
	{
		eq::Pipe::frameStart(frameID, frameNumber);

		const GLEWContext* glewc = getWindows()[0]->glewGetContext();

		glewSetContext(glewc);
		// Initialize the client at the first frame.
		if(!myInitialized)
		{
			myClient->initialize();
			myInitialized = true;
		}
		else
		{
			// Syncronize frame data (containing input events and possibly other stuff)
			myFrameData.sync(frameID);

			// Dispatch received events events to application client.
			int av = myFrameData.getNumEvents();
			if(av != 0)
			{
				for( int evtNum = 0; evtNum < av; evtNum++)
				{
					InputEvent& evt = myFrameData.getEvent(evtNum);
					myClient->handleEvent(evt);
				}
			}

			UpdateContext context;
			context.frameNum = frameNumber;
			myClient->update(context);
		}
	}

private:
	bool myInitialized;
	ApplicationClient* myClient;
	FrameData myFrameData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ChannelImpl: public eq::Channel, IGLContextManager
{
public:
	ChannelImpl( eq::Window* parent ) : eq::Channel( parent ), myWindow(parent) 
	{
	}

	virtual ~ChannelImpl() {}

protected:

	virtual void makeCurrent() 
	{
		myWindow->makeCurrent(false);
		glewSetContext(this->glewGetContext());
	}

	virtual void frameDraw( const uint128_t& spin )
	{
		ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
		
		makeCurrent();

		// setup OpenGL State
		eq::Channel::frameDraw( spin );

		PipeImpl* pipe = (PipeImpl*)getPipe();

		eq::PixelViewport pvp = getPixelViewport();

		// setup the context viewport.
		// (spin is 128 bits, gets truncated to 64... do we really need 128 bits anyways!?)
		context.frameNum = spin.low();
		context.viewportX = pvp.x;
		context.viewportY = pvp.y;
		context.viewportWidth = pvp.w;
		context.viewportHeight = pvp.h;
		context.glContext = this;

		//printf("%f\n", myWindow->getFPS());

		for(int i = 0; i < Application::MaxLayers; i++)
		{
			if(view->isLayerEnabled(i))
			{
				context.layer = i;
				pipe->getClient()->draw(context);
			}
		}
	}

private:
	DrawContext context;
	eq::Window* myWindow;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class EqualizerNodeFactory: public eq::NodeFactory
{
public:
	EqualizerNodeFactory() {}
public:
	virtual eq::Config*  createConfig( eq::ServerPtr parent )
		{ return new ConfigImpl( parent ); }
	virtual eq::Channel* createChannel(eq::Window* parent)
		{ return new ChannelImpl( parent ); }
	virtual eq::View* createView(eq::Layout* parent)
		{ return new ViewImpl(parent); }
	virtual eq::Window* createWindow(eq::Pipe* parent)
		{ return new WindowImpl(parent); }
	virtual eq::Pipe* createPipe(eq::Node* parent)
		{ return new PipeImpl(parent); }
};

}; // namespace omega

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::EqualizerDisplaySystem():
	mySys(NULL),
	myConfig(NULL),
	myNodeFactory(NULL),
	mySetting(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::~EqualizerDisplaySystem()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setup(Setting& setting) 
{
	setting.lookupValue("DisplayConfig", myDisplayConfig);
	mySetting = &setting;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initialize(SystemManager* sys)
{
	// Init glew
	glewInit();

	mySys = sys;
	std::vector<char*> argv = Config::stringToArgv( mySys->getApplication()->getName(), myDisplayConfig);

	myNodeFactory = new EqualizerNodeFactory();

	omsg("\n\n--- Equalizer initialization --------------------------------------------------");
	if( !eq::init( argv.size(), &argv[0], myNodeFactory ))
	{
		oerror("Equalizer init failed");
	}

	bool error  = false;
	myConfig = static_cast<ConfigImpl*>(eq::getConfig( argv.size(), &argv[0] ));
	omsg("--- Equalizer initialization [DONE] -------------------------------------------\n\n");

	// Create observers.
	int numObservers = myConfig->getObservers().size();
	for( unsigned int i = 0; i < numObservers; i++) myObservers.push_back(new Observer());
	omsg("initialized %d observer(s).", numObservers);

	// Initialize layers and observers from configuration settings.
	initLayers();

	initObservers();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initObservers()
{
	if(mySetting->exists("Observers"))
	{
		Setting& stObservers = (*mySetting)["Observers"];
		for(int i = 0; i < stObservers.getLength(); i++)
		{
			Setting& stObserver = stObservers[i];
			Observer* obs = getObserver(i);

			Matrix4f emitterMatrix( eq::Matrix4f::IDENTITY );

			if(stObserver.exists("EmitterScale"))
			{
				Setting& stEmitterScale = stObserver["EmitterScale"];
				emitterMatrix.scale((float)stEmitterScale[0], (float)stEmitterScale[1], (float)stEmitterScale[2]);
			}
			if(stObserver.exists("EmitterTranslation"))
			{
				Setting& stEmitterTranslation = stObserver["EmitterTranslation"];
				Vector3f pos;
				pos.x() = (float)stEmitterTranslation[0];
				pos.y() = (float)stEmitterTranslation[1];
				pos.z() = (float)stEmitterTranslation[2];
				emitterMatrix.set_translation(pos);
			}
			/*if(stObserver.exists("EmitterRotation"))
			{
				Setting& stEmitterRotation = stObserver["EmitterRotation"];
				emitterMatrix.rotate((float)stEmitterRotation[0], 1, 0, 0);
				emitterMatrix.rotate((float)stEmitterRotation[1], 0, 1, 0);
				emitterMatrix.rotate((float)stEmitterRotation[2], 0, 0, 1);
			}*/

			// Set observer initial position to origin, neutral orientation.
			obs->setWorldToEmitter(emitterMatrix);
			obs->update(Vector3f::ZERO, Vector3f::ZERO);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initLayers()
{
	if(mySetting->exists("Views"))
	{
		Setting& stViews = (*mySetting)["Views"];
		for(int i = 0; i < stViews.getLength(); i++)
		{
			Setting& stView = stViews[i];
			Setting& stLayers = stView["Layers"];
			for(int j = 0; j < stLayers.getLength(); j++)
			{
				setLayerEnabled(stLayers[j], stView.getName(), true);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::run()
{
	bool error = false;
	if( myConfig )
	{
		omsg("\n\n--- Equalizer display system startup ------------------------------------------");
		if( myConfig->init( 0 ))
		{
			omsg("--- Equalizer display system startup [DONE] -----------------------------------\n\n");

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::cleanup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::setLayerEnabled(int layerNum, const char* viewName, bool enabled) 
{
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::setLayerEnabled - must be called AFTER EqualizerDisplaySystem::initialize");
		return;
	}

	ViewImpl* view = myConfig->findView(viewName);
	if(view != NULL)
	{
		view->setLayerEnabled(layerNum, enabled);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EqualizerDisplaySystem::isLayerEnabled(int layerNum,const char* viewName) 
{ 
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::GetLayerEnabled - must be called AFTER EqualizerDisplaySystem::initialize");
		return false;
	}

	ViewImpl* view = myConfig->findView(viewName);
	if(view != NULL)
	{
		return view->isLayerEnabled(layerNum);
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Observer* EqualizerDisplaySystem::getObserver(int observerId)
{
	oassert(myObservers.size() > observerId);
	return myObservers[observerId];
}

