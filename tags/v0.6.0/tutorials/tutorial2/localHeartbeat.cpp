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
#include <omega>

using namespace omega;

// A simple configuration, creating a Heartbeat service 
// and setting it's rate to 1 event per second
const char* configString = 
"@config: { "
" services: { "
"  HeartbeatService: { "
"   rate = 1.0;"
"  };"
" };"
"};";

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// Load a configuration using the config string specified before. 
	// If we don't want to hardcode the configuration we can pass a the path to a file 
	// containing the config instead.
	Config cfg(configString);

	// Get the system manager instance.
	SystemManager* sys = SystemManager::instance();

	// Setup and initialize the system manager.
	sys->setup(&cfg);
	sys->initialize();

	// Start services and begin listening to events.
	ServiceManager* sm = sys->getServiceManager();
	sm->start();

	while(true)
	{
		// Poll services for new events.
		sm->poll(); 

		// Get available events
		Event evts[OMEGA_MAX_EVENTS];
		int av = sm->getEvents(evts, ServiceManager::MaxEvents);
		for(int evtNum = 0; evtNum < av; evtNum++)
		{
			ofmsg("Event received: timestamp %1%", %evts[evtNum].getTimestamp());
		}
	}
	
	sys->cleanup();
}


