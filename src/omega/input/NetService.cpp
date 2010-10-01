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
void NetService::setup(Setting& settings)
{
	if(settings.exists("ServerIP"))
	{
		serverAddress =  (const char*)settings["ServerIP"];
	}
	if(settings.exists("MsgPort"))
	{
		serverPort = (const char*)settings["MsgPort"];
	}
	if(settings.exists("DataPort"))
	{
		dataPort = (const char*)settings["DataPort"];
	}
	if(settings.exists("ScreenX"))
	{
		screenX =  atoi((const char*)settings["ScreenX"]);
		printf("NetService: ScreenX set to %d\n", screenX);
	}
	if(settings.exists("ScreenY"))
	{
		screenY =  atoi((const char*)settings["ScreenY"]);
		printf("NetService: ScreenY set to %d\n", screenY);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Based on MSDN Winsock examples:
 * http://msdn.microsoft.com/en-us/library/ms738566(VS.85).aspx
 */
void NetService::initialize() 
{
	mysInstance = this;
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

	//printf("Bytes Sent: %ld\n", iResult);

	sockaddr_in RecvAddr;
	int Port = 27015;
	SenderAddrSize = sizeof(SenderAddr);

	// Create a UDP receiver socket to receive datagrams
	// http://msdn.microsoft.com/en-us/library/ms740120
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//-----------------------------------------------
	// Bind the socket to any address and the specified port.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(atoi(dataPort));
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(RecvSocket, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
	readyToReceive = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::poll()
{
	char RecvBuf[100];
	int  BufLen = 100;
	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	//printf("About to check for data\n");
	if( readyToReceive ){
		int result;
		
		// Create a set of fd_set to store sockets
		fd_set ReadFDs, WriteFDs, ExceptFDs;

		// Set collections to null
		FD_ZERO(&ReadFDs);
		FD_ZERO(&WriteFDs);
		FD_ZERO(&ExceptFDs);

        FD_SET(RecvSocket, &ReadFDs);
        FD_SET(RecvSocket, &ExceptFDs);

		timeout.tv_sec  = 0;
		timeout.tv_usec = 0;

		result = select(0, &ReadFDs, &WriteFDs, &ExceptFDs, &timeout);
		// Possible return values:
		// -1: error occurred
		// 0: timed out
		// > 0: data ready to be read
		// if -1 WSAGetLastError() can return error code
		if( result > 0 ){
			result = recvfrom(RecvSocket, 
			RecvBuf, 
			BufLen, 
			0, // If non-zero, socket is non-blocking
			(SOCKADDR *)&SenderAddr, 
			&SenderAddrSize);
			if( result > 0 ){
				int msgLen = result - 1;
				char* message = new char[msgLen];
				String msgStr = RecvBuf;

				// Parse message out of datagram
				int lastIndex = 0;
				int inputType = -1;
				float params[7];
				int currentParam = 0;

				for(int i = 0; i < msgLen; i++ ){
					if( msgStr[i] == ':' ){
						inputType = atoi( msgStr.substr(lastIndex,i).c_str() );
						lastIndex = i + 1;
					}
					if( msgStr[i] == ',' ){
						params[currentParam] = atof( msgStr.substr(lastIndex,i).c_str() );
						lastIndex = i + 1;
						currentParam++;
					}
					message[i] = RecvBuf[i];
					message[i+1] = '\0';
				}// for
				
				// Append last parameter
				params[currentParam] = atof( msgStr.substr(lastIndex,msgLen).c_str() );
				
				mysInstance->lockEvents();
				InputEvent* evt;

				switch(inputType){
					case(1): // MoCap
						evt = mysInstance->writeHead();
						evt->serviceType = InputService::Mocap;

						evt->sourceId = (int)(params[0] + 0.5);
						evt->position[0] = params[1];
						evt->position[1] = params[2];
						evt->position[2] = params[3];
						
						evt->rotation[0] = params[4];
						evt->rotation[1] = params[5];
						evt->rotation[2] = params[6];
						break;
					case(2): // Touch (points only not gestures)
						evt = mysInstance->writeHead();
						evt->serviceType = InputService::Touch;
						if( (int)(params[0]) == TP_DOWN ){
							evt->type = InputEvent::Down;
						}
						else if( (int)(params[0]) == TP_MOVE ){
							evt->type = InputEvent::Move;
						}
						else if( (int)(params[0]) == TP_UP ){
							evt->type = InputEvent::Up;
						}
						evt->sourceId = (int)(params[1]);
						evt->position[0] = params[2] * (float)screenX;
						evt->position[1] = params[3] * (float)screenY;

						evt->numberOfPoints = 1;
						evt->pointSet[0][0] = params[4] * (float)screenX;
						evt->pointSet[0][1] = params[5] * (float)screenY;
						break;
				}// switch
				mysInstance->unlockEvents();
				printf("Receiving datagram '%s'\n", message);
			} else {
				printf("recvfrom failed with error code '%d'\n", WSAGetLastError());
			}// if-else recv result
			
			
		} else {
			//if( result == 0 )
			//	printf("UDP socket has no data to receive '%d'\n", WSAGetLastError());
			//if( result == -1 )
			//	printf("UDP socket error code '%d'\n", WSAGetLastError());
		}// if-else select result
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::dispose() 
{
}
