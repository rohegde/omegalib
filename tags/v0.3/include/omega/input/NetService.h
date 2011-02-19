/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

#include "omega/osystem.h"
#include "omega/InputManager.h"

#include "pqlabs/PQMTClient.h"
using namespace PQ_SDK_MultiTouch;

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
class NetService: public InputService
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
};

}; // namespace omega

#endif
