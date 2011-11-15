//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "TCPClientOmega.h"

@implementation TCPClientOmega

@synthesize serverSocket;
@synthesize myState;
@synthesize debugClient;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) 
    {
        myState = CLIENT_INIT; 
        debugClient = DEBUG_CLIENT_DEFAULT;
    }
    return self;
}

#
#
#pragma mark Handle Connection
#
#

-(BOOL) estConnectionToServer
{
    //Create a socket of with the specificed details
    CFAllocatorRef allocator = NULL;    
    SInt32 protocolFamily = PF_INET;
    SInt32 socketType = SOCK_STREAM;
    SInt32 protocol = IPPROTO_TCP;

    //kCFSocketNoCallBack       ::  No callback should be made for any activity.
    //kCFSocketReadCallBack     ::  The callback is called when data is available to be read or a new connection is waiting to be accepted. 
    //                              The data is not automatically read; the callback must read the data itself.
    //kCFSocketAcceptCallBack   ::  New connections will be automatically accepted and the callback is called with the data argument being
    //                              a pointer to a CFSocketNativeHandle of the child socket. This callback is usable only with listening sockets.
    //kCFSocketDataCallBack     ::  Incoming data will be read in chunks in the background and the callback is called with the data argument 
    //                              being a CFData object containing the read data.
    //kCFSocketConnectCallBack  ::  If a connection attempt is made in the background by calling CFSocketConnectToAddress or 
    //                              CFSocketCreateConnectedToSocketSignature with a negative timeout value, this callback type is made when the connect finishes. 
    //                              In this case the data argument is either NULL or a pointer to an SInt32 error code, if the connect failed. 
    //                              This callback will never be sent more than once for a given socket.
    //kCFSocketWriteCallBack    ::  The callback is called when the socket is writable. 
    //                              This callback type may be useful when large amounts of data are being sent rapidly over the socket and 
    //                              you want a notification when there is space in the kernel buffers for more data.
    CFOptionFlags callBackTypes = kCFSocketReadCallBack + kCFSocketDataCallBack + kCFSocketConnectCallBack;
    CFSocketCallBack callout = (CFSocketCallBack)CallBackFunction;
    char detail[] = "This is the socket for OmegaClient\n\r";
    CFSocketContext context = { 0, detail, NULL, NULL, NULL };
    serverSocket = CFSocketCreate ( allocator , protocolFamily , socketType , protocol , callBackTypes, callout, &context);
    
    if ( serverSocket == NULL )
    {
        NSLog(@"\tOmegaClient :: ERROR : socket creation error.");   
        return NO;
    }
    else
    {
        if( debugClient ) NSLog(@"\tOmegaClient : socket created");
    }
    
    //Set the port and address we want to listen on 
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr = INADDR_ANY;
    
    //Grab the host name of the specficed IP
    //Later need to change this to incorporate Bonjour so the IP does not need to be specified
    struct hostent *hp;
    hp = gethostbyname( IP_ADDRESS );
    if( hp == NULL )
    {
        NSLog(@"\tOmegaClient :: ERROR : can not resolve host name for IP : %s" , IP_ADDRESS);
        return NO;
    }
    memcpy( &addr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length );
    
    
    //Generate the CFDataRef for the connection address
    CFDataRef connectAddr = CFDataCreate(NULL, (unsigned char *)&addr, sizeof(addr));
    
    //Connect to the address
    CFSocketError error = CFSocketConnectToAddress(serverSocket, connectAddr, -1.0);
    switch (error)
    {
        case kCFSocketSuccess:
            if( debugClient ) NSLog(@"\tOmegaClient : socket connected.");
            myState = CLIENT_CONNECTED; 
            break;
        case kCFSocketError:
            NSLog(@"\tOmegaClient :: ERROR : socket connection error.");
            return NO;
            break;
        case kCFSocketTimeout:
            NSLog(@"\tOmegaClient :: ERROR : socket connection timeout");
            return NO;
            break;
    }
    
    CFRunLoopSourceRef sourceRef = CFSocketCreateRunLoopSource(kCFAllocatorDefault, serverSocket, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), sourceRef, kCFRunLoopCommonModes);
    CFRelease(sourceRef);
    CFRunLoopRun();

    return YES;
    
}

