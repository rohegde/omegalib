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
#include "omega/StringUtils.h"

using namespace omega;
using namespace co::base;
using namespace std;

using namespace eq;

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::ChannelImpl( eq::Window* parent ) 
	:eq::Channel( parent ), myWindow(parent) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::~ChannelImpl() 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::setupDrawContext(DrawContext* context, const co::base::uint128_t& spin)
{
	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();

	eq::PixelViewport pvp = getPixelViewport();
	eq::PixelViewport gpvp = getWindow()->getPixelViewport();

	// setup the context viewport.
	// (spin is 128 bits, gets truncated to 64... do we really need 128 bits anyways!?)
	context->frameNum = spin.low();
	//context.glContext = this;

	context->viewport = Rect(pvp.x, pvp.y, pvp.w, pvp.h);
	context->globalViewport = Rect(gpvp.x, gpvp.y, gpvp.w, gpvp.h);
		
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

	memcpy(context->modelview.data(), getHeadTransform().begin(), 16 * sizeof(float));
	memcpy(context->projection.data(), getFrustum().compute_matrix().begin(), 16 * sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::makeCurrent() 
{
	myWindow->makeCurrent(false);
	glewSetContext(this->glewGetContext());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameDraw( const co::base::uint128_t& spin )
{
	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();

	//String chName = this->getName();
	//ofmsg("%1%", %chName);

	makeCurrent();
	EQ_GL_CALL(applyBuffer());
	EQ_GL_CALL( setupAssemblyState( ));

	// setup OpenGL State
	eq::Channel::frameDraw( spin );

	DrawContext context;
	setupDrawContext(&context, spin);

	context.layer = view->getLayer() & 0x03;
	client->draw(context);

	EQ_GL_CALL( resetAssemblyState( ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewStart( const co::base::uint128_t& spin )
{
	eq::Channel::frameViewStart( spin );

	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	FrameData& fd = pipe->getFrameData();
	ApplicationClient* client = pipe->getClient();

	DrawContext context;
	setupDrawContext(&context, spin);
	context.layer = view->getLayer() & 0x0c;

	// Dispatch received events events to application client.
	int av = fd.getNumEvents();
	if(av != 0)
	{
		for( int evtNum = 0; evtNum < av; evtNum++)
		{
			Event& evt = fd.getEvent(evtNum);
			// If event has not been processed during update, handle it now.
			if(!evt.isProcessed())
			{
				// Pointer type events get special treatment: they are delivered
				// only to channels whose pixel viewport contains the event source position.
				// Also, the event poistion is converted from global pixel coordinates to
				// window coordinates.
				if(evt.getServiceType() == Service::Pointer)
				{
					int vx1 = context.globalViewport.x() + context.viewport.x();
					int vy1 = context.globalViewport.y() + context.viewport.y();
					int vx2 = vx1 + context.viewport.width();
					int vy2 = vy1 + context.viewport.height();
					//ofmsg("pos %1% (%2%-%3% %4%-%5%)", %evt.position %vx1 %vx2 %vy1 %vy2);

					Vector3f position = evt.getPosition();
					if(position[0] > vx1 &&
						position[0] < vx2 &&
						position[1] > vy1 &&
						position[1] < vy2)
					{
						position[0] -= context.globalViewport.x();
						position[1] -= context.globalViewport.y();
						//ofmsg("pos %1%", %position);
						evt.setPosition(position);
						client->handleEvent(evt);
					}
				}
				else
				{
					client->handleEvent(evt);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewFinish( const co::base::uint128_t& spin )
{
	eq::Channel::frameViewFinish( spin );

	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();

	DrawContext context;
	setupDrawContext(&context, spin);
	context.layer = view->getLayer() & 0x0c;

	if(context.layer != 0)
	{
		client->draw(context);
	}
	if(view->isDrawStatisticsEnabled())
	{
		drawStatistics();
	}
	else if(view->isDrawFpsEnabled())
	{
		EQ_GL_CALL( applyBuffer( ));
		EQ_GL_CALL( applyViewport( ));
		EQ_GL_CALL( setupAssemblyState( ));
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		applyScreenFrustum();

		glMatrixMode( GL_MODELVIEW );
		glDisable( GL_LIGHTING );

		getWindow()->drawFPS();
		EQ_GL_CALL( resetAssemblyState( ));
	}
}

//namespace
//{
//#define HEIGHT 12
//#define SPACE  2
//
//struct EntityData
//{
//    EntityData() : yPos( 0 ), doubleHeight( false ) {}
//    uint32_t yPos;
//    bool doubleHeight;
//    std::string name;
//};
//
//struct IdleData
//{
//    IdleData() : idle( 0 ), nIdle( 0 ) {}
//    uint32_t idle;
//    uint32_t nIdle;
//    std::string name;
//};
//
//static bool _compare( const Statistic& stat1, const Statistic& stat2 )
//{ return stat1.type < stat2.type; }
//
//}

//void ChannelImpl::drawStatistics()
//{
//    const PixelViewport& pvp = getPixelViewport();
//    EQASSERT( pvp.hasArea( ));
//    if( !pvp.hasArea( ))
//        return;
//
//    eq::Config* config = getConfig();
//    EQASSERT( config );
//
//    std::vector< eq::FrameStatistics > statistics;
//    config->getStatistics( statistics );
//
//    if( statistics.empty( )) 
//        return;
//
//    EQ_GL_CALL( applyBuffer( ));
//    EQ_GL_CALL( applyViewport( ));
//    EQ_GL_CALL( setupAssemblyState( ));
//
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    applyScreenFrustum();
//
//    glMatrixMode( GL_MODELVIEW );
//    glDisable( GL_LIGHTING );
//    
//    Window* window = getWindow();
//    const Window::Font* font = window->getSmallFont();
//
//    // find min/max time
//    int64_t xMax = 0;
//    int64_t xMin = std::numeric_limits< int64_t >::max();
//
//    std::map< co::base::UUID, EntityData > entities;
//    std::map< co::base::UUID, IdleData >   idles;
//
//	std::vector< eq::FrameStatistics >::iterator sbegin = statistics.begin();
//	std::vector< eq::FrameStatistics >::iterator send = statistics.end();
//
//    for( std::vector< eq::FrameStatistics >::iterator i = sbegin;
//         i != send; ++i )
//    {
//        eq::FrameStatistics& frameStats  = *i;
//        SortedStatistics& configStats = frameStats.second;
//
//        for( SortedStatistics::iterator j = configStats.begin();
//             j != configStats.end(); ++j )
//        {
//            const co::base::UUID& id = j->first;
//            Statistics& stats = j->second;
//            std::sort( stats.begin(), stats.end(), _compare );
//
//            if( entities.find( id ) == entities.end( ))
//            {
//                EntityData& data = entities[ id ];
//                data.name = stats.front().resourceName;
//            }
//
//            for( Statistics::const_iterator k = stats.begin(); 
//                 k != stats.end(); ++k )
//            {
//                const Statistic& stat = *k;
//
//                switch( stat.type )
//                {
//                case Statistic::PIPE_IDLE:
//                {
//                    IdleData& data = idles[ id ];
//                    std::map< co::base::UUID, EntityData >::iterator l = 
//                        entities.find( id );
//
//                    if( l != entities.end( ))
//                    {
//                        entities.erase( l );
//                        data.name = stat.resourceName;
//                    }
//
//                    data.idle += (stat.idleTime * 100ll / stat.totalTime);
//                    ++data.nIdle;
//                    continue;
//                }
//                case Statistic::CHANNEL_FRAME_TRANSMIT:
//                case Statistic::CHANNEL_FRAME_COMPRESS:
//                case Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN:
//                    entities[ id ].doubleHeight = true;
//                    break;
//                default:
//                    break;
//                }
//
//                xMax = EQ_MAX( xMax, stat.endTime );
//                xMin = EQ_MIN( xMin, stat.startTime );
//            }
//        }
//    }
//    const Viewport& vp = getViewport();
//    const uint32_t width = uint32_t( pvp.w/vp.w );
//    
//	float scale = (float)(xMax - xMin) / width;
//
//    //while( (xMax - xMin) / scale > width )
//    //    scale *= 2;
//
//    xMax  /= scale;
//    int64_t xStart = xMax - width + SPACE;
//
//    const uint32_t height = uint32_t( pvp.h / vp.h);
//    uint32_t nextY = height - SPACE;
//
//    float dim = 0.0f;
//    for( std::vector< eq::FrameStatistics >::reverse_iterator 
//             i = statistics.rbegin(); i != statistics.rend(); ++i )
//    {
//        const eq::FrameStatistics& frameStats  = *i;
//        const SortedStatistics& configStats = frameStats.second;
//
//        int64_t     frameMin = xMax;
//        int64_t     frameMax = 0;
//
//        // draw stats
//        for( SortedStatistics::const_iterator j = configStats.begin();
//             j != configStats.end(); ++j )
//        {
//            const co::base::UUID&    id    = j->first;
//            const Statistics& stats = j->second;
//
//            if( stats.empty( ))
//                continue;
//
//            std::map< co::base::UUID, EntityData >::iterator l = entities.find( id );
//            if( l == entities.end( ))
//                continue;
//
//            EntityData& data = l->second;
//            if( data.yPos == 0 )
//            {
//                data.yPos = nextY;
//                nextY -= (HEIGHT + SPACE);
//                if( data.doubleHeight )
//                    nextY -= (HEIGHT + SPACE);
//            }
//
//            uint32_t y = data.yPos;
//
//            for( Statistics::const_iterator k = stats.begin(); 
//                 k != stats.end(); ++k )
//            {
//                const Statistic& stat = *k;
//
//                switch( stat.type )
//                {
//                case Statistic::PIPE_IDLE:
//                    continue;
//
//                case Statistic::CHANNEL_FRAME_TRANSMIT:
//                case Statistic::CHANNEL_FRAME_COMPRESS:
//                case Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN:
//                    y = data.yPos - (HEIGHT + SPACE);
//                    break;
//                default:
//                    break;
//                }
//
//                const int64_t startTime = stat.startTime / scale;
//                const int64_t endTime   = stat.endTime   / scale;
//
//                frameMin = EQ_MIN( frameMin, startTime );
//                frameMax = EQ_MAX( frameMax, endTime   );
//
//                if( endTime < xStart || endTime == startTime )
//                    continue;
//
//                float y1 = static_cast< float >( y );
//                float y2 = static_cast< float >( y - HEIGHT );
//                const float x1 = static_cast< float >( startTime - xStart );
//                const float x2 = static_cast< float >( endTime   - xStart );
//                std::stringstream text;
//                
//                switch( stat.type )
//                {
//                case Statistic::CONFIG_WAIT_FINISH_FRAME:
//                case Statistic::CHANNEL_FRAME_WAIT_READY:
//                case Statistic::CHANNEL_FRAME_WAIT_SENDTOKEN:
//                    y1 -= SPACE;
//                    y2 += SPACE;
//                    break;
//
//                case Statistic::CHANNEL_FRAME_COMPRESS:
//                    y1 -= SPACE;
//                    y2 += SPACE;
//                    // no break;
//                case Statistic::CHANNEL_READBACK:
//                    text << unsigned( 100.f * stat.ratio ) << '%';
//                    break;
//
//                default:
//                    break;
//                }
//                
//                //const Vector3f color( Statistic::getColor( stat.type ) - dim );
//                glColor3fv( (Statistic::getColor( stat.type ) - dim).array );
//
//                glBegin( GL_QUADS );
//                glVertex3f( x2, y1, 0.f );
//                glVertex3f( x1, y1, 0.f );
//                glVertex3f( x1, y2, 0.f);
//                glVertex3f( x2, y2, 0.f );
//                glEnd();
//
//                if( !text.str().empty( ))
//                {
//                    glColor3f( 0.f, 0.f, 0.f );
//                    glRasterPos3f( x1+1, y2, 0.f );
//                    font->draw( text.str( ));
//                }
//            }
//        }
//
//        frameMin -= xStart;
//        frameMax -= xStart;
//
//        float x = static_cast< float >( frameMin );
//        const float y1 = static_cast< float >( nextY );
//        const float y2 = static_cast< float >( height );
//
//        glBegin( GL_LINES );
//        glColor3f( .5f-dim, 1.0f-dim, .5f-dim );
//        glVertex3f( x, y1, 0.3f );
//        glVertex3f( x, y2, 0.3f );
//
//        x = static_cast< float >( frameMax );
//        glColor3f( .5f-dim, .5f-dim, .5f-dim );
//        glVertex3f( x, y1, 0.3f );
//        glVertex3f( x, y2, 0.3f );
//        glEnd();
//
//        dim += .1f;
//    }
//
//    // Entitity names
//    for( std::map< co::base::UUID, EntityData >::const_iterator i = entities.begin();
//         i != entities.end(); ++i )
//    {
//        const EntityData& data = i->second;
//
//        glColor3f( 1.f, 1.f, 1.f );
//        glRasterPos3f( 60.f, data.yPos-SPACE-12.0f, 0.99f );
//        font->draw( data.name );
//    }
//
//    // Global stats (scale, GPU idle)
//    glColor3f( 1.f, 1.f, 1.f );
//    nextY -= (HEIGHT + SPACE);
//    glRasterPos3f( 60.f, static_cast< float >( nextY ), 0.99f );
//    std::ostringstream text;
//    text << scale << "ms/pixel";
//
//    if( !idles.empty( ))
//        text << ", Idle:";
//
//    for( std::map< co::base::UUID, IdleData >::const_iterator i = idles.begin();
//         i != idles.end(); ++i )
//    {
//        const IdleData& data = i->second;
//        EQASSERT( data.nIdle > 0 );
//
//        text << " " << data.name << ":" << data.idle / data.nIdle << "%";
//    }
//
//    font->draw( text.str( ));
//    
//    // Legend
//    nextY -= SPACE;
//    float x = 0.f;
//
//    glRasterPos3f( x+1.f, nextY-12.f, 0.f );
//    glColor3f( 1.f, 1.f, 1.f );
//    font->draw( "channel" );
//
//    for( size_t i = 1; i < Statistic::CONFIG_START_FRAME; ++i )
//    {
//        const Statistic::Type type = static_cast< Statistic::Type >( i );
//        if( type == Statistic::CHANNEL_DRAW_FINISH ||
//            type == Statistic::PIPE_IDLE )
//        {
//            continue;
//        }
//
//        switch( type )
//        {
//          case Statistic::CHANNEL_FRAME_TRANSMIT:
//            x = 0.f;
//            nextY -= (HEIGHT + SPACE);
//
//            glColor3f( 1.f, 1.f, 1.f );
//            glRasterPos3f( x+1.f, nextY-12.f, 0.f );
//            break;
//
//          case Statistic::WINDOW_FINISH:
//            x = 0.f;
//            nextY -= (HEIGHT + SPACE);
//
//            glColor3f( 1.f, 1.f, 1.f );
//            glRasterPos3f( x+1.f, nextY-12.f, 0.f );
//            font->draw( "window" );
//            break;
//
//          case Statistic::NODE_FRAME_DECOMPRESS:
//            x = 0.f;
//            nextY -= (HEIGHT + SPACE);
//
//            glColor3f( 1.f, 1.f, 1.f );
//            glRasterPos3f( x+1.f, nextY-12.f, 0.f );
//            font->draw( "node" );
//            break;
//
//          default:
//            break;
//        }
//
//        x += 60.f;
//        const float x2 = x + 60.f - SPACE; 
//        const float y1 = static_cast< float >( nextY );
//        const float y2 = static_cast< float >( nextY - HEIGHT );
//
//        glColor3fv( Statistic::getColor( type ).array );
//        glBegin( GL_QUADS );
//        glVertex3f( x2, y1, 0.f );
//        glVertex3f( x,  y1, 0.f );
//        glVertex3f( x,  y2, 0.f );
//        glVertex3f( x2, y2, 0.f );
//        glEnd();
//
//        glColor3f( 0.f, 0.f, 0.f );
//        glRasterPos3f( x+1.f, nextY-12.f, 0.f );
//        font->draw( Statistic::getName( type ));
//    }
//    // nextY -= (HEIGHT + SPACE);
//    
//    glColor3f( 1.f, 1.f, 1.f );
//    window->drawFPS();
//    EQ_GL_CALL( resetAssemblyState( ));
//}

