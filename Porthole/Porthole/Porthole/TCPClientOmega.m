//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "TCPClientOmega.h"

@implementation TCPClientOmega

@synthesize myState;
@synthesize debugClient;
@synthesize inputStream;
@synthesize outputStream;

@synthesize imgByteData;
@synthesize imgByteDataLength;    
@synthesize newImgByteData;

@synthesize TCPClientOmegaDelegate;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) 
    {
        myState = CLIENT_INIT; 
        debugClient = DEBUG_CLIENT_DEFAULT;
        
        inputStream = nil;
        outputStream = nil;
        
        imgByteData = NULL;
        imgByteDataLength = -1;

    }
    return self;
}



#
#
#pragma mark Handle Connection
#
#
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

-(UInt32) determinePort
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *serverPort = [ NSString stringWithString:[defaults stringForKey:@"serverPort"]];
    UInt32 port = [serverPort intValue];
    return port;
}

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
//    inputStream = ( __bridge NSInputStream *)readStream;
//    outputStream = ( __bridge NSOutputStream *)writeStream;

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
				uint8_t buffer[1024];
				int len;
				
				while ([inputStream hasBytesAvailable]) 
                {
					len = [inputStream read:buffer maxLength:sizeof(buffer)];
					if (len > 0) 
                    {
						NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
						if (nil != output) 
                        {
                            if(debugClient) NSLog(@"\tOmegaClient :: Server said : %@ " , output);
                            [self messageReceived:output];
						}
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


#
#
#pragma mark Handle Recieve Data from Sockets
#
#

-(void) clearOutOldImgByteData
{
    free(imgByteData);
    imgByteData = NULL;
    imgByteDataLength = -1;
    newImgByteData = NO;
}


- (void) messageReceived:(NSString *)message 
{
    [self clearOutOldImgByteData];
    //simulate getting the byte array of data 
    //static BOOL first = YES;
    //if( first )
    //{
        //Grab the png info into UIImage
        UIImage *img = [UIImage imageNamed:@"Tendrils.png"];
    
        //UIImage --> NSData
        NSData *imageData = UIImagePNGRepresentation(img);

        //Convert the NSData to Bytes array
        imgByteDataLength = [imageData length];
        imgByteData = (Byte*)malloc( imgByteDataLength );
        memcpy(imgByteData, [imageData bytes], imgByteDataLength);
     //   first = NO;
        newImgByteData = YES;
    //}
    
    NSLog(@"TCP :: Recieving data");
    [self.TCPClientOmegaDelegate flagRedraw:self];
    [self clearOutOldImgByteData];

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
    /*
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
     */
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) determineGUISpec:(NSString*) GUISpec
{
    /*
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
    */
}

@end
