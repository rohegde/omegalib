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
#include "omega/Application.h"
#include "omega/Config.h"
#include "omega/EqualizerDisplaySystem.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"
#include "omega/SystemManager.h"
#include "omega/RenderTarget.h"
#include "omega/MouseService.h"
#include "omega/StringUtils.h"
#include "omega/GpuManager.h"

#ifdef OMEGA_OS_WIN
// This include is needed to use Layout::findView since equalizer code doesn't use this method, and its 
// template definition isn't compiled in the original equalizer static library.
#include "eq/../../../equalizer/libs/fabric/layout.ipp"
#endif

using namespace omega;
using namespace co::base;
using namespace std;

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
//!  Frame-specific data.
//!   The frame-specific data is used as a per-config distributed object and contains mutable, rendering-relevant data. 
//!   Each rendering thread (pipe) keeps its own instance synchronized with the frame currently being rendered. The 
//!   data is managed by the Config, which modifies it directly.
class FrameData : public eq::fabric::Serializable
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	FrameData()	{ }

	virtual ~FrameData() {};

	int getNumEvents() { return myNumEvents; }
	void setNumEvents(int value) { myNumEvents = value; }
	Event& getEvent(int index) { return myEventBuffer[index]; }

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
	Event myEventBuffer[ OMEGA_MAX_EVENTS ];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ViewImpl: public eq::View
{
public:
		///////////////////////////////////////////////////////////////////////////////////////////
		class Proxy : public eq::fabric::Serializable
		{
		public:
			Proxy( ViewImpl* view ) : myView( view ) {}

		protected:
			/** The changed parts of the view. */
			enum DirtyBits
			{
				DIRTY_LAYER       = eq::fabric::Serializable::DIRTY_CUSTOM << 0,
				DIRTY_DRAW_STATS       = eq::fabric::Serializable::DIRTY_CUSTOM << 1,
			};

			virtual void serialize( co::DataOStream& os, const uint64_t dirtyBits )
			{
				if( dirtyBits & DIRTY_LAYER )
				{
					for(int i = 0; i < Application::MaxLayers; i++)
					{
						os << myView->myLayer;
					}
				}
				if( dirtyBits & DIRTY_DRAW_STATS )
				{
					os << myView->myDrawStatistics;
				}
			}

			virtual void deserialize( co::DataIStream& is, const uint64_t dirtyBits )
			{
				if( dirtyBits & DIRTY_LAYER )
				{
					for(int i = 0; i < Application::MaxLayers; i++)
					{
						is >> myView->myLayer;
					}
				}
				if( dirtyBits & DIRTY_DRAW_STATS )
				{
					is >> myView->myDrawStatistics;
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
		myDrawStatistics = false;
		myLayer = Layer::Null;
		setUserData(&myProxy);
	}

	~ViewImpl()
	{
		this->setUserData(NULL);
	}

	Layer::Enum getLayer() 
	{ 
		return myLayer; 
	}

	void setLayer(Layer::Enum layer) 
	{ 
		myLayer = layer; 
		myProxy.setDirty( Proxy::DIRTY_LAYER );
	}

	void drawStatistics(bool enable)
	{
		myDrawStatistics = enable;
		myProxy.setDirty( Proxy::DIRTY_DRAW_STATS );
	}

	bool isDrawStatisticsEnabled()
	{
		return myDrawStatistics;
	}

private:
	bool myDrawStatistics;
	Layer::Enum myLayer;
	Proxy myProxy;
	friend class myProxy;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ConfigImpl: public eq::Config
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
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

#define USE_WINDOW_EVENTS

	virtual bool handleEvent(const eq::ConfigEvent* event)
	{
		static int x;
		static int y;
		switch( event->data.type )
		{
#ifdef OMEGA_USE_MOUSE
	#ifdef USE_WINDOW_EVENTS
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
		case eq::Event::CHANNEL_POINTER_MOTION:
			{
				x = event->data.pointerMotion.x; 
				y = event->data.pointerMotion.y; 
				MouseService::mouseMotionCallback(x, y);
				return true;
			}
		case eq::Event::CHANNEL_POINTER_BUTTON_PRESS:
			{
				x = event->data.pointerButtonPress.x;
				y = event->data.pointerButtonPress.y;
				uint buttons = 0;
				if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON1) == eq::PTR_BUTTON1) buttons |= Event::Left;
				if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON2) == eq::PTR_BUTTON2) buttons |= Event::Middle;
				if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON3) == eq::PTR_BUTTON3) buttons |= Event::Right;
				MouseService::mouseButtonCallback(buttons, 1, x, y);
				return true;
			}
		case eq::Event::CHANNEL_POINTER_BUTTON_RELEASE:
			{
				x = event->data.pointerButtonPress.x;
				y = event->data.pointerButtonPress.y;
				uint buttons = 0;
				if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON1) == eq::PTR_BUTTON1) buttons |= Event::Left;
				if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON2) == eq::PTR_BUTTON2) buttons |= Event::Middle;
				if((event->data.pointerButtonPress.buttons & eq::PTR_BUTTON3) == eq::PTR_BUTTON3) buttons |= Event::Right;
				MouseService::mouseButtonCallback(buttons, 0, x, y);
				return true;
			}
	#endif
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

	virtual uint32_t startFrame( const uint128_t& version )
	{
		myFrameData.commit();
		return eq::Config::startFrame( version );
	}

	virtual uint32_t finishFrame()
	{
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		ServiceManager* im = SystemManager::instance()->getServiceManager();

		im->poll();

		// update observer head matrices.
		for( unsigned int i = 0; i < getObservers().size(); i++) 
		{
			Observer* obs  = ds->getObserver(i);
			eq::fabric::Matrix4f om;
			const AffineTransform3& ht = obs->getHeadTransform();
			om.set(ht.data(), ht.data() + 16 * sizeof(float), false);
			getObservers().at(i)->setHeadMatrix(om);
		}

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
				myServer->handleEvent(*evt);
				// Copy events to frame data.
				myFrameData.getEvent(evtNum) = *evt;
			}
			im->unlockEvents();
		}
		im->clearEvents();

		static float lt = 0.0f;
		// Compute dt.
		float t = (float)((double)clock() / CLOCKS_PER_SEC);
		lt = t;
		UpdateContext uc;
		uc.dt = t - lt;

		myServer->update(uc);

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

	ViewImpl* findView(const String& viewName)
	{
		eq::Layout* layout = this->getLayouts()[0];
		return static_cast< ViewImpl* >(layout->findView(viewName));
	}
	
	void setLayerEnabled(const String& viewName, Layer::Enum layer)
	{
		ViewImpl* view  = findView(viewName);
		if(view != NULL)
		{
			view->setLayer(layer);
		}
	}

	const FrameData& getFrameData() { return myFrameData; }

