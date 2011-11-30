//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "UIArea.h"

#define DEFAULT_CUA_DEBUG   NO;

#define CUA_HIT_AREA 0.07

@class CustomUIArea;

@protocol CustomUIAreaDelegate
- (void) CUAFlagRedraw:(CustomUIArea*)requestor;
@end


@interface CustomUIArea : UIArea <UITableViewDelegate, UITableViewDataSource>
{
    BOOL CUAVisable;
    CGFloat hitArea;
    
    UITableView *GUITable;
    NSMutableArray *GUITableData;
    
    UIView *hitBox;
    
    BOOL debugMode;
    
    id <CustomUIAreaDelegate> __unsafe_unretained CUAdelegate;
}

@property (assign) BOOL CUAVisable;
@property (assign) BOOL debugMode;
@property (assign) CGFloat hitArea;

@property ( strong , nonatomic ) UITableView *GUITable;
@property ( strong , nonatomic ) NSMutableArray *GUITableData;

@property ( strong , nonatomic ) UIView *hitBox;

@property (unsafe_unretained) id <CustomUIAreaDelegate> CUAdelegate;

-(void) setupsubViews:(CGRect)frame;

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
