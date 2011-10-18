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
#include "omega/SystemManager.h"
#include "omega/TabletService.h"
#include "omega/StringUtils.h"
#include "omega/Tcp.h"

using namespace omega;

namespace omega {
///////////////////////////////////////////////////////////////////////////////////////////////////
class TabletConnection: public TcpConnection
{
public:
	TabletConnection(asio::io_service& ioService, int id, TabletService* service): 
		TcpConnection(ioService),
			myId(id)
	{	}

	virtual void handleData()
	{
	}

	virtual void handleClosed()
	{
		ofmsg("Connection closed (id=%1%)", %myId);
	}



private:
	static const int BufferSize = 1024;
	char myBuffer[BufferSize];

	TabletService* myService;
	int myId;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class TabletServer: public TcpServer
{
public:
	TabletServer(TabletService* service):
		myPointerCounter(0),
		myService(service)
		{}

	virtual TcpConnection* createConnection()
	{
		ofmsg("New tablet connection (id=%1%)", %myPointerCounter);
		TabletConnection* conn = new TabletConnection(myIOService, ++myPointerCounter, myService);
	    myClients.push_back(conn);
	    return conn;
	}

private:
	int myPointerCounter;
	TabletService* myService;
};
};

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletService::TabletService()
{
	myServer = new TabletServer(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TabletService::~TabletService()
{
	delete myServer;
	myServer = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TabletService::setup(Setting& settings)
{
	int port = 3490;
	if(settings.exists("port"))
	{
		port = settings["port"];
	}
	myServer->initialize(port);
	myServer->start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TabletService::poll() 
{
	myServer->poll();
}
