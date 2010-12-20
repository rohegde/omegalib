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
#include <vector>

//using namespace std;
using namespace omega;

#ifdef WIN32
#define itoa _itoa
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on Winsock UDP Server Example:
// http://msdn.microsoft.com/en-us/library/ms740148
class NetClient
{
private:
	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;
	int Port;
	char SendBuf[1024];
	int BufLen;

public:
	NetClient::NetClient( const char* address, int port ){
		BufLen = 100;

		// Create a socket for sending data
		SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		// Set up the RecvAddr structure with the IP address of
		// the receiver
		RecvAddr.sin_family = AF_INET;
		RecvAddr.sin_port = htons(port);
		RecvAddr.sin_addr.s_addr = inet_addr(address);
		printf("NetClient %s:%i created...\n", address, port);
	}// CTOR

	void NetClient::sendEvent( char* eventPacket ){
		// Send a datagram to the receiver
		printf("InputService: Sending datagram '%s' to receiver...\n", eventPacket);
		sendto(SendSocket, 
		  eventPacket, 
		  strlen(eventPacket), 
		  0, 
		  (SOCKADDR *) &RecvAddr, 
		  sizeof(RecvAddr));
	}// SendEvent
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestApplication: public Application
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void draw(DrawContext& context)
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Checks the type of event. If a valid event, creates an event packet and returns true. Else return false.
	virtual bool handleEvent(const InputEvent& evt)
	{
		eventPacket = new char[99];
		eventPacket = itoa(evt.serviceType, eventPacket, 10); // Append input type
		strcat( eventPacket, ":" );
		char floatChar[32];
		switch(evt.serviceType)
		{
		case InputService::Touch:
			x = evt.position[0];
			y = evt.position[1];
			//printf(" Touch type %d \n", evt.type); 
			//printf("               at %f %f \n", x, y ); 

			// Converts gesture type to char, appends to eventPacket
			sprintf(floatChar,"%d",evt.type);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts id to char, appends to eventPacket
			sprintf(floatChar,"%d",evt.sourceId);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts x to char, appends to eventPacket
			sprintf(floatChar,"%f",x);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts y to char, appends to eventPacket
			sprintf(floatChar,"%f",y);
			strcat( eventPacket, floatChar );

			if( evt.numberOfPoints == 1 ){ // TouchPoint down/up/move
				// Converts xWidth to char, appends to eventPacket
				strcat( eventPacket, "," ); // Spacer
				sprintf(floatChar,"%f", evt.pointSet[0][0] );
				strcat( eventPacket, floatChar );
				
				// Converts yWidth to char, appends to eventPacket
				strcat( eventPacket, "," ); // Spacer
				sprintf(floatChar,"%f", evt.pointSet[0][1] );
				strcat( eventPacket, floatChar );
			} else if( evt.numberOfPoints == 2 ){ // Touch Gestures

			}

			strcat( eventPacket, " " ); // Spacer

			return true;
			break;
		case InputService::Pointer:
			x = evt.position[0];
			y = evt.position[1];

			// Converts x y float to chars and appents to eventPacket char*
			sprintf(floatChar,"%f",x);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," );
			sprintf(floatChar,"%f",y);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, " " );
			return true;
			break;

		case InputService::Mocap:
		{
#ifdef LAPTOP
			Observer* obs = getDisplaySystem()->getObserver(0);
			obs->update(evt.position, evt.rotation);
#else
			// Converts id to char, appends to eventPacket
			sprintf(floatChar,"%d",evt.sourceId);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts xPos to char, appends to eventPacket
			sprintf(floatChar,"%f",evt.position[0]);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts yPos to char, appends to eventPacket
			sprintf(floatChar,"%f",evt.position[1]);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts zPos to char, appends to eventPacket
			sprintf(floatChar,"%f",evt.position[2]);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts xRot to char, appends to eventPacket
			sprintf(floatChar,"%f",evt.rotation[0]);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts yRot to char, appends to eventPacket
			sprintf(floatChar,"%f",evt.rotation[1]);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, "," ); // Spacer

			// Converts zRot to char, appends to eventPacket
			sprintf(floatChar,"%f",evt.rotation[2]);
			strcat( eventPacket, floatChar );
			strcat( eventPacket, " " ); // Spacer
			return true;
#endif
			break;

			break;
		}
		default: break;
		}
		return false;
	}
	
	void startConnection();
	SOCKET startListening();
	char* getEvent();
	void sendToClients( char* );
