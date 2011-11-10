//
//  FreeInteractionAreaView.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIArea.h"
#import "UIImage+Resize.h"
#import "PulseCircleView.h"
#import "OmegaProtocol.h"

typedef enum { INIT , CONNECTED , NEW_MODEL , SAME_MODEL } stateIAV;

@class FreeInteractionAreaView;

@protocol FreeInteractionAreaViewDelegate
- (void) sendMsgAsService:(int)serviceType event:(int)eventType param:(NSArray*)eventParam from:(FreeInteractionAreaView *)requestor;
- (void) sendMsgAsService:(int)service event:(int)event sid:(int)srcId value:(float)val from:(FreeInteractionAreaView *)requestor;
- (stateIAV) getConnectionState:(FreeInteractionAreaView *)requestor;
- (void) intConnectionState:(FreeInteractionAreaView *)requestor;
- (void) incrConnectionState:(FreeInteractionAreaView *)requestor;
- (void) connect:(FreeInteractionAreaView *)requestor;
@end


@interface FreeInteractionAreaView : UIArea <PulseCircleViewDelegate>
{
    UIImageView *overlayView;   
    UILabel *myLabel;
    stateIAV myState;
    CGFloat lastScale;
    CGFloat lastRotation;
    
    CGPoint prevPt;
    PulseCircleView *markerView;
    
    id <FreeInteractionAreaViewDelegate> __unsafe_unretained delegate;
}

@property (strong) PulseCircleView *markerView;
@property (assign) CGFloat lastScale;
@property (assign) CGFloat lastRotation;

@property (unsafe_unretained) id <FreeInteractionAreaViewDelegate> delegate;

-(void) makeLabelWithString:(NSString*)msg;

-(void)genThumbnail;

-(void)setupLabel;
-(void)setupMarkerWith:(CGRect)frame;
-(void)setupThumbnail:(CGRect)frame;

-(void)wipeMarkers;

-(void)genTCPMsgWithRecognizer:(UIGestureRecognizer *)recognizer Event:(int)event Param:(CGFloat)param;

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

