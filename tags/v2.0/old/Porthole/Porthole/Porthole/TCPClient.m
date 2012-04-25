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
-(void) setupTCPConnectionTo:(NSString*)IP at:(NSString*)PORT
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
    
    
    const char *ipAddress = [IP UTF8String];  
    const char *port = [PORT UTF8String];      
    if ((rv = getaddrinfo(ipAddress, port, &hints, &servinfo)) != 0) 
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
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) sendToServer:(NSString*) msg 
{
    if(debugClient) NSLog(@"\t\tTCPSending : %@ " , msg);
    const char *msgCstr = [msg UTF8String];        
    if( send(connectionSocket , msgCstr , msg.length , 0) == -1 )perror( "send" );
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
