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
#include "SystemManager.h"
#include "Config.h"
#include "EqualizerDisplaySystem.h"

using namespace omega;
using namespace eq::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned int EqualizerDisplaySystem::Id = OID("EQLZ");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Channel : public eq::Channel
{
public:
    Channel( eq::Window* parent ) : eq::Channel( parent ) {}

protected:
    virtual void frameDraw( const uint32_t spin );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NodeFactory : public eq::NodeFactory
{
public:
    virtual eq::Channel* createChannel( eq::Window* parent )
        { return new Channel( parent ); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Channel::frameDraw( const uint32_t spin )
{
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
		app->Draw(dt);

		if(SystemManager::GetInstance()->IsExitRequested())
		{
			exit(0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqualizerDisplaySystem::EqualizerDisplaySystem():
	mySys(NULL)
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::Run()
{
    NodeFactory nodeFactory;

	std::vector<char*> argv = Config::StringToArgv( mySys->GetApplication()->GetName(), mySys->GetConfig()->GetDisplayConfig());

    if( !eq::init( argv.size(), &argv[0], &nodeFactory ))
    {
		Log::Error("Equalizer init failed");
    }

    bool error  = false;
	eq::Config* config = eq::getConfig( argv.size(), &argv[0] );
    if( config )
    {
        if( config->init( 0 ))
        {
            uint32_t spin = 0;
            while( config->isRunning( ))
            {
                config->startFrame( ++spin );
                config->finishFrame();
            }
        
            // 5. exit config
            config->exit();
        }
        else
        {
            Log::Error("Config initialization failed: %s", config->getErrorMessage());
            error = true;
        }

        eq::releaseConfig( config );
    }
    else
    {
        Log::Error("Cannot get config");
        error = true;
    }    

    eq::exit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EqualizerDisplaySystem::Cleanup()
{
}