private:
	ApplicationServer* myServer;
	FrameData myFrameData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class PipeImpl: public eq::Pipe
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	PipeImpl(eq::Node* parent): eq::Pipe(parent), myClient(NULL), myInitialized(false) {}
	ApplicationClient* getClient() { return myClient; }
	FrameData& getFrameData() { return myFrameData; }

protected:
	virtual ~PipeImpl() {}

	virtual bool configInit( const uint128_t& initID )
	{
		bool result = eq::Pipe::configInit(initID);

		// Map the frame data object.
		ConfigImpl* config = static_cast<ConfigImpl*>( getConfig( ));
		const bool mapped = config->mapObject( &myFrameData, config->getFrameData().getID() );
		oassert( mapped );

		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();

		// Create and initialize an application client.
		Application* app = SystemManager::instance()->getApplication();
		if(app)
		{
			myClient = app->createClient();
			const eq::fabric::PixelViewport pw = getPixelViewport();
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

		// Activate the glew context for this pipe, so initialize and update client
		// methods can handle openGL buffers associated with this Pipe.
		// NOTE: getting the glew context from the first window is correct since all
		// windows attached to the same pape share the same Glew (and OpenGL) contexts.
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

			UpdateContext context;
			context.frameNum = frameNumber;

			// Dispatch received events events to application client.
			int av = myFrameData.getNumEvents();
			if(av != 0)
			{
				for( int evtNum = 0; evtNum < av; evtNum++)
				{
					Event& evt = myFrameData.getEvent(evtNum);
					if(!evt.processed)
					{
						evt.processed = myClient->handleEvent(evt, context);
					}
				}
			}
			myClient->update(context);
		}
	}

private:
	bool myInitialized;
	ApplicationClient* myClient;
	FrameData myFrameData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
