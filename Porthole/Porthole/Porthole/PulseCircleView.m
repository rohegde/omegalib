//
//  PulseCircleView.m
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "PulseCircleView.h"
#import <QuartzCore/QuartzCore.h>

@implementation PulseCircleView
@synthesize PCVDelegate;

@synthesize radius;
@synthesize _innerCircleRect;
@synthesize _outerCircleRect;




#pragma mark -
#pragma mark Drawing Code:

-(void)drawSimpleCircleWith:(CGContextRef)context at:(CGPoint)loc
{
    CGContextSetLineWidth(context, 1.0);
    
    CGContextSetRGBFillColor(context, 255.0, 0.0 , 0.0 , 1.0 );
    
    CGContextSetRGBStrokeColor(context, 255.0, 0.0, 0.0, 1.0);
    
    CGFloat size = 50;
    
    CGRect rectHoldingCircle = CGRectMake( loc.x - size*.5 , loc.y- size*.5 , size , size);
    
    CGContextFillEllipseInRect(context, rectHoldingCircle);   
}

-(void)drawCircleWith:(CGContextRef)context at:(CGPoint)loc
{
   const int numberColors = 5;

    // the number of different colours
    size_t num_locations = numberColors; 
    // the location of each colour change, these are between 0 and 1, zero is the first circle and 1 is the end circle, so 0.5 is in the middle.
    CGFloat locations[numberColors] = { 0.00 , 0.25 , 0.50 , 0.75 , 1.00 }; 
    // this is the colour components array, because we are using an RGBA system each colour has four components (four numbers associated with it).
    CGFloat components[numberColors * 5] = {  
//        0.4, 0.4, 0.4, 0.9, // Start colour
//        0.9, 0.9, 0.9, 1.0,	// middle colour
//        0.4, 0.4, 0.4, 0.9 }; // End colour
        0.61, 0.78, 0.97, 0.50, // Start colour
        0.16, 0.02, 0.95, 0.70,	// middle colour
        0.90, 0.90, 0.90, 0.60,	// middle colour        
        0.16, 0.02, 0.95, 0.70,	// middle colour
        0.61, 0.78, 0.97, 0.50 }; // End colour

    // You need tell Quartz your colour space (how you define colours), there are many colour spaces: RGBA, black&white...
    CGColorSpaceRef myColorspace = CGColorSpaceCreateDeviceRGB(); 
    
    // Draw the gray background for our progress view:    
    // gradient properties:
    CGGradientRef myGradient = CGGradientCreateWithColorComponents (myColorspace, components,locations, num_locations);

    // gradient start and end points
    CGPoint myStartPoint, myEndPoint; 
    CGFloat myStartRadius, myEndRadius; 

    myStartPoint.x = loc.x; 
    myStartPoint.y = loc.y;
    myEndPoint.x = loc.x;
    myEndPoint.y = loc.y;
    
    myStartRadius = radius ; 
    myEndRadius = radius + 5; 

    // draw the gradient.
    CGContextDrawRadialGradient(context, 
                                myGradient, 
                                myStartPoint, 
                                myStartRadius, 
                                myEndPoint, 
                                myEndRadius, 0);
    CGGradientRelease(myGradient);
    CGColorSpaceRelease(myColorspace);
}

// Drawing code.
- (void)drawRect:(CGRect)rect 
{
    //TODO :: Test Img NSLog(@"PCV :: drawRect");
    // get the drawing canvas (CGContext):
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    // save the context's previous state:
    CGContextSaveGState(context);
    
    //Grab all the markers locations from FIA
    NSArray *markerLocs = [self.PCVDelegate markersForPulseCircleView:self];
    if( markerLocs == nil ) return;
    
    //Draw all the markers in the array
    for( int curMarker = 0 ; curMarker < markerLocs.count ; curMarker ++ )
    {
        NSValue *curLoc = [ markerLocs objectAtIndex:curMarker];
        CGPoint curLocCG = [ curLoc CGPointValue ];
        [self drawCircleWith:context at:CGPointMake( curLocCG.x , curLocCG.y ) ];        
//        [self drawSimpleCircleWith:context at:CGPointMake( curLocCG.x , curLocCG.y ) ];        

    }
}


#pragma mark -
#pragma mark Setter & Getters:

// set the component's value
-(void) setProgress:(CGFloat) newProgress {
	_progress = newProgress;
	[self setNeedsDisplay];
}

// set component colour, set using RGBA system, each value should be between 0 and 1.
-(void) setColourR:(CGFloat) r G:(CGFloat) g B:(CGFloat) b A:(CGFloat) a {
	_r = r;
	_g = g;
	_b = b;
	_a = a;
	[self setNeedsDisplay];
}

