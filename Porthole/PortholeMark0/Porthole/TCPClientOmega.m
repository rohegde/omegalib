//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "TCPClientOmega.h"

@implementation TCPClientOmega

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) 
    {
        myState = CLINET_INIT; 
    }
    return self;
}

#
#
#pragma mark Handle Event Msg Generation
#
#

-(void) connection
{
    NSString* addy = [NSString stringWithUTF8String:IP_ADDRESS];
    NSString* port = [NSString stringWithUTF8String:PORT];
    
    [super setupTCPConnectionTo:addy at:port];
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam
{
    if( myState == CLINET_WAIT )
    {
        NSNumber *serviceN = [NSNumber numberWithInt:service];
        NSNumber *eventN = [NSNumber numberWithInt:event];        
        
        NSArray *eventData = [NSArray arrayWithObjects:serviceN, eventN, nil];
        
        NSString *eventMsg = [self genEventMsgWith:eventData param:eventParam];
        [super sendToServer:eventMsg];
    }
    return YES;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val
{
    if( myState == CLINET_WAIT )
    {
        NSNumber *serviceN = [NSNumber numberWithInt:service];
        NSNumber *eventN = [NSNumber numberWithInt:event];        
        NSNumber *srcIdN = [NSNumber numberWithInt:srcId];
        NSNumber *valN = [NSNumber numberWithFloat:val];        
        
        NSArray *eventData = [NSArray arrayWithObjects:serviceN , eventN , nil ];        
        NSArray *eventParam= [NSArray arrayWithObjects:srcIdN, valN , nil ];        
        NSString *eventMsg = [self genEventMsgWith:eventData param:eventParam];
        
        [super sendToServer:eventMsg];
    }
    return YES;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(NSString*) genEventMsgWith:(NSArray*)data param:(NSArray*)info
{
    NSString * spacer = [NSString stringWithUTF8String:":"];
    NSString * seperator = [NSString stringWithUTF8String:"|"];
    
    NSString *eventMsg = @"";
    
    //put in the data
    for( int iData = 0 ; iData < data.count ; iData++)
    {
        NSNumber *curData = [ data objectAtIndex: iData ];
        NSString *curDataString = [curData stringValue];
        
        eventMsg = [eventMsg stringByAppendingString:curDataString];
        eventMsg = [eventMsg stringByAppendingString:spacer];
    }
    
    //put in the parameters
    for( int iInfo = 0 ; iInfo < info.count ; iInfo++)
    {
        NSNumber *curInfo = [ info objectAtIndex: iInfo ];
        NSString *curInfoString = [curInfo stringValue];
        
        eventMsg = [eventMsg stringByAppendingString:curInfoString];
        eventMsg = [eventMsg stringByAppendingString:spacer];
    }
    eventMsg = [eventMsg stringByAppendingString:seperator];
    
    //Append the msg length in the beginning
//    int eventMsgLen = eventMsg.length;
//    NSString *eventLenS = [ NSString stringWithFormat:@"%d" , eventMsgLen];
//    eventMsgLen = eventMsgLen + eventLenS.length;
//    eventLenS = [ NSString stringWithFormat:@"%d" , eventMsgLen];
//    
//    eventLenS = [eventLenS stringByAppendingString:spacer];
//    eventLenS = [eventLenS stringByAppendingString:eventMsg];
    
    return eventMsg;
}


#
#
#pragma mark Handle GUI Parsing
#
#


////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) handleGUIElementsFor:(int)GUIType in:(NSArray*)pieces
{
    //Get all the other pieces
    int numOfElements = pieces.count - 2;   // 1 for the count and 1 for the end marker
    
    for( int iPiece = 1 ; iPiece < numOfElements ; iPiece++)
    {
        if( GUIType == CUA_SLIDER ) //group the source_id, label, min , max
        {
            NSString* id = [pieces objectAtIndex:iPiece];
            iPiece++;
            NSString* label = [pieces objectAtIndex:iPiece];
            iPiece++;
            NSString* min = [pieces objectAtIndex:iPiece];
            iPiece++;
            NSString* max = [pieces objectAtIndex:iPiece];  
            iPiece++;
            if(debugClient) NSLog(@"\t\t id:%@ label:%@ min:%@ max:%@" , id , label , min , max );
            
        }
        else    //group the source_id and label
        {
            NSString* id = [pieces objectAtIndex:iPiece];
            iPiece++;
            NSString* label = [pieces objectAtIndex:iPiece];
            
            if(debugClient) NSLog(@"\t\t id:%@ label:%@" , id , label);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) determineGUISpec:(NSString*) GUISpec
{
    int msgLength;
    
    if(debugClient) NSLog( @"GUISpec : %@ \n" , GUISpec );
    
    NSString* spacer = [NSString stringWithUTF8String:":"];
    NSString* seperator = [NSString stringWithUTF8String:"|"];
    
    //Get the sections of the GUISpec and put them in sections NSArray
    NSArray *pieces;
    NSArray *sections = [ GUISpec componentsSeparatedByString:seperator ];
    for ( int iSection = 0 ; iSection < 4 ; iSection++ )
    {
        NSString* curSection = [sections objectAtIndex:iSection];
        
        switch (iSection)
        {
            case 0: 
                msgLength = [curSection intValue];
                if(debugClient) NSLog(@"\t Msg Length : %i" , msgLength);
                break;
            default:
                //Grab all the pieces out of the section
                pieces = [ curSection componentsSeparatedByString:spacer];
                
                //First piece always tells you the number of elements
                if(debugClient) NSLog(@"\t Section : %i - has %@ elements" , iSection , [pieces objectAtIndex:0] );
                
                CUATypes GUIType;
                switch (iSection)
            {
                case 1:GUIType = CUA_BUTTON;break;
                case 2:GUIType = CUA_SLIDER;break;
                case 3:GUIType = CUA_TOGGLE;break;                    
            }
                
                [self handleGUIElementsFor:GUIType in:pieces];
                break;                
        }
    }
}

@end
