/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Test executable
 *********************************************************************************************************************/
#include "omega.h"
#include "omega/input/MouseService.h"

#include <winsock2.h>
#include <ws2tcpip.h>

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void draw(DrawContext& context)
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool handleEvent(const InputEvent& evt)
	{
		return true;
	}
	
	void startConnection();
	SOCKET startListening();
private:
	WSADATA wsaData;
	const char* serverPort;
	SOCKET ListenSocket;
	
	#define DEFAULT_BUFLEN 512
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	SystemManager* sys = SystemManager::instance();

	Config* cfg = new Config("../../data/test.cfg");
	//cfg->setDisplayConfig("--eq-config ../../data/eqc/test.eqc");
	cfg->load();

	sys->setup(cfg);

	TestApplication app;
	sys->setApplication(&app);

	sys->initialize();

	app.startConnection();
	
	printf("NetService: Starting to listen for clients... \n");
	while(true){
		// Start listening for clients (non-blocking)
		app.startListening();

		// Get events

		// Send events to clients

	}// while
	//sys->run();
	sys->cleanup();
}

/*
 * Based on MSDN Winsock examples:
 * http://msdn.microsoft.com/en-us/library/ms738566(VS.85).aspx
 *
 * Non-blocking socket example:
 * http://www.win32developer.com/tutorial/winsock/winsock_tutorial_4.shtm
 */
void TestApplication::startConnection(){
	serverPort = "27000";
	ListenSocket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("InputService: WSAStartup failed: %d\n", iResult);
		return;
	} else {
		printf("InputService: Winsock initialized \n");
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, serverPort, &hints, &result);
	if (iResult != 0) {
		printf("InputService: getaddrinfo failed: %d\n", iResult);
		WSACleanup();
	} else {
		printf("InputService: Server set to listen on port %s\n", serverPort);
	}

	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	// If iMode != 0, non-blocking mode is enabled.
	u_long iMode = 1;
	ioctlsocket(ListenSocket,FIONBIO,&iMode);

	if (ListenSocket == INVALID_SOCKET) {
		printf("InputService: Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	} else {
		printf("InputService: Listening socket created.\n");
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("InputService: bind failed: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}// startConnection

SOCKET TestApplication::startListening(){
	SOCKET ClientSocket;

	// Listen on socket
	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "InputService: Error at bind(): %ld\n", WSAGetLastError() );
		closesocket(ListenSocket);
		WSACleanup();
		return NULL;
	} else {
		//printf("NetService: Listening on socket.\n");
	}

	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET) {
		//printf("NetService: accept failed: %d\n", WSAGetLastError());
		// Commented out: We do not want to close the listen socket
		// since we are using a non-blocking socket until we are done listening for clients.
		//closesocket(ListenSocket);
		//WSACleanup();
		return NULL;
	} else {
		printf("InputService: Client Accepted.\n");
	}
	
	// Client performs handshake, check handshake

	// Get data port client wants data sent to

	// Register client to client list


	// Wait for client handshake
	// Here we constantly loop until data is received.
	// Because we're using a non-blocking socket, it is possible to attempt to receive before data is
	// sent, resulting in the 'recv failed' error that is commented out.
	bool gotData = false;
	printf("InputService: Waiting for client handshake\n");
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("InputService: Bytes received: %d\n", iResult);
			char* inMessage;
			char* portCStr;
			inMessage = new char[iResult];
			portCStr = new char[iResult];

			// Iterate through message string and
			// separate 'data_on,' from the port number
			int portIndex = -1;
			for( int i = 0; i < iResult; i++ ){
				if( recvbuf[i] == ',' ){
					portIndex = i + 1;
				} else if( i < portIndex ){
					inMessage[i] = recvbuf[i];
				} else {
					portCStr[i-portIndex] = recvbuf[i];
				}
			}

			// Make sure handshake is correct
			String handshake = "data_on";
			int dataPort = 7000; // default port
			if( handshake.find(inMessage) ){
				// Get data port number
				dataPort = atoi(portCStr);
				printf("InputService: Data_on received: '%d'\n", dataPort);
			}
			gotData = true;			
		} else if (iResult == 0)
			printf("InputService: Connection closing...\n");
		else {
			//printf("InputService: recv failed: %d\n", WSAGetLastError());
			//closesocket(ClientSocket);
			//WSACleanup();
			//return NULL;
		}

	} while (!gotData);
	

	return ClientSocket;
}// startListening