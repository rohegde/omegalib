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
	//! Stores the 4-character ids of messages used by the Mission Control protocol.
	class MissionControlMessageIds
	{
	public:
		//! bye! - tells the server this connection is over and will be closed after this message.
		static const char* Bye;
		//! mnis <name> - updates the name the server will use to identify this connection.
		static const char* MyNameIs;
		//! smsg <string> - sent by the server to a client: contains a log or response message that can be printed to the local console.
		static const char* LogMessage;
		//! sdto <name> - tells the server than messages from this connection should go ONLY to the named client.
		static const char* SendTo;
		//! sall - tells the server that messaged from this connection should be broadcast to all other clients. This is the default behavior.
		static const char* SendAll;

		//! scmd <command> - default behavior (see MissionControlMessageHandler): 
		//! the receiver will dispatch <command> to the script interpreter.
		static const char* ScriptCommand;
		//! strq [statname][|statname]* - default behavior (see MissionControlMessageHandler): 
		//! the receiver will send back a strq message with a list of pipe | separated stat names
		static const char* StatRequest;
		//! sted [statname]+ - default behavior (see MissionControlMessageHandler): 
		//! the receiver will enable a set of statistics whose data will be returned back for each stat update message
		static const char* StatEnable;
		//! stup [name cur min max avg]* - default behavior (see MissionControlMessageHandler): 
		//! the receiver will send back a stup message with current data (name, min, max, average times / values) about statistics enabled by a sten message.
		static const char* StatUpdate;

	private:
		//! Can't be instantiated.
		MissionControlMessageIds() {}
	};

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
		virtual ~MissionControlConnection() {}

		virtual void handleData();
		virtual void handleClosed();
		virtual void handleConnected();

		void sendMessage(const char* header, void* data, int size);
		//! Client side: tells the server we are done talking and waits for graceful close.
		void goodbyeServer();

		String getName() { return myName; }
		virtual void setName(const String& name);

	private:
		static const int BufferSize = 1024;
		char myBuffer[BufferSize];
		Ref<MissionControlServer> myServer;
		MissionControlConnection* myRecipient; // Message destination when private-message mode is enabled.
		Ref<IMissionControlMessageHandler> myMessageHandler;
		String myName;
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
		MissionControlConnection* findConnection(const String& name);
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
		MissionControlClient(): EngineModule("MissionControlClient") {}
		virtual ~MissionControlClient() 
		{ 
			// We make sure the connection object is destroyed here. This is important because if we
			// let default destruction take place, the io_service object may get destroyed before the
			// connection that uses it, leading to a crash. io_service is declared before the connection
			// object in the class members so things would work anyways, but it's better to be explicit.
			// In the future it would be good to keep this required behavior hidden in a base TcpClient class.
			dispose(); 
		}

		virtual void dispose();
		virtual void initialize();
		virtual void update(const UpdateContext& context);
		virtual void handleEvent(const UpdateContext& context);
		virtual bool handleCommand(const String& command);
		void connect(const String& host, int port);

		String getName() { return myConnection->getName(); }
		virtual void setName(const String& name) { myConnection->setName(name); }

	private:
		asio::io_service myIoService;
		Ref<MissionControlConnection> myConnection;
		Ref<IMissionControlMessageHandler> myMessageHandler;
	};
}; // namespace omicron

#endif