void CallBackFunction( CFSocketRef socket , CFSocketCallBackType type , CFDataRef address , const void *data , void *info)
{
    //    CFReadStreamRef readStream = NULL;
    //    CFWriteStreamRef writeStream = NULL;
    //    CFIndex bytes;
    UInt8 buffer[250];
    //    UInt8 recv_len = 0, send_len = 0;
    
    //The native socket, used for various operations
    CFSocketNativeHandle sock = CFSocketGetNative(socket);
    
    switch (type) {
        case kCFSocketReadCallBack:
            NSLog(@"\tOmegaClient :: socket read call back");
            break;
            
        case kCFSocketDataCallBack:
            NSLog(@"\tOmegaClient :: socket data call back");            
            break;
            
        case kCFSocketConnectCallBack:
            NSLog(@"\tOmegaClient :: socket connection call back");            
            break;
        default:
            break;
    }
    
    
    /* The joke we stored in the socket context */
    //    char *joke = info;
    
    //printf("OK. %s", joke);
    
    //send(sock, joke, strlen(joke)+1, 0);
    //recv(sock, buffer, sizeof(buffer), 0);
    //printf("%s", buffer);
    
    //close(sock);
    //exit(EXIT_SUCCESS);
}

#
#
#pragma mark Handle Event Msg Generation
#
#

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(BOOL) sendEventService:(int)service event:(int)event param:(NSArray*)eventParam
{
    if( myState == CLIENT_IDLE )
    {
        NSNumber *serviceN = [NSNumber numberWithInt:service];
        NSNumber *eventN = [NSNumber numberWithInt:event];        
        
        NSArray *eventData = [NSArray arrayWithObjects:serviceN, eventN, nil];
        
        NSString *eventMsg = [self genEventMsgWith:eventData param:eventParam];
//TODO::Network Port        [super sendToServer:eventMsg];
    }
    return YES;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(BOOL) sendEventService:(int)service event:(int)event sid:(int)srcId value:(float)val
{
    if( myState == CLIENT_IDLE )
    {
        NSNumber *serviceN = [NSNumber numberWithInt:service];
        NSNumber *eventN = [NSNumber numberWithInt:event];        
        NSNumber *srcIdN = [NSNumber numberWithInt:srcId];
        NSNumber *valN = [NSNumber numberWithFloat:val];        
        
        NSArray *eventData = [NSArray arrayWithObjects:serviceN , eventN , nil ];        
        NSArray *eventParam= [NSArray arrayWithObjects:srcIdN, valN , nil ];        
        NSString *eventMsg = [self genEventMsgWith:eventData param:eventParam];
        
//TODO::Network Port         [super sendToServer:eventMsg];
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
            
//TODO::GUI Spec fix           if(debugClient) NSLog(@"\t\t id:%@ label:%@ min:%@ max:%@" , id , label , min , max );
            
        }
        else    //group the source_id and label
        {
            NSString* id = [pieces objectAtIndex:iPiece];
            iPiece++;
            NSString* label = [pieces objectAtIndex:iPiece];
            
//TODO::GUI Spec fix            if(debugClient) NSLog(@"\t\t id:%@ label:%@" , id , label);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) determineGUISpec:(NSString*) GUISpec
{
    int msgLength;
    
//TODO::GUI Spec fix   if(debugClient) NSLog( @"GUISpec : %@ \n" , GUISpec );
    
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
//TODO::GUI Spec fix                if(debugClient) NSLog(@"\t Msg Length : %i" , msgLength);
                break;
            default:
                //Grab all the pieces out of the section
                pieces = [ curSection componentsSeparatedByString:spacer];
                
                //First piece always tells you the number of elements
//TODO::GUI Spec fix                if(debugClient) NSLog(@"\t Section : %i - has %@ elements" , iSection , [pieces objectAtIndex:0] );
                
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
