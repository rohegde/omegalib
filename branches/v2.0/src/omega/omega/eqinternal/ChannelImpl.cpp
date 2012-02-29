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

using namespace omega;
using namespace co::base;
using namespace std;

using namespace eq;

//! Comment do disable running of overlay render tasks.
#define ENABLE_OVERLAY_TASK

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::ChannelImpl( eq::Window* parent ) 
    :eq::Channel( parent ), myWindow(parent), myDrawBuffer(NULL) //, myInitialized(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::~ChannelImpl() 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
ViewImpl* ChannelImpl::getViewImpl() 
{
    return myView; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ChannelImpl::configInit(const eq::uint128_t& initID)
{
    eq::Channel::configInit(initID);

    EqualizerDisplaySystem* ds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();
    String name = getName();

    vector<String> args = StringUtils::split(name, "x,");
	int ix = atoi(args[0].c_str());
	int iy = atoi(args[1].c_str());

	myDC.tile = &ds->getDisplayConfig().tiles[ix][iy];

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::setupDrawContext(DrawContext* context, const co::base::uint128_t& spin)
{
    ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
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
    memcpy(mw.data(), getHeadTransform().begin(), 16 * sizeof(float));
    memcpy(context->projection.data(), getFrustum().compute_matrix().begin(), 16 * sizeof(float));

    context->modelview = mw * ds->getObserver(0)->getViewTransform();

    // Setup draw buffer
    if(myDrawBuffer == NULL)
    {
        myDrawBuffer = new RenderTarget(pipe->getGpuContext(), RenderTarget::RenderOnscreen, getDrawable());
    }
    context->drawBuffer = myDrawBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewStart( const co::base::uint128_t& frameID )
{
    eq::Channel::frameViewStart( frameID );
    //if(!myInitialized) 
    //{
    //    initialize();
    //    myInitialized = true;
    //    return;
    //}
    
    // In frame finish we just perform overlay draw operations, so always force the eye to be 
    // Cyclop. Also, if this method is called twice for the same frame (because of stereo rendering)
    // Ignore the second call (Right Eye)
    //if(getEye() == eq::fabric::EYE_RIGHT) return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameDraw( const co::base::uint128_t& frameID )
{
    eq::Channel::frameDraw( frameID );

    // Skip the first frame to give time to the channels to initialize
    if(frameID == 0) return;
    //if(frameID == 1)
    //{
    //    String name = getName();
    //    vector<String> args = StringUtils::split(name, "x,");
    //    int cx = atoi(args[0].c_str());
    //    int cy = atoi(args[1].c_str());

    //    ChannelImpl* channel = sCanvasChannelPointers[cx][cy];
    //    if(channel != this)
    //    {
    //        ofmsg("Initializing leaf channel for main channel %1%x%2%", %cx %cy);
    //        myChannelInfo = channel->myChannelInfo;
    //    }
    //    return;
    //}

    // Clear the frame buffer using the background color specified in display system.
    DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
    const Color& b = ds->getBackgroundColor();
    glClearColor(b[0], b[1], b[2], b[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    //ofmsg("frameDraw: channel %1% frame %2%", %this %frameID);
    ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
    PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
    RendererBase* client = pipe->getClient();

    // If the pipe has not been initialized yet, return now.
    if(!pipe->isReady()) return;

    setupDrawContext(&myDC, frameID);

    myDC.task = DrawContext::SceneDrawTask;
    client->draw(myDC);

#ifdef ENABLE_OVERLAY_TASK
    if(getEye() != eq::fabric::EYE_CYCLOP)
    {
        myDC.task = DrawContext::OverlayDrawTask;
        getClient()->draw(myDC);
    }
#endif 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewFinish( const co::base::uint128_t& frameID )
{
    eq::Channel::frameViewFinish( frameID );
#ifdef ENABLE_OVERLAY_TASK
    if(getEye() != eq::fabric::EYE_LAST && getEye() != eq::fabric::EYE_CYCLOP) return;

    // Skip the first frame to give time to the channels to initialize
    if(frameID == 0) return;

    setupDrawContext(&myDC, frameID);
    myDC.task = DrawContext::OverlayDrawTask;
    myDC.eye = DrawContext::EyeCyclop;

    EQ_GL_CALL( applyBuffer( ));
    EQ_GL_CALL( applyViewport( ));
    EQ_GL_CALL( setupAssemblyState( ));

    // If the pipe has not been initialized yet, return now.
    PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
    if(!pipe->isReady()) return;

    getClient()->draw(myDC);

    if(isDrawStatisticsEnabled())
    {
        drawStatistics();
    }
    else if(isDrawFpsEnabled())
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        applyScreenFrustum();

        glMatrixMode( GL_MODELVIEW );
        glDisable( GL_LIGHTING );

        getWindow()->drawFPS();
    }

    EQ_GL_CALL( resetAssemblyState( ));
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::RendererBase* ChannelImpl::getClient()
{
    PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
    return pipe->getClient();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ChannelImpl::isDrawStatisticsEnabled()
{
    ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
    return view->isDrawStatisticsEnabled();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ChannelImpl::isDrawFpsEnabled()
{
    ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
    return view->isDrawFpsEnabled();
}
