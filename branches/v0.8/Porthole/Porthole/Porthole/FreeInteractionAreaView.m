//
//  FreeInteractionAreaView.m
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "FreeInteractionAreaView.h"

@implementation FreeInteractionAreaView

@synthesize markerView;
@synthesize lastScale;
@synthesize lastRotation;
@synthesize delegate;

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
- (id)initWithFrame:(CGRect)frame name:(NSString*)theName bounds:(CGRect)theBounds withTouch:(BOOL)touch withMultiTouch:(BOOL)mTouch
{
    self = [super initWithFrame:frame name:theName bounds:theBounds withTouch:touch withMultiTouch:mTouch];
    if (self) 
    {
        // Initialization code
        NSLog(@"FIA init\n");
        
        UIView *background = [[UIView alloc] initWithFrame:theBounds];
        [self addSubview:background];     
        
        [self setupLabel];
        [self setupThumbnail:frame];   

        
        if( touch )
        {
            [self setupMarkerWith:frame];   
        }
        if( mTouch )
        {
            lastScale = 1.0;
            lastRotation = 0.0;
            prevPt = CGPointMake(0.0 , 0.0);
        }
        myState = INIT;
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
    self.markerView.hidden = YES;
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
    stateIAV curState = [self.delegate getConnectionState:self];

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
    
    if( curState == NEW_MODEL )
    {
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
    [overlayView setNeedsDisplay];
    
}

// Only override drawRect: if you perform custom drawing.
- (void)drawRect:(CGRect)rect
{
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
    [self.markerView setNeedsDisplay];
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
        switch ([self.delegate getConnectionState:self] )
        {
            case INIT:
                if( debugTouch) NSLog(@"\tFIA : Touch : Connect");                
                [self.delegate connect:self];
                [self.delegate incrConnectionState:self];
                return;
                break;
            case CONNECTED:
                [self.delegate incrConnectionState:self];
                return;
                break;
            case NEW_MODEL:
                [self.delegate incrConnectionState:self];
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
        NSNumber *xLoc = [NSNumber numberWithFloat:localLoc.x];
        NSNumber *yLoc = [NSNumber numberWithFloat:localLoc.y]; 
        NSArray *param = [NSArray arrayWithObjects:xLoc , yLoc , nil];             
        
        //Send TCP msg 
        [self.delegate sendMsgAsService:Pointer event:Down param:param from:self];
        [self setNeedsDisplay];
    }
    [self setNeedsDisplay];
    
    
    
    
}
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
        CGPoint localPoint = [self localLocOf:prevPoint];
        NSNumber *localX = [NSNumber numberWithFloat:localPoint.x];
        NSNumber *localY = [NSNumber numberWithFloat:localPoint.y]; 
        NSArray *param = [NSArray arrayWithObjects:localX , localY , nil];             
        
        //Send TCP msg 
        [self.delegate sendMsgAsService:Pointer event:Move param:param from:self];
    }  
    [self setNeedsDisplay];
}
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
        NSNumber *xLoc = [NSNumber numberWithFloat:localLoc.x];
        NSNumber *yLoc = [NSNumber numberWithFloat:localLoc.y]; 
        NSArray *param = [NSArray arrayWithObjects:xLoc , yLoc , nil];             
        
        //Send TCP msg
        [self.delegate sendMsgAsService:Pointer event:Up param:param from:self];
        
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

-(void)genTCPMsgWithRecognizer:(UIGestureRecognizer *)recognizer Event:(int)event Param:(CGFloat)param
{
    //If multi-touch
    if(!multiTouchAble) return;
    
    //clear any old marker list
    [self clearMarkerLoc];
  
    //If done pinching reset the last pinch scale
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
        if( debugTouch ) NSLog(@"\%f , %f " , prevPt.x , prevPt.y);   

        NSArray *param = [NSArray arrayWithObjects:[NSNumber numberWithFloat:prevPt.x] , [NSNumber numberWithFloat:prevPt.y] , nil];
        [self.delegate sendMsgAsService:Pointer event:Up param:param from:self];
        
        //Reset at the end
        lastScale = 1.0;  
        lastRotation = 0.0;
        [self wipeMarkers];
        [self setNeedsDisplay];
        return;
    }
    
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
    
    NSNumber *paramN = [NSNumber numberWithFloat:param];
    
    switch (event) 
    {
        case Zoom:      if( debugTouch ) NSLog(@"\tFIA : Gesture : Pinch : Scale %.2f " , param);    
        case Rotate:    if( debugTouch ) NSLog(@"\tFIA : Gesture : Rotate : Angle %.2f " , param);    
        default:break;
    }
    
    //Add the scale to the NSMutableArray
    [paramMutable addObject:paramN];        
    
    //Transform the NSMutableArray to NSArray
    NSArray *paramArray = [NSArray arrayWithArray:paramMutable];
    
    //Send TCP msg
    [self.delegate sendMsgAsService:Pointer event:event param:paramArray from:self];
    
    [self setNeedsDisplay];
    
}


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
    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeDown:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeDown:recognizer];
    
    [self genTCPMsgWithRecognizer:recognizer Event:MoveDown Param:recognizer.direction];
    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeRight:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeRight:recognizer];    
    
    [self genTCPMsgWithRecognizer:recognizer Event:MoveRight Param:recognizer.direction];
    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeLeft:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeLeft:recognizer];        
    
    [self genTCPMsgWithRecognizer:recognizer Event:MoveLeft Param:recognizer.direction];
    
}
//----------------------------------------------------------------------------------------------------

@end