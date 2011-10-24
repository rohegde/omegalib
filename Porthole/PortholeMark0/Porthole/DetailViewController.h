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
#import "TCPClient.h"

#define DEFAULT_SERVER_ON   NO;
#define DEFAULT_DVC_DEBUG   YES;
#define DEFAULT_DEBUG_TOUCH YES;

@interface DetailViewController : UIViewController <UISplitViewControllerDelegate , UIGestureRecognizerDelegate>
{
    BOOL debugMode;
    
    FreeInteractionAreaView *FIA;
    CustomUIArea *CUA;    
    
    BOOL debugTouch; 
    BOOL connectServer;
    float FIAHeight;
    float CUAHeight;
    
    TCPClient* connection;
}

@property (assign , nonatomic) BOOL debugMode;
@property (strong , nonatomic) id detailItem;
@property (strong , nonatomic) IBOutlet UILabel *detailDescriptionLabel;

@property ( strong , nonatomic ) FreeInteractionAreaView *FIA;
@property ( strong , nonatomic ) CustomUIArea *CUA;

@property ( strong , nonatomic ) TCPClient *connection;

-(void) setupGestureSupport;

-(void) setupConnectionToServer;

-(void) setBoundsLandscape;
-(void) setBoundsPortrait;

@end