//! A Window represents an on-screen or off-screen drawable. A drawable is a 2D rendering surface, 
//! typically attached to an OpenGL context. A Window is a child of a Pipe. The task methods for all windows 
//! of a pipe are executed in the same pipe thread. The default window initialization methods initialize all windows 
//! of the same pipe with a shared context, so that OpenGL objects can be reused between them for optimal GPU memory usage.
class WindowImpl: public eq::Window
{
public:
	WindowImpl(eq::Pipe* parent) : eq::Window(parent), myFrameBuffer(NULL) {}
	virtual ~WindowImpl() {}

protected:

	virtual bool configInitGL(const uint128_t& initID)
	{
		// Initialize this window frame buffer.
		bool res = Window::configInitGL(initID);
		const eq::fabric::PixelViewport& pvp = getPixelViewport();
		myFrameBuffer = new RenderTarget();
		myFrameBuffer->initialize(RenderTarget::TypeFrameBuffer, pvp.w, pvp.h);

		// Get the gpu manager from the client instance.
		PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
		ApplicationClient* client = pipe->getClient();
		myGpu = client->getGpu();

		return res;
	}

	virtual void frameStart	(const uint128_t &frameID, const uint32_t frameNumber) 
	{
		// Set the frame buffer for the client gpu to this window frame buffer.
		myGpu->setFrameBuffer(myFrameBuffer);
	}

private:
	RenderTarget* myFrameBuffer;
	GpuManager* myGpu;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
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
		PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
		FrameData& fd = pipe->getFrameData();
		ApplicationClient* client = pipe->getClient();
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();

		//makeCurrent();

		// setup OpenGL State
		eq::Channel::frameDraw( spin );

		eq::PixelViewport pvp = getPixelViewport();
		eq::PixelViewport gpvp = getWindow()->getPixelViewport();

		// setup the context viewport.
		// (spin is 128 bits, gets truncated to 64... do we really need 128 bits anyways!?)
		DrawContext context;
		context.frameNum = spin.low();
		context.glContext = this;

		context.viewport = Rect(pvp.x, pvp.y, pvp.w, pvp.h);
		context.globalViewport = Rect(gpvp.x, gpvp.y, gpvp.w, gpvp.h);

		// Can we get the matrix out of equalizer instead of using opengl?
		glGetFloatv( GL_MODELVIEW_MATRIX, context.modelview.data());
		glGetFloatv( GL_PROJECTION_MATRIX, context.projection.data());

		// Dispatch received events events to application client.
		int av = fd.getNumEvents();
		if(av != 0)
		{
			for( int evtNum = 0; evtNum < av; evtNum++)
			{
				Event& evt = fd.getEvent(evtNum);
				// If event has not been processed during update, handle it now.
				if(!evt.processed)
				{
					// Pointer type events get special treatment: they are delivered
					// only to channels whose pixel viewport contains the event source position.
					// Also, the event poistion is converted from global pixel coordinates to
					// window coordinates.
					if(evt.serviceType == Service::Pointer)
					{
						int vx1 = context.globalViewport.x() + context.viewport.x();
						int vy1 = context.globalViewport.y() + context.viewport.y();
						int vx2 = vx1 + context.viewport.width();
						int vy2 = vy1 + context.viewport.height();

						if(evt.position[0] > vx1 &&
							evt.position[0] < vx2 &&
							evt.position[1] > vy1 &&
							evt.position[1] < vy2)
						{
							evt.position[0] -= context.globalViewport.x();
							evt.position[1] -= context.globalViewport.y();
							evt.processed = client->handleEvent(evt, context);
						}
					}
					else
					{
						evt.processed = client->handleEvent(evt, context);
					}
				}
			}
		}

		context.layer = view->getLayer();
		client->draw(context);
	}

	virtual void frameViewFinish( const uint128_t& spin )
	{
		ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
		if(view->isDrawStatisticsEnabled())
		{
			drawStatistics();
		}
	}

private:
	eq::Window* myWindow;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::EqualizerDisplaySystem():
	mySys(NULL),
	myConfig(NULL),
	myNodeFactory(NULL),
	mySetting(NULL)
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
	mySetting = &setting;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::initialize(SystemManager* sys)
{
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

	myNodeFactory = new EqualizerNodeFactory();

	omsg("\n\n--- Equalizer initialization --------------------------------------------------");
	if( !eq::init( 3, (char**)argv, myNodeFactory ))
	{
		oerror("Equalizer init failed");
	}

	bool error  = false;
	myConfig = static_cast<ConfigImpl*>(eq::getConfig( 3, (char**)argv ));
	omsg("--- Equalizer initialization [DONE] -------------------------------------------\n\n");

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

