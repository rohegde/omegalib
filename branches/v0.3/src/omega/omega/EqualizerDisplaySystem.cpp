/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * EqualizerDisplaySystem method implementation. See EqualizerDisplaySystem.h for more details.
 *********************************************************************************************************************/
#include "omega/Application.h"
#include "omega/Config.h"
#include "omega/EqualizerDisplaySystem.h"
#include "omega/SystemManager.h"

#include "omega/input/MouseService.h"

using namespace omega;
using namespace eq::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned int EqualizerDisplaySystem::Id = OID("EQLZ");

namespace omega
{

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

	virtual bool configInit(const uint32_t initID)
	{
		return Window::configInit(initID);
	}
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

            virtual void serialize( eq::net::DataOStream& os, const uint64_t dirtyBits )
			{
				if( dirtyBits & DIRTY_LAYER )
				{
					for(int i = 0; i < Application::MaxLayers; i++)
					{
						os << myView->myEnabledLayers[i];
					}
				}
			}

            virtual void deserialize( eq::net::DataIStream& is, const uint64_t dirtyBits )
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
class ConfigImpl: public eq::Config
{
public:
	ConfigImpl( eq::base::RefPtr< eq::Server > parent): eq::Config(parent), myServer(NULL) {}

	virtual bool init(const uint32_t initID)
	{
		Application* app = SystemManager::instance()->getApplication();
		myServer = app->createServer();
		myServer->initialize();
		return eq::Config::init(initID);
	}

	virtual bool handleEvent(const eq::ConfigEvent* event)
	{
		static int x;
		static int y;
		switch( event->data.type )
		{
        case eq::Event::POINTER_MOTION:
			{
				x = event->data.pointerMotion.x;
				y = event->data.pointerMotion.y;
				MouseService::mouseMotionCallback(x, y);
				return true;
			}
		case eq::Event::POINTER_BUTTON_PRESS:
			{
				x = event->data.pointerButtonPress.x;
				y = event->data.pointerButtonPress.y;
				MouseService::mouseButtonCallback(0, 1, x, y);
				return true;
			}
		case eq::Event::POINTER_BUTTON_RELEASE:
			{
				x = event->data.pointerButtonPress.x;
				y = event->data.pointerButtonPress.y;
				MouseService::mouseButtonCallback(0, 0, x, y);
				return true;
			}
		}
		return false;
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
		im->processEvents(myServer);	

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
		eq::Layout* layout = this->ohGetLayout(0);
		return static_cast< ViewImpl* >(layout->ohFindView(viewName));
	}
	
	void setLayerEnabled(const String& viewName, int layerId, bool enabled)
	{
	    ViewImpl* view  = findView(viewName);
		if(view != NULL)
		{
			view->setLayerEnabled(layerId, enabled);
		}
	}

private:
	ApplicationServer* myServer;
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

    virtual bool configInit( const uint32_t initID )
	{
		bool result = eq::Pipe::configInit(initID);
		// Create and initialize an application client.
		Application* app = SystemManager::instance()->getApplication();
		if(app)
		{
			myClient = app->createClient();
			myClient->setup();
		}
		return result;
	}

    virtual void frameStart( const uint32_t frameID, const uint32_t frameNumber )
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

		UpdateContext context;
		context.frameNum = frameNumber;
		myClient->update(context);
	}

private:
	bool myInitialized;
	ApplicationClient* myClient;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ChannelImpl: public eq::Channel
{
public:
    ChannelImpl( eq::Window* parent ) : eq::Channel( parent ), myWindow(parent) {}
	virtual ~ChannelImpl() {}

protected:

	virtual void frameDraw( const uint32_t spin )
	{
		ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
		
		glewSetContext(this->glewGetContext());

		// setup OpenGL State
		eq::Channel::frameDraw( spin );
		
		PipeImpl* pipe = (PipeImpl*)getPipe();

		eq::PixelViewport pvp = getPixelViewport();

		// setup the context viewport.
		context.frameNum = spin;
		context.viewportX = pvp.x;
		context.viewportY = pvp.y;
		context.viewportWidth = pvp.w;
		context.viewportHeight = pvp.h;

		//printf("%f\n", myWindow->getFPS());

		for(int i = 0; i < Application::MaxLayers; i++)
		{
			if(view->isLayerEnabled(i))
			{
				myWindow->makeCurrent(false);
				glewSetContext(this->glewGetContext());
				context.layer = i;
				pipe->getClient()->draw(context);
			}
		}

		myWindow->makeCurrent(false);
		glewSetContext(this->glewGetContext());
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
                myConfig->startFrame( ++spin );
                myConfig->finishFrame();
            }
        
            // 5. exit config
            myConfig->exit();
        }
        else
        {
            oerror("Config initialization failed: %s", myConfig->getErrorMessage());
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

