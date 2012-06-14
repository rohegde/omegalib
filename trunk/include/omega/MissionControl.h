/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __TABLET_SERVICE_H__
#define __TABLET_SERVICE_H__

#include "omega/osystem.h"
#include "omega/StatsManager.h"
#include "omicron/Tcp.h"

namespace omega {
	
	class MissionControlServer;
    
	///////////////////////////////////////////////////////////////////////////////////////////////////
	class MissionControlConnection: public TcpConnection
	{
	public:
		MissionControlConnection(ConnectionInfo ci, MissionControlServer* server);

		virtual void handleData();
		virtual void handleClosed();
		virtual void handleConnected();

		void sendMessage(const char* header, void* data, int size);

	private:
		static const int BufferSize = 1024;
		char myBuffer[BufferSize];
		MissionControlServer* myServer;
		List<Stat*> myEnabledStats;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	class MissionControlServer: public TcpServer, public ILogListener
	{
	public:
		MissionControlServer()
		{}
        
		virtual TcpConnection* createConnection(const ConnectionInfo& ci);
		void closeConnection(MissionControlConnection* conn);
		void broadcastMessage(const char* header, void* data, int size);
	
		// from ILogListener
		virtual void addLine(const String& line);

	private:
		List<MissionControlConnection*> myConnections;
	};
}; // namespace omicron

#endif