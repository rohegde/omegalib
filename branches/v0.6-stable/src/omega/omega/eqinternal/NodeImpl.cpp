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
NodeImpl::NodeImpl( eq::Config* parent ):
	Node(parent),
	myServer(NULL),
	myInitialized(NULL)
{
	Application* app = SystemManager::instance()->getApplication();
	myServer = app->createServer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool NodeImpl::configInit( const eq::uint128_t& initID )
{
	// Map the frame data object.
	ConfigImpl* config = static_cast<ConfigImpl*>( getConfig());
	const bool mapped = config->mapObject( &myFrameData, config->getFrameData().getID() );
	oassert( mapped );

	return Node::configInit(initID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool NodeImpl::configExit()
{
	eq::Config* config = getConfig();
	config->unmapObject( &myFrameData );

	odelete(myServer);

	return Node::configExit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NodeImpl::frameStart( const eq::uint128_t& frameID, const uint32_t frameNumber )
{
	// If server has not been initialized yet, do it now.
	if(myInitialized == false)
	{
		myServer->initialize();
		myInitialized = true;
	}

	static float lt = 0.0f;
	static float tt = 0.0f;
	// Compute dt.
	float t = (float)((double)clock() / CLOCKS_PER_SEC);
	if(lt == 0) lt = t;
	UpdateContext uc;
	uc.dt = t - lt;
	tt += uc.dt;
	uc.time = tt;
	uc.frameNum = frameNumber;
	lt = t;

	// Syncronize frame data (containing input events and possibly other stuff)
	myFrameData.sync(frameID);

	// Dispatch received events events to application server.
	int av = myFrameData.getNumEvents();
	if(av != 0)
	{
		for( int evtNum = 0; evtNum < av; evtNum++)
		{
			Event& evt = myFrameData.getEvent(evtNum);
			if(!evt.isProcessed())
			{
				myServer->handleEvent(evt);
			}
		}
	}
	myServer->update(uc);

	Node::frameStart(frameID, frameNumber);
}

