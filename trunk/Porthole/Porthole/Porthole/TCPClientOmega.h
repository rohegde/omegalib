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

#define DEBUG_CLIENT_DEFAULT YES;

//TODO::Enum Fix:: Assumed that the states are ordered
typedef enum { CLIENT_INIT=0 , CLIENT_CONNECTED=1  , CLIENT_GUIRECVED=2 , CLIENT_IDLE=3 } ClientState;

typedef enum { CLIENT_MSG_NONE=0 , CLIENT_MSG_IMG=1 , CLIENT_MSG_GUI=2 } ClientMsgType;


@class TCPClientOmega;

@interface TCPClientOmega : UIView <NSStreamDelegate>
{
    ClientState myState;

    BOOL debugClient;
    
    NSInputStream *inputStream;
    NSOutputStream *outputStream;

    char *byteData;
    int byteLen;    

    int readUpTo;
    BOOL readyForNewData;
    ClientMsgType msgState;
    
}
@property (assign) ClientState myState;

@property (assign) BOOL debugClient;

@property (assign) char *byteData;
@property (assign) int byteLen;    

@property (assign) int readUpTo;
@property (assign) BOOL readyForNewData;
@property (assign) ClientMsgType msgState;

@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;

//Event Msg Sending 
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val;
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam;
-(NSString*) genEventMsgWith:(NSArray*)data param:(NSArray*)info;
-(void) sendToServer:(NSString*)msg;
-(UInt32) determinePort;

//Connection mng
- (NSString*) determineIP;

- (void) estConnectionToServer;

//Msg recieved
- (void) clearOutFlagsForByteData;

//Notificaitons
- (void) UIElementMsg:(NSNotification *) notification;

@end
