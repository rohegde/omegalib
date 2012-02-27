//
//  DetailViewController.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "FreeInteractionAreaView.h"
#import "CustomUIArea.h"
#import "TCPClientOmega.h"

#define DEFAULT_DVC_DEBUG   NO;
#define DEFAULT_DEBUG_TOUCH YES;

#define FIA_LAND_RATIO 0.8
#define FIA_PORT_RATIO 0.6

#define CUA_LAND_RATIO 0.2
#define CUA_PORT_RATIO 0.4


@interface DetailViewController : UIViewController <UISplitViewControllerDelegate , UIGestureRecognizerDelegate, FreeInteractionAreaViewDelegate >
{
    BOOL debugMode;
    BOOL debugTouch; 
    
    FreeInteractionAreaView *FIA;
    CustomUIArea *CUA;    
    
    BOOL connectServer;
    TCPClientOmega* connection;
    
    CGRect titleBar;
}

@property (assign , nonatomic) BOOL debugMode;

@property (strong , nonatomic) id detailItem;
@property (strong , nonatomic) IBOutlet UILabel *detailDescriptionLabel;

@property ( strong , nonatomic ) FreeInteractionAreaView *FIA;
@property ( strong , nonatomic ) CustomUIArea *CUA;

@property ( strong , nonatomic ) TCPClientOmega *connection;

@property (assign , nonatomic) CGRect titleBar;

-(void) setupGestureSupportFIA;
-(void) setupConnectionToServer;

-(void) setupGestureSupportCUA;

-(void) setBoundsForAreas;

@end
