//
//  UIArea.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TCPClientOmega.h"

@interface UIArea : UIView
{
    NSString *myName;
    
    NSArray *markerLoc;
    BOOL touchAble;
    BOOL multiTouchAble;
    BOOL debugTouch;
    
    BOOL connected;
    TCPClientOmega *clientConnection;
}

@property (strong) NSString* myName;

@property (strong) NSArray* markerLoc;
@property (assign) BOOL touchAble;
@property (assign) BOOL multiTouchAble;
@property (assign) BOOL debugTouch;

@property (assign) BOOL connected;
@property (strong) TCPClientOmega *clientConnection;



- (id)initWithFrame:(CGRect)frame name:(NSString*)theName bounds:(CGRect)theBounds withTouch:(BOOL)touch withMultiTouch:(BOOL)mTouch connection:(TCPClientOmega*)tcpClient;
-(BOOL) within: (int)xLoc and:(int)yLoc;
-(void) clearMarkerLoc;
-(void) markerLocAddOne:(CGPoint) marker;
-(void) markerLocAdd:(CGPoint) marker;

-(CGPoint) localLocOf: (CGPoint)current;
-(CGRect) localCoordOf: (CGRect)current;
-(CGRect) localCoordCenterWith: (CGFloat)width and:(CGFloat)height;

-(void)handlePinch:(UIPinchGestureRecognizer *) recognizer;
-(void)handlePan:(UIPanGestureRecognizer *)recognizer; 
-(void)handleRotation:(UIRotationGestureRecognizer *)recognizer; 
-(void)oneFingerTwoTaps:(UITapGestureRecognizer *)recognizer; 

-(void)swipeUp:(UISwipeGestureRecognizer *)recognizer; 
-(void)swipeDown:(UISwipeGestureRecognizer *)recognizer; 
-(void)swipeRight:(UISwipeGestureRecognizer *)recognizer; 
-(void)swipeLeft:(UISwipeGestureRecognizer *)recognizer; 

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event; 
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event; 
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event; 

@end
