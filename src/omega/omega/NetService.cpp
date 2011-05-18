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
#include "omega/NetService.h"
using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::setup(Setting& settings)
{
	if(settings.exists("serverIP"))
	{
		serverAddress =  (const char*)settings["serverIP"];
	}
	if(settings.exists("msgPort"))
	{
		serverPort = (const char*)settings["msgPort"];
	}
	if(settings.exists("dataPort"))
	{
		dataPort = (const char*)settings["dataPort"];
	}
	if(settings.exists("screenX"))
	{
		screenX =  settings["screenX"];
		printf("NetService: screenX set to %d\n", screenX);
	}
	if(settings.exists("screenY"))
	{
		screenY =  settings["screenY"];
		printf("NetService: screenY set to %d\n", screenY);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::initialize() 
{
	mysInstance = this;
#if defined(WIN32)
	/*
	* Based on MSDN Winsock examples:
	* http://msdn.microsoft.com/en-us/library/ms738566(VS.85).aspx
	*/
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
#endif
#if defined(linux)
	/*
	* Based on Beej's Guide to Network Programming:
	* http://beej.us/guide/bgnet/output/html/multipage/index.html
	*/
	printf("NetService: Initializing using linux\n");

	struct addrinfo hints, *res;

	// First, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// Get the server address
	getaddrinfo(serverAddress, serverPort, &hints, &res);

	// Generate the socket
	ConnectSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// Connect to server
	int errno;
	int result = connect(ConnectSocket, res->ai_addr, res->ai_addrlen);

	if (result == -1) {
		printf("NetService: Unable to connect to server '%s' on port '%s': %s\n", serverAddress, serverPort, strerror(errno));
		return;
	} else {
		printf("NetService: Connected to server '%s' on port '%s'!\n", serverAddress, serverPort);
	}
#endif
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

	printf("NetService: Sending handshake: '%s'\n", sendbuf);

	int errno; // linux error code
	iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
#if defined(linux)
	if (iResult == -1) {
		printf("NetService: Send failed: %s\n", strerror(errno));
		return;
	}
#endif
#if defined(WIN32)
	if (iResult == SOCKET_ERROR) {
		printf("NetService: Send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}
	//printf("Bytes Sent: %ld\n", iResult);

	sockaddr_in RecvAddr;

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
#endif
#if defined(linux)
	/*
	* Based on Beej's Guide to Network Programming:
	* http://beej.us/guide/bgnet/output/html/multipage/index.html

	*/
	struct addrinfo hints, *res;

	// First, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	// Get the server address
	getaddrinfo(NULL, dataPort, &hints, &res);

	// Generate the socket
	RecvSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	iResult = bind(RecvSocket, res->ai_addr, res->ai_addrlen );
	if (iResult == -1) {
		printf("NetService: Failed to bind socket on port %s: %s\n", dataPort, strerror(errno));
		return;
	}
#endif
	readyToReceive = true;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::poll()
{
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

		// Check if UDP socket has data waiting to be read before
		// socket blocks to attempt to read.
		result = select(RecvSocket+1, &ReadFDs, &WriteFDs, &ExceptFDs, &timeout);
		// Possible return values:
		// -1: error occurred
		// 0: timed out
		// > 0: data ready to be read
		// if -1 WSAGetLastError() can return error code (Windows only)
		if( result > 0 ){
			parseDGram(result);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::parseDGram(int result)
{
#if defined(WIN32)
	result = recvfrom(RecvSocket, 
		recvbuf,
		DEFAULT_BUFLEN-1,
		0, // If non-zero, socket is non-blocking
		(SOCKADDR *)&SenderAddr, 
		&SenderAddrSize);
#endif
#if defined(linux)
	socklen_t addr_len;
	struct sockaddr_storage their_addr;
	int numbytes;
	char s[INET6_ADDRSTRLEN];

	addr_len = sizeof their_addr;
	numbytes = recvfrom(RecvSocket,
		recvbuf,
		DEFAULT_BUFLEN-1,
		0,
		(struct sockaddr *)&their_addr,
		&addr_len);

	struct sockaddr *sa = (struct sockaddr *)&their_addr;
	const void *inAddr;
	if (sa->sa_family == AF_INET){
		inAddr = &(((struct sockaddr_in*)sa)->sin_addr);
	} else {
		inAddr = &(((struct sockaddr_in6*)sa)->sin6_addr);
	}

	//printf("listener: got packet from %s\n",
	//    inet_ntop(their_addr.ss_family,
	//    inAddr,
	//    s, sizeof s));
	//printf("listener: packet is %d bytes long\n", numbytes);
	recvbuf[numbytes] = '\0';
	//printf("listener: packet contains \"%s\"\n", recvbuf);
	result = numbytes;
#endif
	if( result > 0 ){
		int msgLen = result - 1;
		char* message = new char[msgLen];
		std::string msgStr = recvbuf;

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
			message[i] = recvbuf[i];
			message[i+1] = '\0';
		}// for

		// Append last parameter
		params[currentParam] = atof( msgStr.substr(lastIndex,msgLen).c_str() );

		mysInstance->lockEvents();
		Event* evt;

		switch(inputType){
			case(Service::Mocap): // MoCap
				evt = mysInstance->writeHead();
				evt->serviceType = Service::Mocap;

				evt->sourceId = (int)(params[0] + 0.5);
				evt->position[0] = params[1];
				evt->position[1] = params[2];
				evt->position[2] = params[3];

				evt->orientation.x() = params[4];
				evt->orientation.y() = params[5];
				evt->orientation.z() = params[6];
				evt->orientation.w() = params[7];
				break;
			case(Service::Pointer): // Touch (points only not gestures)
				evt = mysInstance->writeHead();
				evt->serviceType = Service::Pointer;
				if( (int)(params[0]) == TP_DOWN ){
					evt->type = Event::Down;
				}
				else if( (int)(params[0]) == TP_MOVE ){
					evt->type = Event::Move;
				}
				else if( (int)(params[0]) == TP_UP ){
					evt->type = Event::Up;
				}
				evt->sourceId = (int)(params[1]);
				evt->position[0] = params[2] * (float)screenX;
				evt->position[1] = params[3] * (float)screenY;

				evt->numberOfPoints = 1;
				evt->pointSet[0][0] = params[4] * (float)screenX;
				evt->pointSet[0][1] = params[5] * (float)screenY;
				break;
			default:
				printf("NetService: Unsupported input type %d \n", inputType);
				break;
		}// switch
		mysInstance->unlockEvents();
		//printf("Receiving datagram '%s'\n", message);
	} else {
#if defined(WIN32)
		printf("recvfrom failed with error code '%d'\n", WSAGetLastError());
#endif
#if defined(linux)
		printf("recvfrom failed with error: '%s'\n", strerror(errno));
#endif
	}// if-else recv result
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::setServer(const char* address, const char* port) 
{
	printf("Server set to '%s' on port '%s'\n", address, port);
	serverAddress = address;
	serverPort = port;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::setDataport(const char* port) 
{
	printf("Dataport set to '%s'\n", port);
	dataPort = port;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NetService::setScreenResolution(int x, int y) 
{
	printf("Screen resolution set to %d %d\n", x, y);
	screenX = x;
	screenY = y;
}
