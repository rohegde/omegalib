//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreFoundation/CoreFoundation.h> 
#include <sys/socket.h> 
#include <netinet/in.h>

#import <Foundation/Foundation.h>
#if TARGET_OS_IPHONE
#import <CFNetwork/CFNetwork.h>
#endif
#include <sys/types.h>
#include <netdb.h>

#import "OmegaProtocol.h"

#define PORT 3490 // the port client will be connecting to 

#define IP_ADDRESS "192.168.1.100"    //Home
//#define IP_ADDRESS "131.193.77.197"   //EVL
//#define IP_ADDRESS "131.193.77.249"   //EVL LabPad
//#define IP_ADDRESS "131.193.77.222"   //myPad
//#define IP_ADDRESS "131.193.77.200"   //Alessandro's Machine
//#define IP_ADDRESS "131.193.78.108"   //Dual2.evl.uic.edu
//#define IP_ADDRESS "131.193.77.112"   //Thor.evl.uic.edu

#define DEBUG_CLIENT_DEFAULT YES;

typedef enum { CUA_BUTTON=0 , CUA_SLIDER=1  , CUA_TOGGLE=2 } CUATypes;

//TODO::Enum Fix:: Assumed that the states are ordered
typedef enum { CLIENT_INIT=0 , CLIENT_CONNECTED=1  , CLIENT_GUIRECVED=2 , CLIENT_IDLE=3 } ClientState;

@interface TCPClientOmega : UIView
{
    CFSocketRef serverSocket;
    ClientState myState;
    BOOL debugClient;
}

@property (assign) CFSocketRef serverSocket;
@property (assign) ClientState myState;
@property (assign) BOOL debugClient;


//GUI parsing   
-(void) handleGUIElementsFor:(int)GUIType in:(NSArray*)pieces;
-(void) determineGUISpec:(NSString*) GUISpec;

//Event Msg Sending 
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val;
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam;
-(NSString*) genEventMsgWith:(NSArray*)data param:(NSArray*)info;

//Connection mng
-(BOOL) estConnectionToServer;
void CallBackFunction(CFSocketRef, CFSocketCallBackType , CFDataRef , const void *, void *);

@end
