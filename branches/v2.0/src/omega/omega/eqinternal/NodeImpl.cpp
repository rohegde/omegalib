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
#include "omega/EqualizerDisplaySystem.h"

using namespace omega;
using namespace co::base;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
NodeImpl::NodeImpl( eq::Config* parent ):
	Node(parent),
	myServer(NULL)
{
	DEBUG_EQ_FLOW("NodeImpl::NodeImpl %1%", %parent);

	SystemManager* sys = SystemManager::instance();

	ApplicationBase* app = sys->getApplication();
	if(sys->isMaster())
	{
		// This is the master node. Create the master server instance.
		myServer = app->createMaster();
	}
	else
	{
		// This is the not master node. Create a standard server instance.
		myServer = app->createServer();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool NodeImpl::configInit( const eq::uint128_t& initID )
{
	DEBUG_EQ_FLOW("NodeImpl::configInit %1%", %initID);


	// Map the frame data object.
	omsg("NodeImpl::configInit - registering shared data object...");

	ConfigImpl* config = static_cast<ConfigImpl*>( getConfig());
	config->mapSharedData(initID);
	
	EqualizerDisplaySystem* eqds = (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem();
	eqds->finishInitialize(config);

	myServer->initialize();

	//const bool mapped = config->mapObject( &myFrameData, config->getFrameData().getID() );
	//oassert( mapped );
	return Node::configInit(initID);
	//return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool NodeImpl::configExit()
{
	DEBUG_EQ_FLOW("NodeImpl::configExit %1%", %"NOINFO");

	eq::Config* config = getConfig();
	//config->unmapObject( &myFrameData );

	delete myServer;
	myServer = NULL;

	return Node::configExit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void NodeImpl::frameStart( const eq::uint128_t& frameID, const uint32_t frameNumber )
{
	DEBUG_EQ_FLOW("NodeImpl::frameStart %1% %2%", %frameID %frameNumber);



	SystemManager* sys = SystemManager::instance();

	ConfigImpl* config = (ConfigImpl*)getConfig();
	config->updateSharedData();
	const UpdateContext& uc = config->getUpdateContext();


	ServiceManager* im = SystemManager::instance()->getServiceManager();
	if(!myServer->isMaster()) im->poll();

	// Process events.
	int av = im->getAvailableEvents();
	if(av != 0)
	{
    	im->lockEvents();
    	// Dispatch events to application server.
    	for( int evtNum = 0; evtNum < av; evtNum++)
    	{
    		Event* evt = im->getEvent(evtNum);
    		myServer->handleEvent(*evt);
    	}
    	im->unlockEvents();
	}
	im->clearEvents();

	myServer->update(uc);

	Node::frameStart(frameID, frameNumber);
}

