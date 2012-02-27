//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "TCPClientOmega.h"

//notification functions
@interface TCPClientOmega (private)
- (void) SendMsg:(NSNotification *) notification;
@end

@implementation TCPClientOmega

@synthesize myState;
@synthesize debugClient;
@synthesize inputStream;
@synthesize outputStream;

@synthesize byteData;
@synthesize byteLen;    

@synthesize readUpTo;
@synthesize readyForNewData;
@synthesize msgState;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) 
    {
        self.myState = CLIENT_INIT; 
        self.debugClient = DEBUG_CLIENT_DEFAULT;
        
        self.inputStream = nil;
        self.outputStream = nil;
        
        self.byteData = NULL;
        self.byteLen = -1;
        
        self.msgState = CLIENT_MSG_NONE;
        self.readUpTo = 0;
        self.readyForNewData = YES;
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SendMsg:) name:@"TCPClientOmegaSendMsg" object:nil];
    }
    return self;
}

#
#
#pragma mark Handle Notifications
#
#
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void) SendMsg:(NSNotification *) notification
{
    BOOL debugGUIMsg = YES;
    //Make sure it is the right notifcation
    if ([[notification name] isEqualToString:@"TCPClientOmegaSendMsg"])
    {
        //Get the object that the notifcation is sending 
        id object = [notification object];
        
        //If it is a string
        if( [object isKindOfClass:[NSString class] ] )
        {
            //report the string
            if( debugGUIMsg ) NSLog(@"%@" , object);        
            [self sendToServer:object];

        }
    }
        
}

