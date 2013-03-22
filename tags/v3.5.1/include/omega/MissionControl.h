/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __TABLET_SERVICE_H__
#define __TABLET_SERVICE_H__

#include "omega/osystem.h"
#include "omega/StatsManager.h"
#include "omega/ModuleServices.h"
#include "omega/PythonInterpreter.h"
#include "omicron/Tcp.h"

namespace omega {
	
	class MissionControlServer;
	class MissionControlConnection;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API IMissionControlMessageHandler: public ReferenceType
	{
	public:
		virtual bool handleMessage(MissionControlConnection* sender, const char* header, char* data, int size) = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API MissionControlMessageHandler: public IMissionControlMessageHandler
	{
	public:
		virtual bool handleMessage(MissionControlConnection* sender, const char* header, char* data, int size);
	private:
		List<Stat*> myEnabledStats;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API MissionControlConnection: public TcpConnection
	{
	public:
		MissionControlConnection(ConnectionInfo ci, IMissionControlMessageHandler* msgHandler, MissionControlServer* server);

		virtual void handleData();
		virtual void handleClosed();
		virtual void handleConnected();

		void sendMessage(const char* header, void* data, int size);
		//! Client side: tells the server we are done talking and waits for graceful close.
		void goodbyeServer();

	private:
		static const int BufferSize = 1024;
		char myBuffer[BufferSize];
		MissionControlServer* myServer;
		IMissionControlMessageHandler* myMessageHandler;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API MissionControlServer: public TcpServer, public ILogListener
	{
	public:
		static const int DefaultPort = 22500;
	public:
		virtual void initialize();
		virtual void dispose();

		virtual TcpConnection* createConnection(const ConnectionInfo& ci);
		void closeConnection(MissionControlConnection* conn);
		void broadcastMessage(const char* header, void* data, int size, MissionControlConnection* sender = NULL);
		void setMessageHandler(IMissionControlMessageHandler* msgHandler) { myMessageHandler = msgHandler; }
	
		// from ILogListener
		virtual void addLine(const String& line);

	private:
		List<MissionControlConnection*> myConnections;
		Ref<IMissionControlMessageHandler> myMessageHandler;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API MissionControlClient: public EngineModule
	{
	public:
		virtual void dispose();
		virtual void initialize();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const UpdateContext& context);
		virtual bool handleCommand(const String& command);
		void connect(const String& host, int port);

	private:
		Ref<MissionControlConnection> myConnection;
		Ref<IMissionControlMessageHandler> myMessageHandler;
		asio::io_service myIoService;
	};
}; // namespace omicron

#endif