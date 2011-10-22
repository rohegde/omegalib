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

// horrible hack.
bool initStaticVars = false;
omega::Vector2i sCanvasChannels;
omega::Vector2i sCanvasSize;

// TODO: modify to make it work with multiple channels.
ChannelImpl* sCanvasChannelPointers[ConfigImpl::MaxCanvasChannels][ConfigImpl::MaxCanvasChannels];


omega::Lock sLock;

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::ChannelImpl( eq::Window* parent ) 
	:eq::Channel( parent ), myWindow(parent), myInitialized(false)
{
	sLock.lock();
	if(!initStaticVars)
	{
		sCanvasChannels = Vector2i(0, 0);
		sCanvasSize = Vector2i(0, 0);
		initStaticVars = true;
	}
	sLock.unlock();
	myDrawBuffer.initialize();
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

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::initialize()
{
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	String name = getName();
	bool interlaced = false;

	if(name[0] == 'i')
	{
		//interlaced = true;
		name = name.substr(1);
	}

	vector<String> args = StringUtils::split(name, "x,");
	myChannelInfo.index = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));
	int ix = myChannelInfo.index[0];
	int iy = myChannelInfo.index[1];

	int w = getPixelViewport().w;
	int h = getPixelViewport().h * (interlaced ? 2 : 1);
	
	ofmsg("Channel %1% size: %2% %3%", %name %w %h);

	myChannelInfo.offset = Vector2i(ix * w, iy * h);
	myChannelInfo.size = Vector2i(w, h);

	sLock.lock();
	// Refresh the number of channels in this view.
	bool canvasChanged = false;
	if(sCanvasChannels.x() <= ix) 
	{
		sCanvasChannels.x() = ix + 1;
		canvasChanged = true;
	}
	if(sCanvasChannels.y() <= iy) 
	{
		sCanvasChannels.y() = iy + 1;
		canvasChanged = true;
	}
	// If the number of channels in this view has been updated, refresh the view pixel size.
  	sCanvasChannelPointers[ix][iy] = this;
	if(canvasChanged)
	{
		sCanvasSize.x() = sCanvasChannels.x() * w;
		sCanvasSize.y() = sCanvasChannels.y() * h;
	}

	ofmsg("Initializing channel. pipe id=%1% canvas=default channels=%2%x%3% size=%4%x%5%",
		%pipe->getClient()->getId()
		%sCanvasChannels.x() %sCanvasChannels.y()
		%sCanvasSize.x() %sCanvasSize.y());

	myChannelInfo.canvasChannels = &sCanvasChannels;
	myChannelInfo.canvasSize = &sCanvasSize;

	sLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::setupDrawContext(DrawContext* context, const co::base::uint128_t& spin)
{
	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();

	eq::PixelViewport pvp = getPixelViewport();
	eq::PixelViewport gpvp = getWindow()->getPixelViewport();

	context->channel = &myChannelInfo;

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

	memcpy(context->modelview.data(), getHeadTransform().begin(), 16 * sizeof(float));
	memcpy(context->projection.data(), getFrustum().compute_matrix().begin(), 16 * sizeof(float));

	// Setup draw context
	myDrawBuffer.setGLId(getDrawBuffer());
	context->drawBuffer = &myDrawBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameDraw( const co::base::uint128_t& frameID )
{
	eq::Channel::frameDraw( frameID );
	//if(!myInitialized) 
	//{
	//	//initialize();
	//	myInitialized = true;
	//}
	//else
	{
		//ofmsg("frameDraw: channel %1% frame %2%", %this %frameID);
		ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
		PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
		ApplicationClient* client = pipe->getClient();

		setupDrawContext(&myDC, frameID);

		myDC.layer = view->getLayer();
		myDC.task = DrawContext::SceneDrawTask;
		client->draw(myDC);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewFinish( const co::base::uint128_t& frameID )
{
	eq::Channel::frameViewFinish( frameID );
	if(!myInitialized) 
	{
		initialize();
		myInitialized = true;
	}
	else
	{
		setupDrawContext(&myDC, frameID);

		myDC.layer = getLayers();
		myDC.task = DrawContext::OverlayDrawTask;
		getClient()->draw(myDC);

		if(isDrawStatisticsEnabled())
		{
			drawStatistics();
		}
		else if(isDrawFpsEnabled())
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
}


///////////////////////////////////////////////////////////////////////////////////////////////////
omega::Vector2i ChannelImpl::windowToCanvas(const omega::Vector2i& point)
{
	return point + myChannelInfo.offset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const omega::DrawContext& ChannelImpl::getLastDrawContext()
{
	//ofmsg("DC viewport: %1% %2%", %myDC.viewport.max %myDC.viewport.min);
	return myDC;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::ApplicationClient* ChannelImpl::getClient()
{
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	return pipe->getClient();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int ChannelImpl::getLayers()
{
	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	return view->getLayer();
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