#
#
#pragma mark Handle Connection
#
#
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (NSString*) determineIP
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *overrideIP = [ NSString stringWithString:[defaults stringForKey:@"overrideIP"]];
    if( [overrideIP isEqualToString:@""] ) //there is no overriding IP
    {
        NSString *knownIP = [ NSString stringWithString:[defaults stringForKey:@"knownIP"]];
        
        //Get the sections of the IP that is the address
        NSString* seperator = [NSString stringWithUTF8String:")"];
        NSArray *sections = [ knownIP componentsSeparatedByString:seperator ];
        //Know that it is always the last part : (name)              000.000.000.000
        knownIP = [sections objectAtIndex:1];
        knownIP = [knownIP stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        return knownIP;        
    }
    else // There is an overriding IP
    {
        overrideIP = [overrideIP stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];        
        return overrideIP;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(UInt32) determinePort
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *serverPort = [ NSString stringWithString:[defaults stringForKey:@"serverPort"]];
    UInt32 port = [serverPort intValue];
    return port;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) estConnectionToServer
{
    CFReadStreamRef readStream = nil;
    CFWriteStreamRef writeStream = nil;
    
    // CFStreamCreatePairWithSocketToHost helps us to bind two streams to a host and a port. 
    // Prepared the connection but there is no communication established. 

    NSString* IP = [self determineIP];
    CFStringRef CFIP = (__bridge CFStringRef) IP;
    UInt32 port = [ self determinePort];
    
    CFStreamCreatePairWithSocketToHost( NULL,  CFIP, port , &readStream, &writeStream);
    CFRelease(CFIP);

    inputStream = objc_unretainedObject(readStream);
    outputStream = objc_unretainedObject(writeStream);    
    
    [inputStream setDelegate:self];
	[outputStream setDelegate:self];
	[inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[inputStream open];
	[outputStream open];
    
    NSLog(@"OmegaClient connecting to %@ Port %lu ..... " , IP , port );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)stream:(NSStream *)theStream handleEvent:(NSStreamEvent)streamEvent 
{
    switch (streamEvent) 
    {
		case NSStreamEventOpenCompleted:
            if([theStream isEqual:inputStream] ) NSLog(@"\tOmegaClient :: Input Stream Opened.");            
            if([theStream isEqual:outputStream] ) NSLog(@"\tOmegaClient :: Output Stream Opened.");            
			break;
		case NSStreamEventHasBytesAvailable:
			if (theStream == inputStream) 
            {			
                bool debugByteInput = NO;

                Byte identBuffer[4];
				Byte lenBuffer[4];                
				NSUInteger read;
                
				while ([inputStream hasBytesAvailable]) //while there are bytes in the stream 
                {
                    switch (msgState) 
                    {
                        //If there is no msg being processed
                        case CLIENT_MSG_NONE:
                        {
                            //setup for new data
                            if( readyForNewData )
                            {
                                //get msg ident type from the first 4 bytes
                                read = [inputStream read:identBuffer maxLength:4];
                                NSString *ident = [[NSString alloc] initWithBytes:identBuffer length:4 encoding:NSUTF8StringEncoding];
                                if(debugByteInput)
                                {
                                    NSLog(@" " );
                                    NSLog(@"\tOmegaClient :: read : ident  : %@" , ident);   
                                }
                                
                                //ident matchs supported msgs
                                if( [ident isEqualToString:@"ipng"]) self.msgState = CLIENT_MSG_IMG;
                                else if( [ident isEqualToString:@"mgui"]) self.msgState = CLIENT_MSG_GUI;
                                else //if not return bc not support read
                                {
                                    NSLog(@"\tOmegaClient :: ERROR : msg recv'ed on the stream, but not supported msg type : %@ " , ident );
                                    return;
                                }
                                
                                self.readyForNewData = NO;       //currently working on an img so no new idents
                                self.readUpTo = 0;              //reset how much has been read 
                                
                                //Read the next 4 bytes for the length 
                                read = read + [inputStream read:lenBuffer maxLength:4];                                  
                                int length = (lenBuffer[0] << 24) + (lenBuffer[1] << 16) + (lenBuffer[2] << 8) + lenBuffer[3];
                                length = NSSwapBigIntToHost(length);
                                length = NSSwapLittleIntToHost(length);
                                self.byteLen = length;
                                if(debugByteInput) NSLog(@"\t\tread : length  : %d" , length);
                                
                                //setup the class level img buffer
                                self.byteData = (char*)malloc( self.byteLen );;
                                break;
                            }
                        }
                        case CLIENT_MSG_IMG:
                        case CLIENT_MSG_GUI:
                        {
                            //read in data to a local buffer
                            Byte imgBuffer[self.byteLen];                
                            read = [inputStream read:imgBuffer maxLength:self.byteLen - readUpTo];                    
                            if(debugByteInput) NSLog(@"\t\tread : data was read up to : %d bytes" , read);                            
                            
                            //append this to the class data buffer
                            for( int iByte = 0 ; iByte < read ; iByte++ )
                            {
                                self.byteData[readUpTo] = imgBuffer[iByte];
                                self.readUpTo++;
                            }
                            
                            //if received enough bytes process the msg                            
                            if( self.readUpTo == self.byteLen )
                            {
                                //If it is an img
                                if ( msgState == CLIENT_MSG_IMG )
                                {
                                    if(debugByteInput) NSLog(@"\t\tImg Data Recv");
                                    [[NSNotificationCenter defaultCenter] postNotificationName:@"DetailVCRedrawFIA" object:nil];
                                }
                                //If it is a gui                                
                                else if ( msgState == CLIENT_MSG_GUI )
                                {
                                    NSString *GUISpec =[NSString stringWithUTF8String:self.byteData];
                                    if(debugByteInput) NSLog(@"\t\tGUI Data Recv :");
                                    if(debugByteInput) NSLog(@"\t\t%s" , self.byteData);

                                    [[NSNotificationCenter defaultCenter] postNotificationName:@"SetupCUAFromGUISpec" object:GUISpec];
                                }

                                //reset flags and stuff
                                [self clearOutFlagsForByteData];
                            
                            }
                            break;
                        }
                        default:
                            NSLog(@"\tOmegaClient :: ERROR : there is a ClientMsgType that is not supported");
                            break;
                    }
				}
			}
			break;
            
			
		case NSStreamEventErrorOccurred:
            NSLog(@"\t\tERROR : Can not conenect to the host ...");
            NSLog(@"\t\t\t Check internet connection is working.");            
            NSLog(@"\t\t\t Check that the IP is set correctly under Settings-->Porthole");                        
            NSLog(@"\t\t\t Check that the Port is set correctly under Settings-->Porthole");      
			break;			
		case NSStreamEventEndEncountered:
            NSLog(@"\tOmegaClient :: Closing the input stream");
            [theStream close];
            [theStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            theStream = nil;			
			break;

        case NSStreamEventNone:
            break;
		case NSStreamEventHasSpaceAvailable:            

            break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) clearOutFlagsForByteData
{
    //Clear out image related data and flags
    free(byteData);
    byteData = NULL;
    byteLen = -1;
 
    //Clear out data related data and flags
    msgState = CLIENT_MSG_NONE;
    readyForNewData = YES;
    readUpTo = 0;
}


#
#
#pragma mark Handle Event Msg Generation
#
#

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) sendToServer:(NSString*)msg
{
    NSData *data = [[NSData alloc] initWithData:[msg dataUsingEncoding:NSASCIIStringEncoding]];
	[outputStream write:[data bytes] maxLength:[data length]];
}

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
        
        [self sendToServer:eventMsg];
        
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
    
    return eventMsg;
}

@end
