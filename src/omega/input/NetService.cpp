/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * NetService method definitions. See NetService.h for more details.
 *********************************************************************************************************************/
#include "omega/input/NetService.h"

#include <winsock2.h>
#include <ws2tcpip.h>

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Based on MSDN Winsock examples:
 * http://msdn.microsoft.com/en-us/library/ms738566(VS.85).aspx
 */
void NetService::initialize() 
{
	serverAddress = "127.0.0.1";
	serverPort = "27000";
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("NetService: WSAStartup failed: %d\n", iResult);
		return;
	} else {
		printf("NetService: Winsock initialized \n");
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(serverAddress, serverPort, &hints, &result);
	if (iResult != 0) {
		printf("NetService: getaddrinfo failed: %d\n", iResult);
		WSACleanup();
	} else {
		printf("NetService: Client set to connect to address %s on port %s\n", serverAddress, serverPort);
	}
	
	// Create connection socket
	ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr=result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
		ptr->ai_protocol);

	// Connect to server.
	iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	if (ConnectSocket == INVALID_SOCKET) {
		printf("NetService: Error at socket(): %ld - Failed to connect to server\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("NetService: Unable to connect to server!\n");
		WSACleanup();
		return;
	} else {
		printf("NetService: Connected to server!\n");
	}
	initHandshake();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializes handshake and tells server which port to start streaming data.
void NetService::initHandshake() 
{
	// Send handshake
	dataPort = "7000";
	char sendbuf[50];
	sendbuf[0] = '\0';

	strcat( sendbuf, "data_on," );
	strcat( sendbuf, dataPort );
	
	printf("NetService: sending handshake: '%s'\n", sendbuf);

	iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("NetService: send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	printf("Bytes Sent: %ld\n", iResult);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::dispose() 
{
}