private:
	void createClient(const char*,int);

	WSADATA wsaData;
	const char* serverPort;
	SOCKET ListenSocket;
	
	char* eventPacket;
	
	#define DEFAULT_BUFLEN 512
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen;
	
	// Temp socket until multi-client is implemented
	std::map<char*,NetClient*> netClients;
	//vector<NetClient*> netClients;
	NetClient* tempClient;

	private:
		float rx;
		float ry;
		float rz;

		float x;
		float y;
		float z;

		float mx;
		float my;
		float mz;

		float lx;
		float ly;
		float lz;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	sockaddr_in clientInfo;
	int ret = sizeof(struct sockaddr);
	const char* clientAddress;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, (SOCKADDR*)&clientInfo, &ret);

	if (ClientSocket == INVALID_SOCKET) {
		//printf("NetService: accept failed: %d\n", WSAGetLastError());
		// Commented out: We do not want to close the listen socket
		// since we are using a non-blocking socket until we are done listening for clients.
		//closesocket(ListenSocket);
		//WSACleanup();
		return NULL;
	} else {
		// Gets the clientInfo and extracts the IP address
		clientAddress = inet_ntoa(clientInfo.sin_addr);
		printf("InputService: Client '%s' Accepted.\n", clientAddress);
	}
	
	// Wait for client handshake
	// Here we constantly loop until data is received.
	// Because we're using a non-blocking socket, it is possible to attempt to receive before data is
	// sent, resulting in the 'recv failed' error that is commented out.
	bool gotData = false;
	printf("InputService: Waiting for client handshake\n");
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			//printf("InputService: Bytes received: %d\n", iResult);
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
				printf("InputService: '%s' requests data to be sent on port '%d'\n", clientAddress, dataPort);
				createClient( clientAddress, dataPort );
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* TestApplication::getEvent(){
	return eventPacket;
}// getEvent

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestApplication::createClient(const char* clientAddress, int dataPort){
	// Generate a unique name for client "address:port"
	char* addr = new char[128];
	strcpy( addr, clientAddress );
	char buf[32];
	strcat( addr, ":" );
	strcat( addr, itoa(dataPort,buf,10) );
	
	netClients[addr] =  new NetClient( clientAddress, dataPort );
	//netClients.push_back( new NetClient( clientAddress, dataPort ) );
}// createClient

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestApplication::sendToClients(char* event){
	// Iterate through all clients
	
	std::map<char*,NetClient*>::iterator itr = netClients.begin();
	while( itr != netClients.end() ){
		NetClient* client = itr->second;
		client->sendEvent( event );
		itr++;
	}// while
	
	//for( int i = 0; i < netClients.size(); i++ ){
	//	netClients[i]->sendEvent( event );
	//}
}// createClient

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(int argc, char** argv)
{
	Config* cfg = new Config("../../data/oinputserver.cfg");

	SystemManager* sys = SystemManager::instance();
	sys->setup(cfg);

	TestApplication app;
	sys->setApplication(&app);

	sys->initialize();
	sys->getInputManager()->start();
	//sys->run();
	app.startConnection();
	
	printf("NetService: Starting to listen for clients... \n");
	while(true){
		// Start listening for clients (non-blocking)
		app.startListening();
		
		app.sendToClients( app.getEvent() );
		// Get events
		/*
		int av = sys->getInputManager()->getAvailableEvents();
		if(av != 0)
		{
			// @todo: Instead of copying the event list, we can lock the main one.
			InputEvent evts[InputManager::MaxEvents];
			sys->getInputManager()->getEvents(evts, InputManager::MaxEvents);
			for( int evtNum = 0; evtNum < av; evtNum++)
			{
				if( app.handleEvent(evts[evtNum]) ){ // is there an event?
					// Send event to clients
					app.sendToClients( app.getEvent() );
				}
			}
		}// if
		*/
		

	}// while
	
	sys->cleanup();
}
