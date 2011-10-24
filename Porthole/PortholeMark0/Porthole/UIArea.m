//
//  UIArea.m
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "UIArea.h"

@implementation UIArea

@synthesize myName;

@synthesize touchAble;
@synthesize markerLoc;
@synthesize multiTouchAble;
@synthesize debugTouch;

@synthesize connected;
@synthesize clientConnection;

- (void)dealloc
{
    [myName release];
    [markerLoc release];
    [clientConnection release];
    [super dealloc];
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
- (id)initWithFrame:(CGRect)frame name:(NSString*)theName bounds:(CGRect)theBounds withTouch:(BOOL)touch withMultiTouch:(BOOL)mTouch connection:(TCPClient*)tcpClient
{
    self = [super initWithFrame:frame];
    if (self) 
    {
        // Initialization code here.
        [self setMyName:theName];
        [self setBounds:theBounds];
        [self setTouchAble:touch];
        [self setMultiTouchAble:mTouch];
        [self clearMarkerLoc];
        
        if( tcpClient != nil)
        {
            [self setClientConnection:tcpClient];
            connected = YES;             
        }
        else
        {
            [self setClientConnection:nil];
            connected = NO;             
            
        }
    }
    
    return self;
}

-(void) clearMarkerLoc
{
    self.markerLoc = nil;
}

-(void) markerLocAddOne:(CGPoint) marker
{
    self.markerLoc = [NSArray arrayWithObject:[NSValue valueWithCGPoint:CGPointMake(marker.x, marker.y)]];
}

-(void) markerLocAdd:(CGPoint) marker
{
    if( self.markerLoc == nil )
    {
        [self markerLocAddOne:marker];
    }

    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:self.markerLoc.count + 1];
    for ( int curPoint = 0 ; curPoint < self.markerLoc.count; curPoint++)
    {
        [newArray addObject:[self.markerLoc objectAtIndex:curPoint]];
    }
    [newArray addObject:[NSValue valueWithCGPoint:CGPointMake(marker.x, marker.y)]];
    
    self.markerLoc = [NSArray arrayWithArray:newArray];
}

-(BOOL) within: (int)xLoc and:(int)yLoc
{
    BOOL within = NO;
    
    float x_st = self.frame.origin.x;
    float y_st = self.frame.origin.y;
    float x_nd = x_st + self.frame.size.width;
    float y_nd = y_st + self.frame.size.height;
    
    if( xLoc >= x_st && xLoc <= x_nd && yLoc >= y_st && yLoc <= y_nd ) within = YES;

    return within;
}

-(CGRect) localCoordCenterWith: (CGFloat)width and:(CGFloat)height
{
    CGFloat halfW = self.bounds.size.width * 0.5;
    CGFloat halfH = self.bounds.size.height * 0.5;    
    CGFloat xOffset = self.bounds.origin.x + halfW - width * 0.5;
    CGFloat yOffset = self.bounds.origin.y + halfH - height * 0.5;
    return CGRectMake(xOffset, yOffset, width, height);    
}

-(CGPoint) localLocOf: (CGPoint)current
{
    CGFloat curX = current.x;
    CGFloat curY = current.y;    

    CGFloat myW = self.bounds.size.width;
    CGFloat myH = self.bounds.size.height;    

    return CGPointMake(curX/myW, curY/myH);
}


-(CGRect) localCoordOf: (CGRect)current
{
    CGFloat curW = current.size.width;
    CGFloat curH = current.size.height;    
    CGFloat xOffset = self.bounds.origin.x + current.origin.x - curW * 0.5;
    CGFloat yOffset = self.bounds.origin.y + current.origin.y - curH * 0.5;
    return CGRectMake(xOffset, yOffset, curW, curH);
}

#
#
#pragma mark - Callback for gesture interaction
#
#

-(void)handlePinch:(UIPinchGestureRecognizer *) recognizer
{
    
}

-(void)handlePan:(UIPanGestureRecognizer *) recognizer
{
    
}

-(void)handleRotation:(UIRotationGestureRecognizer *) recognizer
{
    
}

-(void)oneFingerTwoTaps:(UITapGestureRecognizer *) recognizer
{
    
}

-(void)swipeUp:(UISwipeGestureRecognizer *) recognizer
{
}

-(void)swipeDown:(UISwipeGestureRecognizer *) recognizer
{
    
}

-(void)swipeRight:(UISwipeGestureRecognizer *) recognizer
{
    
}

-(void)swipeLeft:(UISwipeGestureRecognizer *) recognizer
{
    
}

#
#
#pragma mark - Callback for touch interaction
#
#

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{

}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

#
#
#pragma mark - All touch functions
#
#



@end
