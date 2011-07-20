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

using namespace omega;
using namespace co::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
PipeImpl::PipeImpl(eq::Node* parent): 
	eq::Pipe(parent), myClient(NULL), myInitialized(false) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
ApplicationClient* PipeImpl::getClient() 
{ return myClient; }

///////////////////////////////////////////////////////////////////////////////////////////////////
FrameData& PipeImpl::getFrameData() 
{ return myFrameData; }

///////////////////////////////////////////////////////////////////////////////////////////////////
PipeImpl::~PipeImpl() 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool PipeImpl::configInit( const uint128_t& initID )
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

///////////////////////////////////////////////////////////////////////////////////////////////////
bool PipeImpl::configExit()
{
	eq::Config* config = getConfig();
	config->unmapObject( &myFrameData );

	return eq::Pipe::configExit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PipeImpl::frameStart( const uint128_t& frameID, const uint32_t frameNumber )
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
	//else
	{
		// Syncronize frame data (containing input events and possibly other stuff)
		myFrameData.sync(frameID);

		static float lt = 0.0f;
		// Compute dt.
		float t = (float)((double)clock() / CLOCKS_PER_SEC);
		UpdateContext context;
		if(lt == 0) lt = t;
		context.time = t - lt;
		context.dt = t - context.time;
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