// returns the component's value.
-(CGFloat) progress {
	return _progress;
}

#pragma mark -
#pragma mark Superclass Methods:

- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code.
		
		// set initial UIView state
		self.backgroundColor = [UIColor clearColor];
		self.opaque = NO;
		self.hidden = NO;
		self.alpha = 1.0;
		
		// set class variables to default values
		_r = 1.0;
		_g = 0.1;
		_b = 0.1;
		_a = 1.0;
		
		_progress = 0.8;
        
        radius = 10;
		
    }
    return self;
}



@end


// draw outline so that the edges are smooth:
/*    
 
 // set line width
 CGContextSetLineWidth(context, 1);
 // set the colour when drawing lines R,G,B,A. (we will set it to the same colour we used as the start and end point of our gradient )
 CGContextSetRGBStrokeColor(context, 0.4,0.4,0.4,0.9);
 
 // draw an ellipse in the provided rectangle
 CGContextAddEllipseInRect(context, _outerCircleRect);
 CGContextStrokePath(context);
 
 CGContextAddEllipseInRect(context, _innerCircleRect);
 CGContextStrokePath(context);
 
 // Draw the progress:
 
 // First clip the drawing area:
 // save the context before clipping
 CGContextSaveGState(context);
 CGContextMoveToPoint(context, 
 _outerCircleRect.origin.x + _outerCircleRect.size.width/2, // move to the top center of the outer circle
 _outerCircleRect.origin.y +1); // the Y is one more because we want to draw inside the bigger circles.
 // add an arc relative to _progress
 CGContextAddArc(context, 
 _outerCircleRect.origin.x + _outerCircleRect.size.width/2,
 _outerCircleRect.origin.y + _outerCircleRect.size.width/2,
 _outerCircleRect.size.width/2 - 1,
 -M_PI/2,
 (-M_PI/2 + _progress*2*M_PI), 0);
 CGContextAddArc(context, 
 _outerCircleRect.origin.x + _outerCircleRect.size.width/2,
 _outerCircleRect.origin.y + _outerCircleRect.size.width/2,
 _outerCircleRect.size.width/2 - 9,
 (-M_PI/2 + _progress*2*M_PI),
 -M_PI/2, 1);
 // use clode path to connect the last point in the path with the first point (to create a closed path)
 CGContextClosePath(context);
 // clip to the path stored in context
 CGContextClip(context);
 
 // Progress drawing code comes here:
 
 // set the gradient colours based on class variables.
 CGFloat components2[12] = {  _r, _g, _b, _a, // Start color
 ((_r + 0.5 > 1) ? 1 : (_r+0.5) ) , ((_g + 0.5 > 1) ? 1 : (_g+0.5) ), ((_b + 0.5 > 1) ? 1 : (_b+0.5) ), ((_a + 0.5 > 1) ? 1 : (_a+0.5) ),
 _r, _g, _b, _a }; // End color
 
 myGradient = CGGradientCreateWithColorComponents (myColorspace, components2,locations, num_locations);
 
 myStartPoint.x = _innerCircleRect.origin.x + _innerCircleRect.size.width/2; 
 myStartPoint.y = _innerCircleRect.origin.y + _innerCircleRect.size.width/2;
 myEndPoint.x = _innerCircleRect.origin.x + _innerCircleRect.size.width/2;
 myEndPoint.y = _innerCircleRect.origin.y + _innerCircleRect.size.width/2;
 // set the radias for start and endpoints a bit smaller, because we want to draw inside the outer circles.
 myStartRadius = _innerCircleRect.size.width/2 +1; 
 myEndRadius = _outerCircleRect.size.width/2 -1; 
 
 CGContextDrawRadialGradient(context, 
 myGradient, 
 myStartPoint, myStartRadius, myEndPoint, myEndRadius, 0);
 // release myGradient and myColorSpace
 CGGradientRelease(myGradient);
 CGColorSpaceRelease(myColorspace);
 
 
 // draw circle on the outline to smooth it out.
 
 CGContextSetRGBStrokeColor(context, _r,_g,_b,_a);
 
 // draw an ellipse in the provided rectangle
 CGContextAddEllipseInRect(context, _outerCircleRect);
 CGContextStrokePath(context);
 
 CGContextAddEllipseInRect(context, _innerCircleRect);
 CGContextStrokePath(context);
 
 
 //restore the context and remove the clipping area.
 CGContextRestoreGState(context);
 
 // restore the context's state when we are done with it:
 CGContextRestoreGState(context);
 */