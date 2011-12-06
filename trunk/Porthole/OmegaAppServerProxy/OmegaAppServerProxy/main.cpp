/*
 ** server.c -- a stream socket server demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "utility.h"

#define PORT_NO "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connectionSockets queue will hold
#define MAXDATASIZE 250 // max number of bytes we can get at once 

using namespace std;

void *get_in_addr(struct sockaddr *sa);
void sigchld_handler(int s);
void generateGUIMsg( int socket);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void generateGUIMsg( int socket)
{
    int uniqueID = 1;
    
    string GUIMsg;
    string spacer = ":";
    string seperator = "|";

    //Buttons section
    ////////////////////////////////    
    
    string buttonId = intToString ( uniqueID );
    string buttonIdent = "I am a button";
    string buttonDescr = "I love to be pushed.";
    string buttonText = "Push Me";

    GUIMsg = GUIMsg + "0";
    GUIMsg = GUIMsg + spacer + buttonId;
    GUIMsg = GUIMsg + spacer + buttonIdent;
    GUIMsg = GUIMsg + spacer + buttonDescr;
    GUIMsg = GUIMsg + spacer + buttonText;
    GUIMsg = GUIMsg + spacer;
    GUIMsg = GUIMsg + seperator;    
    uniqueID++;
    
    buttonId = intToString ( uniqueID );
    buttonIdent = "OMG I am a button";
    GUIMsg = GUIMsg + "0";
    GUIMsg = GUIMsg + spacer + buttonId;
    GUIMsg = GUIMsg + spacer + buttonIdent;
    GUIMsg = GUIMsg + spacer + buttonDescr;    
    GUIMsg = GUIMsg + spacer + buttonText;
    GUIMsg = GUIMsg + spacer;
    GUIMsg = GUIMsg + seperator;
    uniqueID++;
    
    //Slider section
    ////////////////////////////////
    string numSldiers = intToString( 1 );
    
    string sliderId = intToString ( uniqueID );
    string sliderIdent = "Slide to the left";
    string sliderDescr = "I like need to slide";

    string sliderMin = intToString( 0 );
    string sliderMax = intToString( 10 );
    string sliderVal = intToString( 5 );
    
    GUIMsg = GUIMsg + "1";
    GUIMsg = GUIMsg + spacer + sliderId;
    GUIMsg = GUIMsg + spacer + sliderIdent;
    GUIMsg = GUIMsg + spacer + sliderDescr;
    GUIMsg = GUIMsg + spacer + sliderMin;
    GUIMsg = GUIMsg + spacer + sliderMax;    
    GUIMsg = GUIMsg + spacer + sliderVal;    
    GUIMsg = GUIMsg + spacer;
    GUIMsg = GUIMsg + seperator;
    uniqueID++;
    
    
    //Slider section
    ////////////////////////////////
    string numToggles = intToString( 1 );
    
    string toggleId = intToString ( uniqueID );
    string toggleIdent = "On...Off...?!";
    string toggleDescr = "I like need to waffle";
    string toggleInit = intToString( 1 );

    GUIMsg = GUIMsg + "2";
    GUIMsg = GUIMsg + spacer + toggleId;
    GUIMsg = GUIMsg + spacer + toggleIdent;
    GUIMsg = GUIMsg + spacer + toggleDescr;
    GUIMsg = GUIMsg + spacer + toggleInit;        
    GUIMsg = GUIMsg + spacer;    
    GUIMsg = GUIMsg + seperator;
    uniqueID++;
        
    //Send the ident which is the first 4 bytes
    ////////////////////////////////        
    if (send(socket, "mgui", 4, 0) == -1) perror("Error sending mgui");
    
    //Send the length which is the next 4 bytes
    ////////////////////////////////        
    unsigned long length = GUIMsg.length();
    const char* pBytesOfLength = (const char*)&length;    
    if (send(socket, pBytesOfLength, 4, 0) == -1) perror("Error sending mgui length");
    
    //Send the message
    ////////////////////////////////        
    if (send(socket, GUIMsg.c_str(), GUIMsg.length(), 0) == -1) perror("Error sending mgui msg");
    fprintf(stderr, "GUIMsg Sent\n");
    return;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//
int main(void)
{
    int listenSocket, connectionSocket;  // listen on sock_fd, new connectionSocket on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, PORT_NO, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((listenSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        
        if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        
        if (bind(listenSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(listenSocket);
            perror("server: bind");
            continue;
        }
        
        break;
    }
    
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }
    
    freeaddrinfo(servinfo); // all done with this structure
    
    if (listen(listenSocket, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    fprintf( stderr , "Waiting for connectionSockets ..... \n");
    
    sin_size = sizeof their_addr;
    connectionSocket = accept(listenSocket, (struct sockaddr *)&their_addr, &sin_size);
    if (connectionSocket == -1) perror("accept");
        
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    fprintf(stderr, "Got connectionSocket from %s\n", s);
        
    long numbytes;
    char buf[MAXDATASIZE];
    while (1)
    {
        static bool guiSent = false;
        
        if( !guiSent )
        {
            generateGUIMsg( connectionSocket );
            guiSent = true;
        }
        
        if ((numbytes = recv(connectionSocket, buf, MAXDATASIZE-1, 0)) == -1)             
        {
            perror("recv");
            exit(1);
        }
        if( numbytes > 0 )
        {
            buf[numbytes] = '\0';
            printf("Received '%s'\n",buf);
        }                
            
        string closeFilter = string(buf);
        if ( closeFilter == "CommunicationComplete")
        {
            break;
        }
    }        
    close(connectionSocket);  // parent doesn't need this]
    printf("Done\n");

    return 0;
}