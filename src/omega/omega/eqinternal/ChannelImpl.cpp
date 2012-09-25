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
#include "eqinternal.h"
#include "omega/DisplaySystem.h"

#include <GL/glu.h>

using namespace omega;
using namespace co::base;
using namespace std;

using namespace eq;

//! Comment do disable running of overlay render tasks.
//#define ENABLE_OVERLAY_TASK

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::ChannelImpl( eq::Window* parent ) 
    :eq::Channel( parent ), myWindow(parent), myDrawBuffer(NULL), myStencilInitialized(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::~ChannelImpl() 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ChannelImpl::configInit(const eq::uint128_t& initID)
{
    eq::Channel::configInit(initID);

    EqualizerDisplaySystem* ds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();
    String name = getName();

	if(name == "stats")
	{
		myDC.tile = &ds->getDisplayConfig().statsTile;
	}
	else
	{
		vector<String> args = StringUtils::split(name, "x,");
		int ix = atoi(args[0].c_str());
		int iy = atoi(args[1].c_str());

		myDC.tile = &ds->getDisplayConfig().tiles[ix][iy];
	}

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::setupDrawContext(DrawContext* context, const co::base::uint128_t& spin)
{
    PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
    RendererBase* client = pipe->getClient();
    EqualizerDisplaySystem* ds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();

    eq::PixelViewport pvp = getPixelViewport();
    eq::PixelViewport gpvp = getWindow()->getPixelViewport();

    context->gpuContext = pipe->getGpuContext();

    // setup the context viewport.
    // (spin is 128 bits, gets truncated to 64... do we really need 128 bits anyways!?)
    context->frameNum = spin.low();

    context->viewport = Rect(pvp.x, pvp.y, pvp.w, pvp.h);
        
    switch( getEye() )
    {
        case eq::fabric::EYE_LEFT:
            context->eye = DrawContext::EyeLeft;
            break;
        case eq::fabric::EYE_RIGHT:
            context->eye = DrawContext::EyeRight;
            break;
        case eq::fabric::EYE_CYCLOP:
            context->eye = DrawContext::EyeCyclop;
            break;
    }

    AffineTransform3 mw;
    memcpy(mw.data(), getPerspectiveTransform().begin(), 16 * sizeof(float));
    memcpy(context->projection.data(), getPerspective().compute_matrix().begin(), 16 * sizeof(float));


	Camera* cam = Engine::instance()->getDefaultCamera();

    context->modelview = mw * cam->getViewTransform();

	// Setup the stencil buffer if needed.
	const DisplayConfig& dcfg = ds->getDisplayConfig();
	if(dcfg.interleaved && dcfg.enableStencilInterleaver && !myStencilInitialized)
	{
		setupStencil(myDC.tile->resolution[0], myDC.tile->resolution[1]);
		myStencilInitialized = true;
	}

    // Setup draw buffer
    //if(myDrawBuffer == NULL)
    //{
    //    myDrawBuffer = new RenderTarget(pipe->getGpuContext(), RenderTarget::RenderOnscreen, getDrawable());
    //}
    //context->drawBuffer = myDrawBuffer.get();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameDraw( const co::base::uint128_t& frameID )
{
    eq::Channel::frameDraw( frameID );

	if(getEye() == eq::fabric::EYE_LEFT || getEye() == eq::fabric::EYE_CYCLOP) 
	{
		// This is the first eye being drawn: clear the depth and color buffers.
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		const Color& b = ds->getBackgroundColor();
		glClearColor(b[0], b[1], b[2], b[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	

    //ofmsg("frameDraw: channel %1% frame %2%", %this %frameID);
    PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
    RendererBase* client = pipe->getClient();

    setupDrawContext(&myDC, frameID);

	// Configure stencil test when rendering interleaved with stencil is enabled.
	if(myStencilInitialized)
	{
		if(myDC.eye == DrawContext::EyeLeft)
		{
			glStencilFunc(GL_NOTEQUAL,1,1); // draws if stencil <> 1
		}
		else if(myDC.eye == DrawContext::EyeRight)
		{
			glStencilFunc(GL_EQUAL,1,1); // draws if stencil <> 0
 		}
	}

	// Draw scene
	myDC.task = DrawContext::SceneDrawTask;
	client->draw(myDC);

	// Draw overlay when drawing stereo, otherwise we will do a single overlay drawing pass in
	// frameViewFinish
    if(getEye() != eq::fabric::EYE_CYCLOP)
    {
        myDC.task = DrawContext::OverlayDrawTask;
        getClient()->draw(myDC);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewFinish( const co::base::uint128_t& frameID )
{
    eq::Channel::frameViewFinish( frameID );
    if(getEye() != eq::fabric::EYE_LAST && getEye() != eq::fabric::EYE_CYCLOP) return;

    setupDrawContext(&myDC, frameID);
    myDC.task = DrawContext::OverlayDrawTask;
    myDC.eye = DrawContext::EyeCyclop;

    EQ_GL_CALL( applyBuffer( ));
    EQ_GL_CALL( applyViewport( ));
    EQ_GL_CALL( setupAssemblyState( ));

    getClient()->draw(myDC);

	EqualizerDisplaySystem* ds = (EqualizerDisplaySystem*)getClient()->getDisplaySystem();

	if(myDC.tile->drawStats)
    {
		if(frameID.low() % 10 == 0)
		{
			if(myStatsBuffer == NULL)
			{
				myStatsTexture = new Texture(myDC.gpuContext);
				myStatsTexture->initialize(myDC.tile->resolution[0], myDC.tile->resolution[1]);
				myStatsBuffer = new RenderTarget(myDC.gpuContext, RenderTarget::RenderToTexture);
				myStatsBuffer->setTextureTarget(myStatsTexture);
			}
			myStatsBuffer->bind();
			drawStats();
			myStatsBuffer->unbind();
		}
		//else
		{
			if(myStatsTexture != NULL)
			{
				Renderer* r = (Renderer*)getClient();
				DrawInterface* di = r->getRenderer();
				di->beginDraw2D(myDC);
				di->drawRectTexture(myStatsTexture, omicron::Vector2f::Zero(), omicron::Vector2f(myDC.tile->resolution[0], myDC.tile->resolution[1]), DrawInterface::FlipY);
				di->endDraw();
			}
		}
    }
    else if(myDC.tile->drawFps)
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        applyScreenFrustum();

        glMatrixMode( GL_MODELVIEW );
        glDisable( GL_LIGHTING );

        getWindow()->drawFPS();
    }

    EQ_GL_CALL( resetAssemblyState( ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::RendererBase* ChannelImpl::getClient()
{
    PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
    return pipe->getClient();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::setupStencil(int gliWindowWidth, int gliWindowHeight)
{
	GLint gliStencilBits;
	glGetIntegerv(GL_STENCIL_BITS,&gliStencilBits);

	GLint gliY;
	// seting screen-corresponding geometry
	glViewport(0,0,gliWindowWidth,gliWindowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, gliWindowWidth, 0, gliWindowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	// clearing and configuring stencil drawing
	glDrawBuffer(GL_BACK);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE); // colorbuffer is copied to stencil
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS,1,1); // to avoid interaction with stencil content
	
	// drawing stencil pattern
	glColor4f(1,1,1,0);	// alfa is 0 not to interfere with alpha tests
	for (gliY=0; gliY<gliWindowHeight; gliY+=2)
	{
		glLineWidth(1);
		glBegin(GL_LINES);
			glVertex2f(0, gliY);
			glVertex2f(gliWindowWidth, gliY);
		glEnd();	
	}
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP); // disabling changes in stencil buffer
	glFlush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#define HEIGHT 12
#define SPACE  2
struct EntityData
{
    EntityData() : yPos( 0 ), doubleHeight( false ) {}
    uint32_t yPos;
    bool doubleHeight;
    std::string name;
    std::set< uint32_t > downloaders;
    std::set< uint32_t > compressors;
};

struct IdleData
{
    IdleData() : idle( 0 ), nIdle( 0 ) {}
    uint32_t idle;
    uint32_t nIdle;
    std::string name;
};

static bool _compare( const Statistic& stat1, const Statistic& stat2 )
{ return stat1.type < stat2.type; }

struct StatisticData
{
    const Statistic::Type type;
    const std::string name;
    const Color color;
};

static StatisticData _statisticData[] =
{{ Statistic::NONE,
   "NO EVENT",     Color( 0.f, 0.f, 0.f ) }, 
 { Statistic::CHANNEL_CLEAR,
   "clear",        Color( .5f, 1.0f, .5f ) }, 
 { Statistic::CHANNEL_DRAW,
   "draw",         Color( 0.f, .9f, 0.f ) }, 
 { Statistic::CHANNEL_DRAW_FINISH,
   "finish draw",  Color( 0.f, .5f, 0.f ) }, 
 { Statistic::CHANNEL_ASSEMBLE,
   "assemble",     Color( 1.0f, 1.0f, 0.f ) },
 { Statistic::CHANNEL_FRAME_WAIT_READY,
   "wait frame",   Color( 1.0f, 0.f, 0.f ) }, 
 { Statistic::CHANNEL_READBACK,
   "readback",     Color( 1.0f, .5f, .5f ) }, 
 { Statistic::CHANNEL_VIEW_FINISH,
   "view finish",  Color( 1.f, 0.f, 1.0f ) }, 
 { Statistic::CHANNEL_FRAME_TRANSMIT,
   "transmit",     Color( 0.f, 0.f, 1.0f ) }, 
 { Statistic::CHANNEL_FRAME_COMPRESS,
   "compress",     Color( 0.f, .7f, 1.f ) }, 
 { Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN,
   "wait send token", Color( 1.f, 0.f, 0.f ) }, 
 { Statistic::WINDOW_FINISH,
   "finish",       Color( 1.0f, 1.0f, 0.f ) },
 { Statistic::WINDOW_THROTTLE_FRAMERATE,
   "throttle",     Color( 1.0f, 0.f, 1.f ) }, 
 { Statistic::WINDOW_SWAP_BARRIER,
   "barrier",      Color( 1.0f, 0.f, 0.f ) }, 
 { Statistic::WINDOW_SWAP,
   "swap",         Color( 1.f, 1.f, 1.f ) }, 
 { Statistic::WINDOW_FPS,
   "FPS",          Color( 1.f, 1.f, 1.f ) }, 
 { Statistic::PIPE_IDLE,
   "pipe idle",    Color( 1.f, 1.f, 1.f ) }, 
 { Statistic::NODE_FRAME_DECOMPRESS,
   "decompress",   Color( 0.f, .7f, 1.f ) }, 
 { Statistic::CONFIG_START_FRAME,
   "start frame",  Color( .5f, 1.0f, .5f ) }, 
 { Statistic::CONFIG_FINISH_FRAME,
   "finish frame", Color( .5f, .5f, .5f ) }, 
 { Statistic::CONFIG_WAIT_FINISH_FRAME,
   "wait finish",  Color( 1.0f, 0.f, 0.f ) }, 
 { Statistic::ALL,
   "ALL EVENTS",   Color( 0.0f, 0.f, 0.f ) }} ;


const std::string& StatisticGetName( const Statistic::Type type )
{
    return _statisticData[ type ].name;
}

const Color& StatisticGetColor( const Statistic::Type type )
{
    return _statisticData[ type ].color;
}


void ChannelImpl::drawStats()
{
	EqualizerDisplaySystem* eqds = dynamic_cast<EqualizerDisplaySystem*>(getClient()->getDisplaySystem());

	bool allStats = eqds->isStatEnabled("all");
	bool compact = eqds->isStatEnabled("compact");

    const PixelViewport& pvp = getPixelViewport();
    EQASSERT( pvp.hasArea( ));
    if( !pvp.hasArea( ))
        return;

    eq::Config* config = getConfig();
    EQASSERT( config );

    std::vector< eq::FrameStatistics > statistics;
    config->getStatistics( statistics );

    if( statistics.empty( )) 
        return;

	Renderer* r = (Renderer*)getClient();
	DrawInterface* di = r->getRenderer();

	di->beginDraw2D(myDC);
	di->drawRect(omicron::Vector2f(0, 0), omicron::Vector2f(pvp.w, pvp.h), Color::Black);
	
	Window* window = getWindow();
    //const Window::Font* font = window->getSmallFont();

	omega::Font* fnt = di->getFont("statsFont");
	if(fnt == NULL)
	{
		fnt = di->createFont("statsFont", "fonts/segoeuimod.ttf", 12);
	}


    //----- find min/max time
    int64_t xMax = 0;
    int64_t xMin = std::numeric_limits< int64_t >::max();

    std::map< uint32_t, EntityData > entities;
    std::map< uint32_t, IdleData >   idles;

    for( std::vector< eq::FrameStatistics >::iterator i = statistics.begin();
         i != statistics.end(); ++i )
    {
        eq::FrameStatistics& frameStats  = *i;
        SortedStatistics& configStats = frameStats.second;

        for( SortedStatistics::iterator j = configStats.begin();
             j != configStats.end(); ++j )
        {
            const uint32_t id = j->first;
            std::vector< Statistic >& stats = j->second;
            std::sort( stats.begin(), stats.end(), _compare );

            for( std::vector< Statistic >::const_iterator k = stats.begin(); 
                 k != stats.end(); ++k )
            {
                const Statistic& stat = *k;

                switch( stat.type )
                {
                  case Statistic::PIPE_IDLE:
                  {
                    IdleData& data = idles[ id ];
					data.name = stat.resourceName;
                    data.idle += (stat.idleTime * 100ll / stat.totalTime);
                    ++data.nIdle;
                    continue;
                  }

                  case Statistic::WINDOW_FPS:
                    continue;

                  case Statistic::CHANNEL_FRAME_TRANSMIT:
                  case Statistic::CHANNEL_FRAME_COMPRESS:
                  case Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN:
                    entities[ id ].doubleHeight = true;
                    break;
                  default:
                    break;
                }

                xMax = EQ_MAX( xMax, stat.endTime );
                xMin = EQ_MIN( xMin, stat.startTime );

                if( entities.find( id ) == entities.end( ))
                {
                    EntityData& data = entities[ id ];
                    data.name = stats.front().resourceName;
                }
            }
        }
    }
    const Viewport& vp = getViewport();
    const uint32_t width = uint32_t( pvp.w/vp.w );
    float scale = 0.2f;

    while( (xMax - xMin) / scale > width )
        scale *= 2;

    xMax  /= scale;
    int64_t xStart = xMax - width + SPACE;

    const uint32_t height = uint32_t( pvp.h / vp.h);
    uint32_t nextY = height - SPACE;

    //----- statistics
    float dim = 0.0f;
    for( std::vector< eq::FrameStatistics >::reverse_iterator 
             i = statistics.rbegin(); i != statistics.rend(); ++i )
    {
        const eq::FrameStatistics& frameStats  = *i;
        const SortedStatistics& configStats = frameStats.second;

        int64_t     frameMin = xMax;
        int64_t     frameMax = 0;

        // draw stats
        for( SortedStatistics::const_iterator j = configStats.begin();
             j != configStats.end(); ++j )
        {
            const uint32_t id = j->first;
            const std::vector< Statistic >& stats = j->second;

            if( stats.empty( ))
                continue;

            std::map< uint32_t, EntityData >::iterator l = entities.find( id );
            if( l == entities.end( ))
                continue;

            EntityData& data = l->second;

			if(data.name[0] != '.' && !eqds->isStatEnabled(data.name)) continue;

            if( data.yPos == 0 )
            {
                data.yPos = nextY;
				if(compact) 
				{
					nextY -= (HEIGHT + SPACE) / 6;
					if( data.doubleHeight )
						nextY -= (HEIGHT + SPACE) / 6;
				}
				else
				{
					nextY -= (HEIGHT + SPACE);
					if( data.doubleHeight )
						nextY -= (HEIGHT + SPACE);
				}
            }

            uint32_t y = data.yPos;

            for( std::vector< Statistic >::const_iterator k = stats.begin(); 
                 k != stats.end(); ++k )
            {
                const Statistic& stat = *k;

                switch( stat.type )
                {
                  case Statistic::PIPE_IDLE:
                  case Statistic::WINDOW_FPS:
                    continue;

                  case Statistic::CHANNEL_FRAME_TRANSMIT:
                  case Statistic::CHANNEL_FRAME_COMPRESS:
                  case Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN:
                    y = data.yPos - (HEIGHT + SPACE);
                    break;
                default:
                    break;
                }

                const int64_t startTime = stat.startTime / scale;
                const int64_t endTime   = stat.endTime   / scale;

                frameMin = EQ_MIN( frameMin, startTime );
                frameMax = EQ_MAX( frameMax, endTime   );

                if( endTime < xStart || endTime == startTime )
                    continue;

                float y1 = static_cast< float >( y );
                float y2 = static_cast< float >( y - HEIGHT );
				if(compact) y2 = static_cast< float >( y - HEIGHT / 6);

                const float x1 = static_cast< float >( startTime - xStart );
                const float x2 = static_cast< float >( endTime   - xStart );
                std::stringstream text;
                
                switch( stat.type )
                {
                  case Statistic::CONFIG_WAIT_FINISH_FRAME:
                  case Statistic::CHANNEL_FRAME_WAIT_READY:
                  case Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN:
                    y1 -= SPACE;
                    y2 += SPACE;
                    break;

                  case Statistic::CHANNEL_FRAME_COMPRESS:
                    y1 -= SPACE;
                    y2 += SPACE;
                    text << unsigned( 100.f * stat.ratio ) << '%';
                    if( stat.plugins[ 0 ]  > EQ_COMPRESSOR_NONE )
                        data.compressors.insert( stat.plugins[0] );
                    if( stat.plugins[ 1 ]  > EQ_COMPRESSOR_NONE )
                        data.compressors.insert( stat.plugins[1] );
                    break;

                  /*case Statistic::CHANNEL_READBACK:
                    text << unsigned( 100.f * stat.ratio ) << '%';
                    if( stat.plugins[ 0 ]  > EQ_COMPRESSOR_NONE )
                        data.downloaders.insert( stat.plugins[0] );
                    if( stat.plugins[ 1 ]  > EQ_COMPRESSOR_NONE )
                        data.downloaders.insert( stat.plugins[1] );
                    break;*/

                default:
                    break;
                }
                
                const Color& color( StatisticGetColor( stat.type ) );
				glColor3fv( color.data() );

                glBegin( GL_QUADS );
                    glVertex3f( x2, y1, 0.f );
                    glVertex3f( x1, y1, 0.f );
                    glVertex3f( x1, y2, 0.f);
                    glVertex3f( x2, y2, 0.f );
                glEnd();

                if( !text.str().empty( ))
                {
                    glColor3f( 1.f, 1.f, 1.f );
					di->drawText(text.str(), fnt, omega::Vector2f(x1 + 1, y2), Font::HALeft);
                    //glRasterPos3f( x1+1, y2, 0.f );
                    //font->draw( text.str( ));
                }
            }
        }

        frameMin -= xStart;
        frameMax -= xStart;

        float x = static_cast< float >( frameMin );
        const float y1 = static_cast< float >( nextY );
        const float y2 = static_cast< float >( height );

        glBegin( GL_LINES );
            glColor3f( .5f-dim, 1.0f-dim, .5f-dim );
            glVertex3f( x, y1, 0.3f );
            glVertex3f( x, y2, 0.3f );

            x = static_cast< float >( frameMax );
            glColor3f( .5f-dim, .5f-dim, .5f-dim );
            glVertex3f( x, y1, 0.3f );
            glVertex3f( x, y2, 0.3f );
        glEnd();

        dim += .1f;
    }

    //----- Entitity names
	// no entity names in compact mode
	if(!compact)
	{
		for( std::map< uint32_t, EntityData >::const_iterator i = entities.begin();
			 i != entities.end(); ++i )
		{
			const EntityData& data = i->second;

			glColor3f( 1.f, 1.f, 1.f );
			//glRasterPos3f( 60.f, data.yPos-SPACE-12.0f, 0.99f );
			di->drawText(data.name, fnt, omega::Vector2f(60.f, data.yPos-SPACE-12.0f), Font::HALeft | Font::VATop);
			//font->draw( data.name );

			//std::stringstream downloaders;
			//for( std::set<uint32_t>::const_iterator j = data.downloaders.begin();
			//     j != data.downloaders.end(); ++j )
			//{
			//    downloaders << " 0x" << std::hex << *j << std::dec;
			//}
			//if( !downloaders.str().empty( ))
			//    font->draw( std::string( ", down" ) + downloaders.str( ));

			//std::stringstream compressors;
			//for( std::set<uint32_t>::const_iterator j = data.compressors.begin();
			//     j != data.compressors.end(); ++j )
			//{
			//    compressors << " 0x" << std::hex << *j << std::dec;
			//}
			//if( !compressors.str().empty( ))
			//{
			//    glRasterPos3f( 80.f, data.yPos - HEIGHT - 2*SPACE - 12.0f, 0.99f );
			//    font->draw( std::string( "compressors" ) + compressors.str( ));
			//}
		}
	}

    //----- Global stats (scale, GPU idle)
    glColor3f( 1.f, 1.f, 1.f );
    nextY -= (HEIGHT + SPACE);
    //glRasterPos3f( 60.f, static_cast< float >( nextY ), 0.99f );
    std::ostringstream text;
    text << scale << "ms/pixel";

    if( !idles.empty( ))
        text << ", Idle:";

    for( std::map< uint32_t, IdleData >::const_iterator i = idles.begin();
         i != idles.end(); ++i )
    {
        const IdleData& data = i->second;
        EQASSERT( data.nIdle > 0 );

        text << "   " << data.name << ":" << data.idle / data.nIdle << "%";
    }

	di->drawText(text.str(), fnt, omega::Vector2f(60.f, nextY), Font::HALeft | Font::VATop);
    //font->draw( text.str( ));
    
    //----- Legend
    nextY -= SPACE;
    float x = 0.f;

    //glRasterPos3f( x+1.f, nextY-12.f, 0.f );
    glColor3f( 1.f, 1.f, 1.f );
	di->drawText("channel", fnt, omega::Vector2f(x+1.f, nextY-12.f), Font::HALeft | Font::VATop);

    for( size_t i = 1; i < Statistic::CONFIG_START_FRAME; ++i )
    {
        const Statistic::Type type = static_cast< Statistic::Type >( i );
        if( type == Statistic::CHANNEL_DRAW_FINISH ||
            type == Statistic::PIPE_IDLE || type == Statistic::WINDOW_FPS )
        {
            continue;
        }

        switch( type )
        {
          case Statistic::CHANNEL_FRAME_TRANSMIT:
            x = 0.f;
            nextY -= (HEIGHT + SPACE);

            glColor3f( 1.f, 1.f, 1.f );
            glRasterPos3f( x+1.f, nextY-12.f, 0.f );
            break;

          case Statistic::WINDOW_FINISH:
            x = 0.f;
            nextY -= (HEIGHT + SPACE);

            glColor3f( 1.f, 1.f, 1.f );
            //glRasterPos3f( x+1.f, nextY-12.f, 0.f );
			di->drawText("window", fnt, omega::Vector2f(x+1.f, nextY-12.f), Font::HALeft | Font::VATop);
            break;

          case Statistic::NODE_FRAME_DECOMPRESS:
            x = 0.f;
            nextY -= (HEIGHT + SPACE);

            glColor3f( 1.f, 1.f, 1.f );
            //glRasterPos3f( x+1.f, nextY-12.f, 0.f );
			di->drawText("node", fnt, omega::Vector2f(x+1.f, nextY-12.f), Font::HALeft | Font::VATop);
            //font->draw( "node" );
            break;

          default:
            break;
        }

        x += 60.f;
        const float x2 = x + 60.f - SPACE; 
        const float y1 = static_cast< float >( nextY );
        const float y2 = static_cast< float >( nextY - HEIGHT );

        glColor3fv( StatisticGetColor( type ).data() );
        glBegin( GL_QUADS );
            glVertex3f( x2, y1, 0.f );
            glVertex3f( x,  y1, 0.f );
            glVertex3f( x,  y2, 0.f );
            glVertex3f( x2, y2, 0.f );
        glEnd();

        glColor3f( 0.f, 0.f, 0.f );
        //glRasterPos3f( x+1.f, nextY-12.f, 0.f );
		di->drawText(StatisticGetName( type ), fnt, omega::Vector2f(x+1.f, nextY-10.f), Font::HALeft | Font::VATop);
        //font->draw( Statistic::getName( type ));
    }
    // nextY -= (HEIGHT + SPACE);
    
    glColor3f( 1.f, 1.f, 1.f );
    window->drawFPS();
    
	//EQ_GL_CALL( resetAssemblyState( ));
	
	di->endDraw();
}
