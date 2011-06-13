/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto		anishimoto42@gmail.com
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
#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

#include "omega/osystem.h"
#include "omega/ServiceManager.h"
#include <sys/timeb.h>

#include "pqlabs/PQMTClient.h"
using namespace PQ_SDK_MultiTouch;
#define OMEGA_US_LINUX

#if defined (linux)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h> // needed for close()
#include <string>
#endif
#if defined (WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NetService: public Service
{
public:
	// Allocator function
	static NetService* New() { return new NetService(); }

public:
	virtual void setup(Setting& settings);
	virtual void initialize();
	virtual void poll();
	virtual void dispose();
	void setServer(const char*,const char*);
	void setDataport(const char*);
	void setScreenResolution(int,int);
private:
	void initHandshake();
	void parseDGram(int);
private:
	NetService* mysInstance;
#if defined (WIN32)
	WSADATA wsaData;
	SOCKET ConnectSocket;
	SOCKET RecvSocket;	
#endif
#if defined (linux)
	int ConnectSocket;
	int RecvSocket;
#endif
	struct timeval timeout;
	sockaddr_in SenderAddr;

	const char* serverAddress;
	const char* serverPort;
	const char* dataPort;
	
	#define DEFAULT_BUFLEN 512
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;

	int SenderAddrSize;
	int recvbuflen;
	bool readyToReceive;
	int screenX;
	int screenY;

	std::map<int,float*> touchlist;
};

}; // namespace omega

#endif
