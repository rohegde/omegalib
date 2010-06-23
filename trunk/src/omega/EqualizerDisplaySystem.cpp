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
#include "Application.h"
#include "Config.h"
#include "EqualizerDisplaySystem.h"
#include "SystemManager.h"
#include "input/MouseService.h"

using namespace omega;
using namespace eq::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned int EqualizerDisplaySystem::Id = OID("EQLZ");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EqualizerChannel: public eq::Channel
{
public:
    EqualizerChannel( eq::Window* parent ) : eq::Channel( parent ) {}

protected:
    virtual void frameDraw( const uint32_t spin );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EqualizerView: public eq::View
{
public:
	/** The changed parts of the view. */
    enum DirtyBits
    {
        DIRTY_LAYER = eq::View::DIRTY_CUSTOM << 0,
    };

public:
	EqualizerView(): eq::View() 
	{
		memset(myEnabledLayers, 0, sizeof(bool) * Application::MaxLayers);
	}

	bool IsLayerEnabled(int layer) 
	{ 
		return myEnabledLayers[layer]; 
	}

	void SetLayerEnabled(int layer, bool enabled) 
	{ 
		myEnabledLayers[layer] = enabled; 
	    setDirty( DIRTY_LAYER );
	}

	virtual void serialize( eq::net::DataOStream& os, const uint64_t dirtyBits )
	{
		eq::View::serialize( os, dirtyBits );
	    if( dirtyBits & DIRTY_LAYER )
		{
			for(int i = 0; i < Application::MaxLayers; i++)
			{
				os << myEnabledLayers[i];
			}
		}
	}

	virtual void deserialize( eq::net::DataIStream& is, const uint64_t dirtyBits )
	{
		eq::View::deserialize( is, dirtyBits );
	    if( dirtyBits & DIRTY_LAYER )
		{
			for(int i = 0; i < Application::MaxLayers; i++)
			{
				is >> myEnabledLayers[i];
			}
		}
	}

private:
	bool myEnabledLayers[Application::MaxLayers];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EqualizerConfig: public eq::Config
{
public:
	EqualizerConfig( eq::base::RefPtr< eq::Server > parent): eq::Config(parent) {}
	virtual bool handleEvent(const eq::ConfigEvent* event)
	{
		static int x;
		static int y;
		switch( event->data.type )
		{
        case eq::Event::POINTER_MOTION:
			{
				x += event->data.pointerMotion.dx;
				y += event->data.pointerMotion.dy;
				MouseService::mouseMotionCallback(x, y);
				return true;
			}
		}
		return false;
	}

	virtual uint32_t finishFrame()
	{
		DisplaySystem* ds = SystemManager::GetInstance()->GetDisplaySystem();

		// Update observer head matrices.
		for( unsigned int i = 0; i < getObservers().size(); i++) 
		{
			Observer* obs  = ds->GetObserver(i);
			getObservers().at(i)->setHeadMatrix(obs->GetHeadMatrix());
		}

		SystemManager::GetInstance()->GetInputManager()->Poll();
		return eq::Config::finishFrame();
	}

	void SetLayerEnabled(int viewId, int layerId, bool enabled)
	{
	    EqualizerView* view  = static_cast< EqualizerView* >( findView(viewId));
		if(view != NULL)
		{
			view->SetLayerEnabled(layerId, enabled);
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EqualizerNodeFactory: public eq::NodeFactory
{
public:
	EqualizerNodeFactory() {}
public:
    virtual eq::Config*  createConfig( eq::ServerPtr parent )
		{ return new EqualizerConfig( parent ); }
    virtual eq::Channel* createChannel(eq::Window* parent)
        { return new EqualizerChannel( parent ); }
    virtual eq::View* createView()
        { return new EqualizerView(); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerChannel::frameDraw( const uint32_t spin )
{
	static DrawContext context;

    EqualizerView* view  = static_cast< EqualizerView* > (const_cast< eq::View* >( getView( )));

    // setup OpenGL State
    eq::Channel::frameDraw( spin );

	static float lt = 0.0f;
	Application* app = SystemManager::GetInstance()->GetApplication();
	if(app)
	{
		// Compute dt.
		float t = (float)((double)clock() / CLOCKS_PER_SEC);
		float dt = t - lt;
		lt = t;

		// Setup POV and other GL parameters to keep consistency with GLUT renderer
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		glTranslatef(0, 0, -2.0f);

		app->Update(dt);

		eq::PixelViewport pvp = getPixelViewport();

		// Setup the context viewport.
		context.viewportX = pvp.x;
		context.viewportY = pvp.y;
		context.viewportWidth = pvp.w;
		context.viewportHeight = pvp.h;

		for(int layer = 0; layer < Application::MaxLayers; layer++)
		{
			if(view->IsLayerEnabled(layer))
			{
				context.layer = layer;
				app->Draw(context);
			}
		}

		if(SystemManager::GetInstance()->IsExitRequested())
		{
			exit(0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::EqualizerDisplaySystem():
	mySys(NULL),
	myConfig(NULL),
	myNodeFactory(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::~EqualizerDisplaySystem()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::Initialize(SystemManager* sys)
{
	mySys = sys;
	std::vector<char*> argv = Config::StringToArgv( mySys->GetApplication()->GetName(), mySys->GetConfig()->GetDisplayConfig());

	myNodeFactory = new EqualizerNodeFactory();

	omsg("\n\n--- Equalizer initialization --------------------------------------------------");
    if( !eq::init( argv.size(), &argv[0], myNodeFactory ))
    {
		oerror("Equalizer init failed");
    }

    bool error  = false;
	myConfig = eq::getConfig( argv.size(), &argv[0] );
	omsg("--- Equalizer initialization [DONE] -------------------------------------------\n\n");

	// Create observers.
	int numObservers = myConfig->getObservers().size();
	for( unsigned int i = 0; i < numObservers; i++) myObservers.push_back(new Observer());
	omsg("Initialized %d observer(s).", numObservers);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::Run()
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
void EqualizerDisplaySystem::Cleanup()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerView* EqualizerDisplaySystem::FindView(const char* viewName)
{
    const eq::CanvasVector& canvases = myConfig->getCanvases();
	eq::Canvas* currentCanvas = canvases.front();

    if(!currentCanvas) return NULL;

    const eq::Layout* layout = currentCanvas->getActiveLayout();
    if(!layout) return NULL;

    const eq::ViewVector& views = layout->getViews();
	EqualizerView* view = NULL;
	for(int i = 0; i < views.size(); i++)
	{
		eq::View* v = views[i];
		if(v->getName() == viewName) 
		{
			view = static_cast < EqualizerView* > (v);
			break;
		}
	}
	return view;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::SetLayerEnabled(int layerNum, const char* viewName, bool enabled) 
{
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::SetLayerEnabled - must be called AFTER EqualizerDisplaySystem::Initialize");
		return;
	}

	EqualizerView* view = FindView(viewName);
	if(view != NULL)
	{
		view->SetLayerEnabled(layerNum, enabled);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EqualizerDisplaySystem::IsLayerEnabled(int layerNum,const char* viewName) 
{ 
	if(!myConfig)
	{
		oerror("EqualizerDisplaySystem::GetLayerEnabled - must be called AFTER EqualizerDisplaySystem::Initialize");
		return false;
	}

	EqualizerView* view = FindView(viewName);
	if(view != NULL)
	{
		return view->IsLayerEnabled(layerNum);
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Observer* EqualizerDisplaySystem::GetObserver(int observerId)
{
	return myObservers[observerId];
}
