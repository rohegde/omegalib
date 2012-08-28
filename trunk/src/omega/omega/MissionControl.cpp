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
#include "omega/MissionControl.h"
#include "omega/PythonInterpreter.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
MissionControlConnection::MissionControlConnection(ConnectionInfo ci, MissionControlServer* server): 
	TcpConnection(ci),
	myServer(server)
{
}
        
///////////////////////////////////////////////////////////////////////////////////////////////////
void MissionControlConnection::handleData()
{
    // Read message header.
	char header[4];
    read((byte*)myBuffer, 4);
	memcpy(header, myBuffer, 4);

    // Read data length.
	int dataSize;
    read((byte*)myBuffer, 4);
	memcpy(&dataSize, myBuffer, 4);

    // Read data.
    read((byte*)myBuffer, dataSize);
	myBuffer[dataSize] = '\0';

	if(!strncmp(header, "scmd", 4)) 
	{
		//script command message
		String command(myBuffer);
		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		if(interp != NULL)
		{
			interp->queueCommand(command);
		}
	}
	//if(!strncmp(header, "help", 4)) 
	//{
	//	// Request for help string.
	//	String command(myBuffer);
	//	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	//	if(interp != NULL)
	//	{
	//		String helpString = interp->getHelpString("");
	//		sendMessage("help", (void*)helpString.c_str(), helpString.size());
	//	}
	//}
	if(!strncmp(header, "strq", 4)) 
	{
		// Request for stats names.
		String statIds = "";
		StatsManager* sm = SystemManager::instance()->getStatsManager();
		if(sm != NULL)
		{
			foreach(Stat* s, sm->getStats())
			{
				statIds.append(s->getName());
				statIds.append("|");
			}
			sendMessage("strq", (void*)statIds.c_str(), statIds.size());
		}
	}
	if(!strncmp(header, "sten", 4)) 
	{
		StatsManager* sm = SystemManager::instance()->getStatsManager();
		if(sm != NULL)
		{
			// Set enabled stats.
			String stats(myBuffer);
			myEnabledStats.clear();
			std::vector<String> statVector = StringUtils::tokenise(stats, " ");
			foreach(String statId, statVector)
			{
				Stat* s = sm->findStat(statId);
				if(s != NULL)
				{
					myEnabledStats.push_back(s);
				}
			}
		}
	}
	if(!strncmp(header, "stup", 4)) 
	{
		if(myEnabledStats.size() > 0)
		{
			// Request for stats update.
			String statIds = "";
			foreach(Stat* s, myEnabledStats)
			{
				statIds.append(ostr("%1% %2% %3% %4% %5% ", %s->getName() %(int)s->getCur() %(int)s->getMin() %(int)s->getMax() %(int)s->getAvg()));
			}
			sendMessage("stup", (void*)statIds.c_str(), statIds.size());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MissionControlConnection::handleClosed()
{
    ofmsg("Mission control connection closed (id=%1%)", %getConnectionInfo().id);
	myServer->closeConnection(this);
}
        
///////////////////////////////////////////////////////////////////////////////////////////////////
void MissionControlConnection::handleConnected()
{
	TcpConnection::handleConnected();

    ofmsg("Mission control connection open (id=%1%)", %getConnectionInfo().id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MissionControlConnection::sendMessage(const char* header, void* data, int size)
{
	write((void*)header, 4);
	write(&size, sizeof(int));
	write(data, size);
}

///////////////////////////////////////////////////////////////////////////////////////////
void MissionControlServer::initialize() 
{
	omsg("MissionControlServer: adding log listener");
	// Register the mission control server as a log listener
	ologaddlistener(this);

	TcpServer::initialize();
}

///////////////////////////////////////////////////////////////////////////////////////////
void MissionControlServer::dispose() 
{
	omsg("MissionControlServer: removing log listener");
	ologremlistener(this);

	TcpServer::dispose();
}

///////////////////////////////////////////////////////////////////////////////////////////
TcpConnection* MissionControlServer::createConnection(const ConnectionInfo& ci)
{
	MissionControlConnection* conn = new MissionControlConnection(ci, this);
	myConnections.push_back(conn);
	return conn;
}

///////////////////////////////////////////////////////////////////////////////////////////
void MissionControlServer::closeConnection(MissionControlConnection* conn)
{
	myConnections.remove(conn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MissionControlServer::broadcastMessage(const char* header, void* data, int size)
{
	foreach(MissionControlConnection* conn, myConnections)
	{
		if(conn->getState() == TcpConnection::ConnectionOpen)
		{
			conn->sendMessage(header, data, size);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MissionControlServer::addLine(const String& line)
{
	broadcastMessage("smsg", (void*)line.c_str(), line.size());
}
