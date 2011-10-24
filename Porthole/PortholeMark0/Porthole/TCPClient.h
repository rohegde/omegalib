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

#import "OmegaProtocol.h"

#define PORT "3490" // the port client will be connecting to 

#define DEFAUL_DEBUG_CLIENT    YES;

//#define IP_ADDRESS "192.168.1.105"    //Home
//#define IP_ADDRESS "131.193.77.197"   //EVL
#define IP_ADDRESS "131.193.77.249"   //EVL LabPad
//#define IP_ADDRESS "131.193.77.200"   //Alessandro's Machine

#define MAXDATASIZE 100 // max number of bytes we can get at once 

typedef enum { CLINET_INIT , CLINET_CONNECTED , CLINET_GUIRECVED } TCPClientState;
typedef enum { CUA_BUTTON=0 , CUA_SLIDER=1  , CUA_TOGGLE=2 } CUATypes;

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

-(NSString*) genEventMsgWith:(NSArray*)data param:(NSArray*)info;

-(void*)get_in_addrWith:(struct sockaddr*)sa;
-(void) setupTCPConnection;
-(void) determineGUISpec:(NSString*) GUISpec;
-(void) handleGUIElementsFor:(int)GUIType in:(NSArray*)pieces;


-(void) sendToServer:(NSString*) msg; 
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val;
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam;

@end
