//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TCPClient.h"

#import "OmegaProtocol.h"

#define PORT "3490" // the port client will be connecting to 

//#define IP_ADDRESS "192.168.1.105"    //Home
//#define IP_ADDRESS "131.193.77.197"   //EVL
#define IP_ADDRESS "131.193.77.249"   //EVL LabPad
//#define IP_ADDRESS "131.193.77.200"   //Alessandro's Machine

typedef enum { CUA_BUTTON=0 , CUA_SLIDER=1  , CUA_TOGGLE=2 } CUATypes;

@interface TCPClientOmega : TCPClient
{
}

//GUI parsing
-(void) handleGUIElementsFor:(int)GUIType in:(NSArray*)pieces;
-(void) determineGUISpec:(NSString*) GUISpec;

//Event Msg Sending 
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val;
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam;
-(NSString*) genEventMsgWith:(NSArray*)data param:(NSArray*)info;

@end
