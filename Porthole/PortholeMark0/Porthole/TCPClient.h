//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#import <UIKit/UIKit.h>

#define DEFAUL_DEBUG_CLIENT    YES;

#define MAXDATASIZE 100 // max number of bytes we can get at once 

typedef enum { CLINET_INIT , CLINET_CONNECTED , CLINET_GUIRECVED } TCPClientState;

@class TCPClient;
 
@interface TCPClient : UIView
{
    TCPClientState myState;
    int connectionSocket;
    BOOL debugClient;
    
}

@property ( assign ) TCPClientState myState;

-(void) closeSocketToServer;

-(void) generateDebugAlert:(NSString*) msg;
-(void) generateErrorAlert:(NSString*) msg;

-(void*)get_in_addrWith:(struct sockaddr*)sa;
-(void) setupTCPConnectionTo:(NSString*)IP at:(NSString*)PORT;

-(void) sendToServer:(NSString*) msg; 

@end
