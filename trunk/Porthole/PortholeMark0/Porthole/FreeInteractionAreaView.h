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

typedef enum { INIT , CONNECTED , NEW_MODEL , SAME_MODEL } stateIAV;

@class FreeInteractionAreaView;

@protocol FreeInteractionAreaViewDelegate
- (void)sendMsg:(NSString*)msg from:(FreeInteractionAreaView *)requestor;
@end


@interface FreeInteractionAreaView : UIArea <PulseCircleViewDelegate>
{
    UIImageView *overlayView;   
    UILabel *myLabel;
    stateIAV myState;
    CGFloat lastScale;
    
    PulseCircleView *markerView;

    id <FreeInteractionAreaViewDelegate> delegate;

}

@property (strong) PulseCircleView *markerView;
@property (assign) CGFloat lastScale;

@property (assign) id <FreeInteractionAreaViewDelegate> delegate;

-(void) makeLabelWithString:(NSString*)msg;

-(void)setupOverlay;
-(void)setupLabel;
-(void)setupMarkerWith:(CGRect)frame;

-(void)wipeMarkers;

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
