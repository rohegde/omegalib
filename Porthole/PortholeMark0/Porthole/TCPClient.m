//
//  TCPClient.m
//  Porthole
//
//  Created by Dennis Chau (Koracas) on 10/23/11.
//  Copyright (c) 2011 All rights reserved.
//

#import "TCPClient.h"

@implementation TCPClient

@synthesize myState;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) 
    {
        debugClient = DEFAUL_DEBUG_CLIENT;
        [self setupTCPConnection];
    }
    return self;
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
/*
- (void)drawRect:(CGRect)rect
{
    NSLog(@"I am here");
}
*/

#
#
#pragma mark Memory management
#
#
-(void)dealloc
{
    close(connectionSocket);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
////
//// get sockaddr, IPv4 or IPv6:
//void *get_in_addr(struct sockaddr *sa)
//{
//    if (sa->sa_family == AF_INET) {
//        return &(((struct sockaddr_in*)sa)->sin_addr);
//    }
//    
//    return &(((struct sockaddr_in6*)sa)->sin6_addr);
//}
-(void*)get_in_addrWith:(struct sockaddr*)sa
{
    if (sa->sa_family == AF_INET) 
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) setupTCPConnection
{
    if( debugClient ) NSLog(@"TCPClient : client connection est.");    

    myState = CLINET_INIT;
   
    char buf[MAXDATASIZE];    
    long numbytes;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((rv = getaddrinfo(IP_ADDRESS, PORT, &hints, &servinfo)) != 0) 
    {
        NSString *error = [NSString stringWithFormat:@"getaddrinfo: %s\n", gai_strerror(rv)];
        [self generateErrorAlert:error];
        exit(1);
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((connectionSocket = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
            if( debugClient ) NSLog(@"Client : socket");
            continue;
        }
        if (connect(connectionSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(connectionSocket);
            if( debugClient ) NSLog(@"Client : connection");
            continue;
        }
        break;
    }
    
    if (p == NULL) {
        if( debugClient ) NSLog(@"client: failed to connect");
        exit(1);
    }
    else
    {
        struct sockaddr* sa = ((struct sockaddr *)p->ai_addr);
        inet_ntop(p->ai_family, [self get_in_addrWith:(sa)] , s , sizeof s );
        NSString *msg = [NSString stringWithFormat:@"Connecting to %s\n", s];
        [self generateDebugAlert:msg];
        myState = CLINET_CONNECTED;
    }        
    freeaddrinfo(servinfo); // all done with this structure
    

    /*
    while (1)
    {
        if ((numbytes = recv(connectionSocket, buf, MAXDATASIZE-1, 0)) == -1) 
        {
            if( debugClient ) NSLog(@"recv message");
            exit(1);
        }
        if( numbytes > 0 )
        {
            buf[numbytes] = '\0';
            char *buffer = buf;
            NSString* GUISpec = [NSString stringWithUTF8String:buffer];
            [self determineGUISpec:GUISpec];

//            [self generateDebugAlert:@"Received GUI spec."];
            
            myState = CLINET_GUIRECVED;
            break;
        }                
    }
     */
    myState = CLINET_GUIRECVED;

}

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam
{
    if( myState == CLINET_GUIRECVED )
    {
        NSNumber *serviceN = [NSNumber numberWithInt:service];
        NSNumber *eventN = [NSNumber numberWithInt:event];        
        
        NSArray *eventData = [NSArray arrayWithObjects:serviceN, eventN, nil];
    
        NSString *eventMsg = [self genEventMsgWith:eventData param:eventParam];
        const char *eventMsgCstr = [eventMsg UTF8String];        
        if( send(connectionSocket , eventMsgCstr , eventMsg.length , 0) == -1 )perror( "send" );
    }
    return YES;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val
{
    if( myState == CLINET_GUIRECVED )
    {
        NSNumber *serviceN = [NSNumber numberWithInt:service];
        NSNumber *eventN = [NSNumber numberWithInt:event];        
        NSNumber *srcIdN = [NSNumber numberWithInt:srcId];
        NSNumber *valN = [NSNumber numberWithFloat:val];        

        NSArray *eventData = [NSArray arrayWithObjects:serviceN , eventN , nil ];        
        NSArray *eventParam= [NSArray arrayWithObjects:srcIdN, valN , nil ];        
        NSString *eventMsg = [self genEventMsgWith:eventData param:eventParam];
        
        const char *eventMsgCstr = [eventMsg UTF8String];        
        if( send(connectionSocket , eventMsgCstr , eventMsg.length , 0) == -1 )perror( "send" );
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
    
    int eventMsgLen = eventMsg.length;
    NSString *eventLenS = [ NSString stringWithFormat:@"%d" , eventMsgLen];
    eventMsgLen = eventMsgLen + eventLenS.length;
    eventLenS = [ NSString stringWithFormat:@"%d" , eventMsgLen];

    eventLenS = [eventLenS stringByAppendingString:spacer];
    eventLenS = [eventLenS stringByAppendingString:eventMsg];
    
    return eventLenS;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) sendToServer:(NSString*) msg 
{
    if( debugClient ) NSLog( @"%@" , msg );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) generateDebugAlert:(NSString*) msg
{
    if( debugClient)
    {
        //Create the alert message
        NSString *title = [NSString stringWithFormat:@"TCPClient"];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message: msg delegate:self cancelButtonTitle:@"Close" otherButtonTitles:nil];    
        [alert show];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) generateErrorAlert:(NSString*) msg
{
    //Create the alert message
    NSString *title = [NSString stringWithFormat:@"TCPClient Error!!!"];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message: msg delegate:self cancelButtonTitle:@"Close" otherButtonTitles:nil];    
    [alert show];
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) closeSocketToServer
{
    if( myState == CLINET_GUIRECVED )
    {
        NSString *eventMsg = @"CommunicationComplete";        
        const char *eventMsgCstr = [eventMsg UTF8String];        
        if( send(connectionSocket , eventMsgCstr , eventMsg.length , 0) == -1 )perror( "send" );
        close(connectionSocket);
        myState = CLINET_INIT;
    }
    if( debugClient ) NSLog(@"TCPClient : close connection");        

}


@end
