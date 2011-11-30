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

typedef enum { CUA_BUTTON=0 , CUA_SLIDER=1  , CUA_TOGGLE=2 } CUATypes;

//TODO::Enum Fix:: Assumed that the states are ordered
typedef enum { CLIENT_INIT=0 , CLIENT_CONNECTED=1  , CLIENT_GUIRECVED=2 , CLIENT_IDLE=3 } ClientState;

@class TCPClientOmega;

@protocol TCPClientOmegaDelegate
- (void) flagRedraw:(TCPClientOmega*)requestor;
@end

@interface TCPClientOmega : UIView <NSStreamDelegate>
{
    ClientState myState;
    BOOL debugClient;
    
    NSInputStream *inputStream;
    NSOutputStream *outputStream;

    Byte *imgByteData;
    NSUInteger imgByteDataLength;    
    BOOL newImgByteData;

    id <TCPClientOmegaDelegate> __unsafe_unretained TCPClientOmegaDelegate;

}
@property (assign) ClientState myState;
@property (assign) BOOL debugClient;

@property (assign) Byte *imgByteData;
@property (assign) NSUInteger imgByteDataLength;    
@property (assign) BOOL newImgByteData;    

@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;

@property (unsafe_unretained) id <TCPClientOmegaDelegate> TCPClientOmegaDelegate;


//GUI parsing   
-(void) handleGUIElementsFor:(int)GUIType in:(NSArray*)pieces;
-(void) determineGUISpec:(NSString*) GUISpec;

//Event Msg Sending 
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val;
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam;
-(NSString*) genEventMsgWith:(NSArray*)data param:(NSArray*)info;
-(void) sendToServer:(NSString*)msg;
-(UInt32) determinePort;

//Connection mng
- (NSString*) determineIP;

- (void) estConnectionToServer;

//Img Msg recieved
- (void) messageReceived:(NSString *)message;
-(void) clearOutOldImgByteData;

//- (void) callbackFuncFor:(CFSocketRef)socket callType:(CFSocketCallBackType)type withAddress:(CFDataRef)addy withData:(const void *)data withInfo:(void*)info;
//void CallBackFunction( CFSocketRef socket , CFSocketCallBackType type , CFDataRef address , const void *data , void *info);

@end
