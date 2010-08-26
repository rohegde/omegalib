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

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NetService: public InputService
{
public:
	// Allocator function
	static NetService* New() { return new NetService(); }

public:
	OMEGA_API virtual void initialize();
	OMEGA_API virtual void dispose();

private:
	OMEGA_API virtual void initHandshake();

private:
	WSADATA wsaData;
	const char* serverAddress;
	const char* serverPort;
	const char* dataPort;
	SOCKET ConnectSocket;
	
	#define DEFAULT_BUFLEN 512
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen;
};

}; // namespace omega

#endif