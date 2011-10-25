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
        [self setupOverlay];

        if( touch )[self setupMarkerWith:frame];
        if( mTouch )
        {
            lastScale = 1.0;
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
-(void)setupOverlay
{
    CGSize size = self.bounds.size;
    
    UIImage *overlayImage;
    overlayImage = [ UIImage imageNamed:@"overlay.png" ];
    UIImage *overlayImgResized = [overlayImage resizedImage:size interpolationQuality:kCGInterpolationDefault];
    
    overlayView = [ [ UIImageView alloc ] initWithImage: overlayImgResized];
    overlayView.hidden = YES;
    [overlayView setNeedsDisplay];
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
    
    [self.markerView setNeedsDisplay];
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
}

- (void)drawInit
{
    overlayView.hidden = YES;
    [self makeLabelWithString:@"OmegaViewer Server Started"];
}

- (void)drawConnected
{
    overlayView.hidden = NO;
    [self makeLabelWithString:@"Connection to OmegaViewer Established"];
}

- (void)drawNewModel

{
    overlayView.hidden = YES;
    [self makeLabelWithString:@"Model Loaded"];
}

- (void)drawSameModel
{
    overlayView.hidden = YES;    
    [self makeLabelWithString:@""];
}

// Only override drawRect: if you perform custom drawing.
- (void)drawRect:(CGRect)rect
{
    /*
     switch (myState) 
    {
        case INIT: [self drawInit]; break;
        case CONNECTED: [self drawConnected]; break;
        case NEW_MODEL: [self drawNewModel]; break;    
        case SAME_MODEL: [self drawSameModel]; break;       
        default: break;
    }
    */
    
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

         if( myState == INIT ) myState = CONNECTED;
         else if( myState == CONNECTED ) myState = NEW_MODEL;
         else if( myState == NEW_MODEL ) myState = SAME_MODEL;
         else if( myState == SAME_MODEL ) myState = INIT;
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
        if( debugTouch ) NSLog(@"\tFIA : Touch : A double touch has ended");
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)handlePinch:(UIPinchGestureRecognizer *) recognizer
{
    [super handlePinch:recognizer];
    
    //If multi-touch
    if(!multiTouchAble) return;
    
    //clear any old marker list
    [self clearMarkerLoc];
    
    //Calcuations the Scale
    CGFloat scale = 1.0 - ( lastScale - recognizer.scale);
    lastScale = scale;
    NSNumber *scaleN = [NSNumber numberWithFloat:scale];
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Pinch : Scale %.2f " , scale);    
    
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
    }
    
    //Add the scale to the NSMutableArray
    [paramMutable addObject:scaleN];        
    //Transform the NSMutableArray to NSArray
    NSArray *param = [NSArray arrayWithArray:paramMutable];

    //Send TCP msg
    [self.delegate sendMsgAsService:Pointer event:Zoom param:param from:self];
    
    [self setNeedsDisplay];
    
    //If done pinching reset the last pinch scale
    if(recognizer.state ==UIGestureRecognizerStateEnded)
    {
        if( debugTouch ) NSLog(@"\tFIA : Gesture : Pinching ended");        
        lastScale = 1.0;            //Reset at the end
        [self wipeMarkers];
    }
    
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
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Rotation");            
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)oneFingerTwoTaps:(UITapGestureRecognizer *) recognizer
{
    [super oneFingerTwoTaps:recognizer];        
    if( debugTouch ) NSLog(@"\tFIA : Gesture : 1 finger 2 taps");            
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeUp:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeUp:recognizer];
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe up");
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeDown:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeDown:recognizer];
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe down");        
    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeRight:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeRight:recognizer];    
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe right");        
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void)swipeLeft:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeLeft:recognizer];        
    if( debugTouch ) NSLog(@"\tFIA : Gesture : Swipe left");            
}
//----------------------------------------------------------------------------------------------------

@end
