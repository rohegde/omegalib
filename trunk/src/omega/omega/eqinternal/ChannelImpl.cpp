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

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::ChannelImpl( eq::Window* parent ) 
	:eq::Channel( parent ), myWindow(parent) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
ChannelImpl::~ChannelImpl() 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::setupDrawContext(DrawContext* context, const uint128_t& spin)
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
void ChannelImpl::frameDraw( const uint128_t& spin )
{
	ViewImpl* view  = static_cast< ViewImpl* > (const_cast< eq::View* >( getView( )));
	PipeImpl* pipe = static_cast<PipeImpl*>(getPipe());
	ApplicationClient* client = pipe->getClient();

	//String chName = this->getName();
	//ofmsg("%1%", %chName);

	//makeCurrent();

	// setup OpenGL State
	eq::Channel::frameDraw( spin );

	DrawContext context;
	setupDrawContext(&context, spin);

	context.layer = view->getLayer() & 0x03;
	client->draw(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewStart( const uint128_t& spin )
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
						if(client->handleEvent(evt, context))
						{
							evt.setProcessed();
						}
					}
				}
				else
				{
					if(client->handleEvent(evt, context))
					{
						evt.setProcessed();
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ChannelImpl::frameViewFinish( const uint128_t& spin )
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
