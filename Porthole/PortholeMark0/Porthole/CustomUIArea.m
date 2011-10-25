//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "CustomUIArea.h"

@implementation CustomUIArea

#
#
#pragma mark Memory management
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////
//
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
        NSLog(@"CUA init\n");
        
        UIView *background = [[UIView alloc] initWithFrame:theBounds];
        [self addSubview:background];     
        
        [self setupLabel];
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
    label.text = self.myName;    
    label.textAlignment = UITextAlignmentCenter;
    label.textColor = [UIColor yellowColor];
    label.shadowColor = [UIColor whiteColor];
    label.shadowOffset = CGSizeMake(1,1);
    label.font = [UIFont fontWithName:@"Helvetica" size:20];
    label.backgroundColor = [UIColor clearColor];
    [self addSubview:label];
    myLabel = label; 
    
}

#
#
#pragma mark - Draw functions
#
#

// Only override drawRect: if you perform custom drawing.
- (void)drawRect:(CGRect)rect
{
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
    
    if(!touchAble) return;

    if( debugTouch) NSLog(@"\tCUA : Touch : A touch has begun");
    
    UITouch *touch = [[event allTouches] anyObject];
    if ([touch tapCount] == 2) 
    {
        
    }    
    else
    {
        CGPoint point = [ touch locationInView:self];
        if( debugTouch && touchAble) NSLog(@"\tCUA : Touch : Touch Began at : %f,%f", point.x , point.y );            
    }
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{	
    [super touchesBegan:touches withEvent:event ];        
    if( debugTouch) NSLog(@"\tCUA : Touch : Touch moved ");      
}
//----------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{	
    [super touchesBegan:touches withEvent:event ];        
    
    UITouch *touch = [[event allTouches] anyObject];
    if ([touch tapCount] == 2) 
    {
        if( debugTouch && touchAble ) NSLog(@"\tCUA : Touch : A double touch has ended");
    }
    else
    {
        if( debugTouch && touchAble ) NSLog(@"\tCUA : Touch : A single touch has ended");        
    }    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [super touchesBegan:touches withEvent:event ];
    if( debugTouch && multiTouchAble ) NSLog(@"\tCUA : Touch : A touch event has been canceled");        
}
//----------------------------------------------------------------------------------------------------


#
#
#pragma mark - Callback for gesture interaction
#
#

-(void)handlePinch:(UIPinchGestureRecognizer *) recognizer
{
    [super handlePinch:recognizer];
    
    if(!multiTouchAble) return;
    if( debugTouch ) NSLog(@"\tCUA : Gesture : Pinch");    
}

-(void)handlePan:(UIPanGestureRecognizer *) recognizer
{
    [super handlePan:recognizer];        
    //   if( debugTouch ) NSLog(@"\tCUA : Gesture : Pan");            
}

-(void)handleRotation:(UIRotationGestureRecognizer *) recognizer
{
    [super handleRotation:recognizer];        
    if( debugTouch ) NSLog(@"\tCUA : Gesture : Rotation");            
}

-(void)oneFingerTwoTaps:(UITapGestureRecognizer *) recognizer
{
    [super oneFingerTwoTaps:recognizer];        
    if( debugTouch ) NSLog(@"\tCUA : Gesture : 1 finger 2 taps");            
}

-(void)swipeUp:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeUp:recognizer];
    if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe up");
}

-(void)swipeDown:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeDown:recognizer];
    if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe down");        
    
}

-(void)swipeRight:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeRight:recognizer];    
    if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe right");        
}

-(void)swipeLeft:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeLeft:recognizer];        
    if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe left");            
}

@end
