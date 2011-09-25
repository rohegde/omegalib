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
Dictionary<String, omega::Vector2i> sCanvasChannels;
Dictionary<String, omega::Vector2i> sCanvasSize;

// TODO: modify to make it work with multiple channels.
ChannelImpl* sCanvasChannelPointers[ConfigImpl::MaxCanvasChannels][ConfigImpl::MaxCanvasChannels];

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::ChannelImpl( eq::Window* parent ) 
	:eq::Channel( parent ), myWindow(parent), myInitialized(false)
{
	myLock.lock();
	if(!initStaticVars)
	{
		sCanvasChannels["default"] = Vector2i(0, 0);
		sCanvasSize["default"] = Vector2i(0, 0);
		initStaticVars = true;
	}
	myLock.unlock();
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

	String name = getName();
	vector<String> args = StringUtils::split(name, "x,");
	myChannelInfo.index = Vector2i(atoi(args[0].c_str()), atoi(args[1].c_str()));
	int ix = myChannelInfo.index[0];
	int iy = myChannelInfo.index[1];

	int w = getPixelViewport().w;
	int h = getPixelViewport().h;

	myChannelInfo.offset = Vector2i(ix * w, iy * h);
	myChannelInfo.size = Vector2i(w, h);

	myLock.lock();
	// Refresh the number of channels in this view.
	bool canvasChanged = false;
	if(sCanvasChannels["default"].x() <= ix) 
	{
		sCanvasChannels["default"].x() = ix + 1;
		canvasChanged = true;
	}
	if(sCanvasChannels["default"].y() <= iy) 
	{
		sCanvasChannels["default"].y() = iy + 1;
		canvasChanged = true;
	}
	sCanvasChannelPointers[ix][iy] = this;
	// If the number of channels in this view has been updated, refresh the view pixel size.
	if(canvasChanged)
	{
		sCanvasSize["default"].x() = sCanvasChannels["default"].x() * w;
		sCanvasSize["default"].y() = sCanvasChannels["default"].y() * h;
	}
	myLock.unlock();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::initialize()
{
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ofmsg("Initializing channel. pipe id=%1% canvas=default channels=%2%x%3% size=%4%x%5%",
		%pipe->getClient()->getId()
		%sCanvasChannels["default"].x() %sCanvasChannels["default"].y()
		%sCanvasSize["default"].x() %sCanvasSize["default"].y());

	myChannelInfo.canvasChannels = sCanvasChannels["default"];
	myChannelInfo.canvasSize = sCanvasSize["default"];
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameDraw( const co::base::uint128_t& spin )
{
	if(!myInitialized) 
	{
		initialize();
		myInitialized = true;
	}

	eq::Channel::frameDraw( spin );

	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();

	setupDrawContext(&myDC, spin);

	myDC.layer = view->getLayer();
	client->draw(myDC);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewFinish( const co::base::uint128_t& spin )
{
	eq::Channel::frameViewFinish( spin );

	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));

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

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::Vector2i ChannelImpl::windowToCanvas(const omega::Vector2i& point)
{
	return point + myChannelInfo.offset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const omega::DrawContext& ChannelImpl::getLastDrawContext()
{
	return myDC;
}
