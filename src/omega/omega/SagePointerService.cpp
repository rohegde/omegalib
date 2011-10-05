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
#include "omega/DisplaySystem.h"
#include "omega/SagePointerService.h"
#include "omega/StringUtils.h"
#include "omega/Color.h"
#include "omega/Tcp.h"

using namespace omega;

namespace omega {
///////////////////////////////////////////////////////////////////////////////////////////////////
class SagePointerConnection: public TcpConnection
{
public:
	SagePointerConnection(asio::io_service& ioService, int id, SagePointerService* service): 
		TcpConnection(ioService),
			myId(id), myService(service),
			myButtonFlags(0)
	{	}

	virtual void handleData()
	{
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();

		// Read ip address
		readString(myBuffer, BufferSize, ':');

		// Read user name
		if(readString(myBuffer, BufferSize, ' ') > 0)
		{
			myName = myBuffer;
		}
		
		// Read device name
		readString(myBuffer, BufferSize, ' ');

		// Read command
		readString(myBuffer, BufferSize, ' ');
		int cmd = atoi(myBuffer);

		switch(cmd)
		{
		case 1:
			{
				handleMoveMessage();
				break;
			}
		case 2:
			{
				handleButtonMessage();
				break;
			}
		case 3:
			{
				omsg("WHEEL");
				break;
			}
		case 4:
			handleInfoMessage();
			break;
		}

		// Read until the end of the command
		readString(myBuffer, BufferSize, '\n');
	}

	virtual void handleClosed()
	{
		ofmsg("Connection closed (id=%1%)", %myId);
	}

	void handleButtonMessage()
	{
		// Read button
		readString(myBuffer, BufferSize, ' ');
		int btn = atoi(myBuffer);

		// Read pressed
		readString(myBuffer, BufferSize, ' ');
		int pressed = atoi(myBuffer);

        myService->lockEvents();
		Event* evt = myService->writeHead();
		evt->reset(pressed == 1 ? Event::Down : Event::Up, Service::Pointer, myId);
		evt->setPosition(myPosition[0], myPosition[1]);

		if(pressed == 1)
		{
			if(btn == 1) myButtonFlags |= Event::Left;
			if(btn == 2) myButtonFlags |= Event::Right;
			if(btn == 3) myButtonFlags |= Event::Middle;
		}
		else
		{
			if(btn == 1) myButtonFlags &= ~Event::Left;
			if(btn == 2) myButtonFlags &= ~Event::Right;
			if(btn == 3) myButtonFlags &= ~Event::Middle;
		}

		evt->setFlags(myButtonFlags);
		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		Ray ray = ds->getViewRay(myPosition);
		evt->setExtraDataType(Event::ExtraDataVector3Array);
		evt->setExtraDataVector3(0, ray.getOrigin());
		evt->setExtraDataVector3(1, ray.getDirection());
		myService->unlockEvents();
	}

	void handleMoveMessage()
	{
		// Read x
		readString(myBuffer, BufferSize, ' ');
		float x = atof(myBuffer);

		// Read y
		readString(myBuffer, BufferSize, ' ');
		float y = atof(myBuffer);

		DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
		Vector2i canvasSize = ds->getCanvasSize();

		myPosition[0] = x * canvasSize[0];
		myPosition[1] = (1 - y) * canvasSize[1];

        myService->lockEvents();
		Event* evt = myService->writeHead();
		evt->reset(Event::Move, Service::Pointer, myId);
		evt->setPosition(myPosition[0], myPosition[1]);
		evt->setFlags(myButtonFlags);

		Ray ray = ds->getViewRay(myPosition);
		evt->setExtraDataType(Event::ExtraDataVector3Array);
		evt->setExtraDataVector3(0, ray.getOrigin());
		evt->setExtraDataVector3(1, ray.getDirection());

		if(myService->isDebugEnabled())
		{
		   ofmsg("Sage pointer %1% position: %2% %3%", %myName %myPosition[0] %myPosition[1]);
		}

		myService->unlockEvents();
	}

	void handleInfoMessage()
	{
		// Read r
		readString(myBuffer, BufferSize, ' ');
		int r = atoi(myBuffer);

		// Read g
		readString(myBuffer, BufferSize, ' ');
		int g = atoi(myBuffer);

		// Read b
		readString(myBuffer, BufferSize, ' ');
		int b = atoi(myBuffer);

        myService->lockEvents();
		Event* evt = myService->writeHead();
		evt->reset(Event::Update, Service::Pointer, myId);
		evt->setPosition((float)r/255, (float)g/255, (float)b/255);
		evt->setExtraDataType(Event::ExtraDataString);
		evt->setExtraDataString(myName);
		myService->unlockEvents();
	}

private:
	static const int BufferSize = 1024;
	char myBuffer[BufferSize];

	SagePointerService* myService;
	int myId;
	uint myButtonFlags;
	String myName;
	Vector2i myPosition;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class SagePointerServer: public TcpServer
{
public:
	SagePointerServer(SagePointerService* service):
		myPointerCounter(0),
		myService(service)
		{}

	virtual TcpConnection* createConnection()
	{
		ofmsg("New sage pointer connection (id=%1%)", %myPointerCounter);
		SagePointerConnection* conn = new SagePointerConnection(myIOService, ++myPointerCounter, myService);
	    myClients.push_back(conn);
	    return conn;
	}

private:
	int myPointerCounter;
	SagePointerService* myService;
};
};

///////////////////////////////////////////////////////////////////////////////////////////////////
SagePointerService::SagePointerService()
{
	myServer = new SagePointerServer(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SagePointerService::~SagePointerService()
{
	delete myServer;
	myServer = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SagePointerService::setup(Setting& settings)
{
	myServer->initialize(20005);
	myServer->start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SagePointerService::poll() 
{
	myServer->poll();
}
