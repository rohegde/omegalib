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
#include "omega.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
void logEvent(const Event& e)
{
	ofmsg("Event received: position %1%", %e.position);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::instance();

	// Add a default filesystem data sources (used to retrieve configuration files and other resources)
	DataManager* dm = sys->getDataManager();
	dm->addSource(new FilesystemDataSource("./"));
	dm->addSource(new FilesystemDataSource("./../../data/"));

	// Load a configuration file for this application and setup the system manager.
	Config* cfg = new Config("eventlogger.cfg");
	sys->setup(cfg);

	// Initialize the system manager
	sys->initialize();

	// Start running services and listening to events.
	ServiceManager* sm = sys->getServiceManager();
	sm->start();

	omsg("eventlogger start logging events...");
	while(true)
	{
		// Poll services for new events.
		sm->poll(); 

		// Get available events
		Event evts[OMEGA_MAX_EVENTS];
		int av;
		if(0 != (av = sm->getEvents(evts, ServiceManager::MaxEvents)))
		{
			for( int evtNum = 0; evtNum < av; evtNum++)
			{
				logEvent(evts[evtNum]);
			}
		}// if

	}// while
	
	sys->cleanup();

	delete cfg;
}


