//
//  FreeInteractionAreaView.m
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "FreeInteractionAreaView.h"

@implementation FreeInteractionAreaView

@synthesize overlayImg;
@synthesize overLayImgNew;

@synthesize markerView;

@synthesize lastScale;
@synthesize lastRotation;

@synthesize FIAdelegate;

#
#
#pragma mark Memory management
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)setFrame:(CGRect)frame
{
    [super setFrame:frame];       
    if( touchAble )[self.markerView setFrame:frame];
}
//----------------------------------------------------------------------------------------------------


#
#
#pragma mark View Setup
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////
//
- (id)initWithFrame:(CGRect)frame name:(NSString*)theName withTouch:(BOOL)touch withMultiTouch:(BOOL)mTouch
{
    self = [super initWithFrame:frame name:theName withTouch:touch withMultiTouch:mTouch];
    if (self) 
    {
        //Setup the background
        UIView *background = [[UIView alloc] initWithFrame:frame];
        [self addSubview:background];     
        
        //Setup the label
        [self setupLabel];

        //Setup the thumbnail view
        [self setupThumbnail:frame];   
        
        if( touch )
        {
            //setup the marker view
            [self setupMarkerWith:frame];   
        }
        if( mTouch )
        {
            lastScale = 1.0;
            lastRotation = 0.0;
            prevPt = CGPointMake(0.0 , 0.0);
        }
        myState = INIT;
        
        NSLog(@"Free Interaction Area Loaded ..... \n");
    }
    return self;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
-(void)setupLabel
{   
    CGFloat desiredW = self.bounds.size.width;
    CGFloat desiredH = self.bounds.size.height * 0.1;    
    CGRect labelBounds = [ self localCoordCenterWith: desiredW and: desiredH];
    
    
    UILabel *label = [[UILabel alloc] initWithFrame:labelBounds];
    //label.text = self.myName;    
    label.text = @"";    
    label.textAlignment = UITextAlignmentCenter;
    label.textColor = [UIColor yellowColor];
    label.shadowColor = [UIColor whiteColor];
    label.shadowOffset = CGSizeMake(1,1);
    label.font = [UIFont fontWithName:@"Helvetica" size:20];
    label.backgroundColor = [UIColor clearColor];
    [self addSubview:label];
    myLabel = label; 
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
-(void)setupThumbnail:(CGRect)frame
{
    overlayImg = nil;
    overLayImgNew = NO;
    overlayView = [ [UIImageView alloc] initWithFrame:frame];
    overlayView.hidden = YES;
    [self addSubview:overlayView];     
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
-(void)setupMarkerWith:(CGRect)frame;
{
    [self clearMarkerLoc ];
    
    PulseCircleView *newMarker = [[PulseCircleView alloc] initWithFrame:frame];
    self.markerView = newMarker;
    self.markerView.hidden = NO;
    self.markerView.PCVDelegate = self;  
    [self addSubview:markerView];     

}


#
#
#pragma mark - Draw functions
#
#

-(void) makeLabelWithString:(NSString*) msg
{
    myLabel.text = msg;
    myLabel.textColor = [UIColor whiteColor];
    myLabel.shadowColor = [UIColor blackColor];
    myLabel.shadowOffset = CGSizeMake(1,1);
    myLabel.font = [UIFont fontWithName:@"Helvetica" size:20];
    myLabel.backgroundColor = [UIColor clearColor];
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
-(void)genThumbnail
{
    stateIAV curState = [self.FIAdelegate getConnectionState:self];

    //Draw the text instructions needed
    switch (curState)
    {
        case INIT:
            overlayView.hidden = YES;
            [self makeLabelWithString:@"Waiting for connection to app server. Touch to Continue..."];
            return;
            break;
            
            
        case CONNECTED:
            overlayView.hidden = YES;
            [self makeLabelWithString:@"Connection to app server established. Touch to Continue..."];
            return;
            break;

        case NEW_MODEL:
            [self makeLabelWithString:@"New Model Loaded <insert img here>. Touch to Continue..."];
            break;

        case SAME_MODEL:
            [self makeLabelWithString:@""];
            break;
        default:
            break;
    }
    
    //If it is a new model draw the overlay of instructions
    if( curState == NEW_MODEL )
    {
        //TODO :: Test Img NSLog(@"FIA :: Drawing Overlay for instructions");
        //Grab the png info into UIImage
        UIImage *img = [UIImage imageNamed:@"Oscar.png"];
    
        //Resize the img
        CGSize size = self.bounds.size;
        UIImage *imgResized = [img resizedImage:size interpolationQuality:kCGInterpolationDefault];
    
        //UIImage --> NSData
        NSData *imageData = UIImagePNGRepresentation(imgResized);
        
        //Convert the NSData to Bytes array
        NSUInteger len = [imageData length];
        Byte *byteData = (Byte*)malloc(len);
        memcpy(byteData, [imageData bytes], len);
    
        //Convert byte Array to NSData
        NSData *imgDataRecv = [ NSData dataWithBytes:byteData length:len ];
    
        //Convert NSData to UIImage
        UIImage *imgRecv = [UIImage imageWithData:imgDataRecv];
        [overlayView setImage:imgRecv];
        overlayView.hidden = NO;
    }
    //If it is the same model draw the thumbnail
    if( curState == SAME_MODEL )
    {
        if( self.overlayImg != nil && self.overLayImgNew)
        {
            //TODO :: Test Img NSLog(@"FIA :: Drawing new img byte data");            
            [overlayView setImage:self.overlayImg];
            overlayView.hidden = NO;                
        }
        else 
        {
            
            //TODO :: Test Img NSLog(@"FIA :: Not drawing shit");            
        }
    }
    [overlayView setNeedsDisplay];
}

// Only override drawRect: if you perform custom drawing.
- (void)drawRect:(CGRect)rect
{
    //TODO :: Test Img NSLog(@" ");
    //TODO :: Test Img NSLog(@"FIA :: drawRect Called");

    [self genThumbnail ];
    if( self.touchAble && markerLoc != nil)
    {
        self.markerView.hidden = NO;
        [self.markerView setNeedsDisplay];  
    }
}

#
#
#pragma mark PulseCircleView Delegate Functions
#
#

-(NSArray*)markersForPulseCircleView:(PulseCircleView*) requestor
{
    if (requestor == self.markerView && self.markerLoc != nil) 
    {
        return self.markerLoc;
    }
    else return nil;
}

#
#
#pragma mark All touch functions
#
#
-(void)wipeMarkers
{
    [self clearMarkerLoc];
    self.markerView.hidden = YES;
    [self setNeedsDisplay];
}

#
#
#pragma mark - Needed for simple touch interaction
#
#

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [super touchesBegan:touches withEvent:event ];        
    
    //If touchable
    if(!touchAble) return;
    
    if( debugTouch) NSLog(@"\tFIA : Touch : A touch has begun");
    
    //Grab the touch data
    UITouch *touch = [[event allTouches] anyObject];
    if ([touch tapCount] == 2) 
    {
        
    }    
    else
    {
        switch ([self.FIAdelegate getConnectionState:self] )
        {
            case INIT:
                if( debugTouch) NSLog(@"\tFIA : Touch : Connect");                
                [self.FIAdelegate connect:self];
                [self.FIAdelegate incrConnectionState:self];
                return;
                break;
            case CONNECTED:
                [self.FIAdelegate incrConnectionState:self];
                return;
                break;
            case NEW_MODEL:
                [self.FIAdelegate incrConnectionState:self];
                return;
                break;
            default:
                break;
        }
        
        //Grab the point
        CGPoint point = [ touch locationInView:self];
        
        //Check touch has not moved out of area
        if( ![self within:point.x and:point.y] ) return;
        
        //Populate markLoc - There is only one touch so this should be easy
        [self markerLocAddOne:point];
        
        if( debugTouch) NSLog(@"\tFIA : Touch : Touch Began at : %.2f,%.2f", point.x , point.y );            
        
        //Place localized x and y location into NSArray
        CGPoint localLoc = [self localLocOf:point];
//        NSNumber *xLoc = [NSNumber numberWithFloat:localLoc.x];
//        NSNumber *yLoc = [NSNumber numberWithFloat:localLoc.y]; 
//        NSArray *param = [NSArray arrayWithObjects:xLoc , yLoc , nil];             
//        //Send TCP msg 
//        [self.FIAdelegate sendMsgAsService:Pointer event:Down param:param from:self];
        
        //Gen and store the TCP for a touch         
        NSMutableString* msg = [NSMutableString stringWithFormat:@"t:%d:%f:%f:|", Down , localLoc.x , localLoc.y];
    
        [[NSNotificationCenter defaultCenter] postNotificationName:@"TCPClientOmegaSendMsg" object:[NSString stringWithString: msg]];

        
        [self setNeedsDisplay];
    }
    
}//- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 

//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{	
    [super touchesBegan:touches withEvent:event ];        
    
    //If touchable
    if(!touchAble) return;
    
    //Grab the touch data
    UITouch *touch = [touches anyObject];
    
    if( touches.count > 2 )
    {
        if( debugTouch) NSLog(@"\tFIA : Touch : Touch moved : too many touches " );      
        return;
    }
    
    if( (touch.phase != UITouchPhaseBegan))
    {
        //Grab the point        
        CGPoint point = [touch locationInView:self];
        
        //Check touch has not moved out of area
        if( ![self within:point.x and:point.y] ) return;
        //Populate markLoc array
        [self markerLocAddOne:point];
        
        //Grab previous point data
        CGPoint prevPoint = [touch previousLocationInView:self];
        
        if( debugTouch) NSLog(@"\tFIA : Touch : Touch moved : %.2f , %.2f from: %.2f , %.2f " , point.x , point.y , prevPoint.x , prevPoint.y );      
        
        //Place localized x and y location into NSArray
        CGPoint localLoc = [self localLocOf:prevPoint];
//        NSNumber *localX = [NSNumber numberWithFloat:localPoint.x];
//        NSNumber *localY = [NSNumber numberWithFloat:localPoint.y]; 
//        NSArray *param = [NSArray arrayWithObjects:localX , localY , nil];             
//        
//        //Send TCP msg 
//        [self.FIAdelegate sendMsgAsService:Pointer event:Move param:param from:self];

        //Gen and store the TCP for a touch         
        NSMutableString* msg = [NSMutableString stringWithFormat:@"t:%d:%f:%f:|", Move , localLoc.x , localLoc.y];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"TCPClientOmegaSendMsg" object:[NSString stringWithString: msg]];

    
    }  
    [self setNeedsDisplay];
}//- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
//----------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{	
    [super touchesBegan:touches withEvent:event ];        
    
    //If touchable
    if(!touchAble) return;
    
	//Grab the touch data
    UITouch *touch = [[event allTouches] anyObject];
    
    if ([touch tapCount] == 2) 
    {
        //if( debugTouch ) NSLog(@"\tFIA : Touch : A double touch has ended");
    }
    else
    {
        //Grab the point
        CGPoint point = [touch locationInView:self];
        
        //Check within
        if( ![self within:point.x and:point.y] ) return;
        
        if( debugTouch ) NSLog(@"\tFIA : Touch : A single touch has ended");        
        
        //Place localized x and y location into NSArray
        CGPoint localLoc = [self localLocOf:point];
        
//        NSNumber *xLoc = [NSNumber numberWithFloat:localLoc.x];
//        NSNumber *yLoc = [NSNumber numberWithFloat:localLoc.y]; 
//        NSArray *param = [NSArray arrayWithObjects:xLoc , yLoc , nil];             
//        [self.FIAdelegate sendMsgAsService:Pointer event:Up param:param from:self];
        
        //Gen and store the TCP for a touch 
        NSMutableString* msg = [NSMutableString stringWithFormat:@"t:%d:%f:%f:|", Up , localLoc.x , localLoc.y];        

        [[NSNotificationCenter defaultCenter] postNotificationName:@"TCPClientOmegaSendMsg" object:[NSString stringWithString: msg]];
        
        //Clear the touches in the markLoc array
        [self wipeMarkers];
    }    
    [self setNeedsDisplay];    
    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [super touchesBegan:touches withEvent:event ];
    if( debugTouch && multiTouchAble ) NSLog(@"\tFIA : Touch : A touch event has been canceled");        
    
    //Clear the touches in the markLoc array
    [self wipeMarkers];
    
    [self setNeedsDisplay];
}
//----------------------------------------------------------------------------------------------------


