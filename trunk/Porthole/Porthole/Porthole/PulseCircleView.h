//
//  PulseCircleView.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PulseCircleView;

@protocol PulseCircleViewDelegate
-(NSArray*)markersForPulseCircleView:(PulseCircleView*) requestor;
@end

@interface PulseCircleView : UIView
{
    CGFloat radius;
    
    CGFloat _r;
	CGFloat _g;
	CGFloat _b;
	CGFloat _a;
	
	CGFloat _progress;
	
	CGRect _outerCircleRect;
	CGRect _innerCircleRect;
    
    id<PulseCircleViewDelegate> PCVDelegate;
}

@property (assign) CGFloat radius;
@property (assign) CGRect _outerCircleRect;
@property (assign) CGRect _innerCircleRect;
@property (strong) id <PulseCircleViewDelegate>PCVDelegate;

-(void)drawCircleWith:(CGContextRef)context at:(CGPoint)loc;
-(void) setProgress:(CGFloat) newProgress;		// set the component's value
-(void) setColourR:(CGFloat) r G:(CGFloat) g B:(CGFloat) b A:(CGFloat) a;	// set component colour, set using RGBA system, each value should be between 0 and 1.
-(CGFloat) progress; // returns the component's value.

@end