#
#
#pragma mark - Callback for gesture interaction
#
#
//Gestures usually have two fingers and some parameters
//  Need to Send 
//      the two fingers x1,y1 and x2,y2 
//      parameters that go along with these two fingers 
//  param is the extra data that comes with the gesture
//  TODO :: ctrl-f to find them


-(void)genTCPMsgWithRecognizer:(UIGestureRecognizer *)recognizer Event:(int)event Param:(CGFloat)param
{
    //If multi-touch
    if(!multiTouchAble) return;
    
    //clear any old marker list
    [self clearMarkerLoc];
  
    //If there was a gesture ended do somehting and return
    if(recognizer.state ==UIGestureRecognizerStateEnded)
    {
        switch (event) 
        {
            case Zoom:      if( debugTouch ) NSLog(@"\tFIA : Gesture : Pinch ended"); break;
            case Rotate:    if( debugTouch ) NSLog(@"\tFIA : Gesture : Rotation ended"); break;
            case MoveUp:    if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Up ended"); break;            
            case MoveDown:  if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Down ended"); break;            
            case MoveLeft:  if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Left ended"); break;            
            case MoveRight: if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Right ended"); break;                        
            case Select:    if( debugTouch ) NSLog(@"\tFIA : Gesture : 1 finger 2 tap ended"); break;                        
            default:break;
        }
        if( debugTouch ) NSLog(@"\t\t%f , %f " , prevPt.x , prevPt.y);   

//        NSArray *param = [NSArray arrayWithObjects:[NSNumber numberWithFloat:prevPt.x] , [NSNumber numberWithFloat:prevPt.y] , nil];
//        [self.FIAdelegate sendMsgAsService:Pointer event:Up param:param from:self];

        //Gen and store the TCP for a touch  
        NSMutableString* msg = [NSMutableString stringWithFormat:@"t:%d:%f:%f:|", Up , prevPt.x , prevPt.y];        

        [[NSNotificationCenter defaultCenter] postNotificationName:@"TCPClientOmegaSendMsg" object:[NSString stringWithString: msg]];
        
        //Reset at the end
        lastScale = 1.0;  
        lastRotation = 0.0;
        [self wipeMarkers];
        [self setNeedsDisplay];
        return;
    }
    
    //If there was a gesture beginning
    else 
    {
        switch (event) 
        {
            case Zoom:      if( debugTouch ) NSLog(@"\tFIA : Gesture : Pinch"); break;
            case Rotate:    if( debugTouch ) NSLog(@"\tFIA : Gesture : Rotation"); break;
            case MoveUp:    if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Up"); break;            
            case MoveDown:  if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Down"); break;            
            case MoveLeft:  if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Left"); break;            
            case MoveRight: if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe Right"); break;                        
            case Select:    if( debugTouch ) NSLog(@"\tFIA : Gesture : 1 finger 2 tap"); break;                        
            default:break;
        }

        //Grab the x,y info for the two points into a mutableArray 
        int numTouches = recognizer.numberOfTouches;
        NSMutableArray *paramMutable = [NSMutableArray arrayWithCapacity:numTouches];
        for ( int curPoint = 0 ; curPoint < numTouches ; curPoint++)
        {
            //Grab the point
            CGPoint point = [recognizer locationOfTouch:curPoint inView:self]; 
            
            //Check within
            if( ![self within:point.x and:point.y] ) return;
            [self markerLocAdd:point];
            
            //Place localized x and y location into NSArray
            CGPoint localLoc = [self localLocOf:point];
            NSNumber *xLoc = [NSNumber numberWithFloat:localLoc.x];
            NSNumber *yLoc = [NSNumber numberWithFloat:localLoc.y]; 
            [paramMutable addObject:xLoc];
            [paramMutable addObject:yLoc];        
            if( debugTouch ) NSLog(@"\t\t@ : %.2f(%.2f), %.2f(%.2f) " , point.x , localLoc.x, point.y , localLoc.y);  
            prevPt = localLoc;
        }

        //grab the extra data that was passed in
        NSNumber *paramN = [NSNumber numberWithFloat:param];
        //Add the extra data to the NSMutableArray
        [paramMutable addObject:paramN];        

        switch (event) 
        {
            case Zoom:      if( debugTouch ) NSLog(@"\tFIA : Gesture : Pinch : Scale %.2f " , param);    break;
            default:break;
        }

//        //Transform the NSMutableArray to NSArray 
//        NSArray *paramArray = [NSArray arrayWithArray:paramMutable];
//        ////Send TCP msg
//        [self.FIAdelegate sendMsgAsService:Pointer event:event param:paramArray from:self];

        //Gen and store the TCP msg for this gesture 
        NSMutableString* msg = [NSMutableString stringWithFormat:@"t:%d:", event];    //indicate gesture and type of gesture
        
        //load the two fingers x1,y1 and x2,y2 and parameters that go along with these two fingers 
        for( int iParam = 0 ; iParam < paramMutable.count ; iParam++)
        {
            [msg appendString:[NSString stringWithFormat:@"%f:", [paramMutable objectAtIndex:iParam] ]];
        }
        [msg appendString:[NSString stringWithString:@"|"]];

        [[NSNotificationCenter defaultCenter] postNotificationName:@"TCPClientOmegaSendMsg" object:[NSString stringWithString: msg]];
    }
    [self setNeedsDisplay];
    
}
//-(void)genTCPMsgWithRecognizer:(UIGestureRecognizer *)recognizer Event:(int)event Param:(CGFloat)param


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)handlePinch:(UIPinchGestureRecognizer *) recognizer
{
    [super handlePinch:recognizer];
    
    //Calcuations the Scale
    CGFloat scale = 1.0 - ( lastScale - recognizer.scale);
    lastScale = scale;
    
    [self genTCPMsgWithRecognizer:recognizer Event:Zoom Param:scale];
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)handlePan:(UIPanGestureRecognizer *) recognizer
{
    [super handlePan:recognizer];        
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Pan");            
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)handleRotation:(UIRotationGestureRecognizer *) recognizer
{
    [super handleRotation:recognizer];        
    
    //Grab the rotation
    lastRotation = lastRotation+recognizer.rotation; 
    if( debugTouch ) NSLog(@"\t\tAngle in radians : %.2f " , lastRotation);   
    
    [self genTCPMsgWithRecognizer:recognizer Event:Rotate Param:lastRotation];
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)oneFingerTwoTaps:(UITapGestureRecognizer *) recognizer
{
    [super oneFingerTwoTaps:recognizer];        
    
    [self genTCPMsgWithRecognizer:recognizer Event:Select Param:recognizer.numberOfTapsRequired];
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeUp:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeUp:recognizer];

    [self genTCPMsgWithRecognizer:recognizer Event:MoveUp Param:recognizer.direction];
    NSLog(@"FIA :: Swipe Left");    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeDown:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeDown:recognizer];
    
    [self genTCPMsgWithRecognizer:recognizer Event:MoveDown Param:recognizer.direction];
    NSLog(@"FIA :: Swipe down");
    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeRight:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeRight:recognizer];    
    
    [self genTCPMsgWithRecognizer:recognizer Event:MoveRight Param:recognizer.direction];
    NSLog(@"FIA :: Swipe Right");    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeLeft:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeLeft:recognizer];        
    
    [self genTCPMsgWithRecognizer:recognizer Event:MoveLeft Param:recognizer.direction];
    NSLog(@"FIA :: Swipe Left");
}
//----------------------------------------------------------------------------------------------------

@end
