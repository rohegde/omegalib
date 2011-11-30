//
//  DetailViewController.m
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "DetailViewController.h"

@interface DetailViewController ()

@property (strong, nonatomic) UIPopoverController *masterPopoverController;
- (void)configureView;
@end

@implementation DetailViewController

@synthesize detailItem = _detailItem;
@synthesize detailDescriptionLabel = _detailDescriptionLabel;
@synthesize masterPopoverController = _masterPopoverController;
@synthesize debugMode;

@synthesize FIA;
@synthesize CUA;
@synthesize connection;

@synthesize titleBar;

#
#
#pragma mark - TCPClientOmega Delegates
#
#
- (void) flagRedraw:(TCPClientOmega*)requestor
{
    if (requestor == self.connection)
    {
        NSLog(@"DetailVC :: Dumping img data to FIA and telling it to draw ");
        //Convert byte Array to NSData
        NSData *imgDataRecv = [ NSData dataWithBytes:self.connection.imgByteData length:self.connection.imgByteDataLength ];    
        //Convert NSData to UIImage
        UIImage *imgRecv = [UIImage imageWithData:imgDataRecv];
        
        self.FIA.overlayImg = imgRecv;
        self.FIA.overLayImgNew = YES;
        [self.FIA setNeedsDisplay];
    }

}

#
#
#pragma mark - CUA Delegates
#
#
- (void) CUAFlagRedraw:(CustomUIArea*)requestor
{
    if (requestor == self.CUA)
    {
        [self setBoundsForAreas];
        [self.CUA setNeedsDisplay];
    }
    
}


#
#
#pragma mark - FIA Delegates
#
#

//-(UIImage*) getImage:(FreeInteractionAreaView *)requestor
//{
//    if (requestor == self.FIA) 
//    {
//        if( self.connection.newImgByteData)
//        {   
//            NSLog(@"DetailVC :: getImage for FIA from TCP");
//            //Convert byte Array to NSData
//            NSData *imgDataRecv = [ NSData dataWithBytes:self.connection.imgByteData length:self.connection.imgByteDataLength ];    
//            //Convert NSData to UIImage
//            UIImage *imgRecv = [UIImage imageWithData:imgDataRecv];
//            
//            return imgRecv;
//        }
//        else return nil;
//    }   
//    else return nil;
//}

- (void) connect:(FreeInteractionAreaView *)requestor
{
    if (requestor == self.FIA) 
    {
        [self.connection estConnectionToServer];
    }    
}


- (stateIAV) getConnectionState:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        if(connectServer && self.connection != nil)
        {
            switch (self.connection.myState) {
                case CLIENT_INIT: return INIT; break;
                case CLIENT_CONNECTED: return CONNECTED; break;
                case CLIENT_GUIRECVED: return NEW_MODEL; break;
                case CLIENT_IDLE: return SAME_MODEL; break;                    
            }
        }
        else return -1;
    }    
    else return -1;
}

- (void) intConnectionState:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        //if connectServer and allows to connect, then set connection to init state
        if(connectServer && self.connection != nil) self.connection.myState = CLIENT_INIT;
    }    
}

//TODO::Enum Fix:: Assumed that the states are ordered

- (void) incrConnectionState:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        //if connectServer and allows to connect, increment the state by one
        if(connectServer && self.connection != nil) self.connection.myState++;

        //After every state draw something.
        NSLog(@"DetailVC :: State change draw something" );
        [self.FIA setNeedsDisplay];
    }    
}

- (void) sendMsgAsService:(int)serviceType event:(int)eventType param:(NSArray*)eventParam from:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        
        if( debugMode ) NSLog(@"\tDetailVC :: Sending");

        //If there is a connection to the server, send the TCP msg
        if( connectServer )[self.connection sendEventService:serviceType event:eventType param:eventParam];

//        for( int i = 0 ; i < eventParam.count ; i++)
//            {
//                NSNumber* ele = [eventParam objectAtIndex:i];
//                NSLog(@"val %d: %f" , i , ele.floatValue);
//            }        
	}    
}
- (void) sendMsgAsService:(int)service event:(int)event sid:(int)srcId value:(float)val from:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        if( debugMode ) NSLog(@"\tDetailVC :: Sending");

        //If there is a connection to the server, send the TCP msg
        if( connectServer )[self.connection sendEventService:service event:event sid:srcId value:val];        
	}    
}    

#
#
#pragma mark - Managing the detail item
#
#

- (void)setDetailItem:(id)newDetailItem
{
    if (_detailItem != newDetailItem) {
        _detailItem = newDetailItem;
        
        // Update the view.
        [self configureView];
    }
    
    if (self.masterPopoverController != nil) {
        [self.masterPopoverController dismissPopoverAnimated:YES];
    }        
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
-(void)setBoundsForAreas
{
    if( debugMode ) NSLog(@"\tDetailVC :: setBoundsForAreas");
    //Size of the workable space 
    CGRect frame = self.view.frame;         
    CGFloat frameX = frame.origin.x;
    CGFloat frameY = frame.origin.y;    
    CGFloat frameH = frame.size.height;
    CGFloat frameW = frame.size.width;

    if( debugMode ) NSLog(@"\t\tFrame : x : %.2f : w : %.2f : y : %.2f h : %.2f" , frameX , frameW , frameY , frameH);
    
    CGFloat FIAH , CUAH;
    
    //If the CUA is visable
    if( self.CUA.CUAVisable )        
    {
        if( [[UIDevice currentDevice] orientation] == UIInterfaceOrientationLandscapeLeft ||
           [[UIDevice currentDevice] orientation] == UIInterfaceOrientationLandscapeRight)
        {
            FIAH = FIA_LAND_RATIO * frameH;
            CUAH = CUA_LAND_RATIO * frameH;
        }
        else
        {
            FIAH = FIA_PORT_RATIO * frameH;
            CUAH = CUA_PORT_RATIO * frameH;
        }    
    }
    
    //If the CUA is ~visable
    else
    {
        //In landscape modes, there is only the top of the CUA and FIA is the rest of the area
        UIView *CUAHitBox = self.CUA.hitBox;
        if( debugMode ) NSLog(@"\t\tCUAHitBox : x : %.2f to %.2f : y : %.2f to %.2f" , CUAHitBox.bounds.origin.x , CUAHitBox.bounds.size.width , CUAHitBox.bounds.origin.y , CUAHitBox.bounds.size.height);

        CUAH = CUAHitBox.frame.size.height;            
        FIAH = (frameH - CUAH);
    }
   
    [self.FIA setFrame:CGRectMake( frameX , frameY , frameW , FIAH )];
    if( debugMode ) NSLog(@"\t\tFIA x: %.2f to %.2f  : y : %.2f to %.2f" , frameX , frameW , frameY , frameY + FIAH);
    frameY = frameY + FIAH;        
    
    [self.CUA setFrame:CGRectMake( frameX , frameY , frameW , CUAH )];
    if( debugMode ) NSLog(@"\t\tCUA x: %.2f to %.2f  : y : %.2f to %.2f" , frameX , frameW , frameY , frameY + CUAH);
    frameY = frameY + CUAH;
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)configureView
{
    //    //Size of the device in portrait mode 
    //    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    //    CGFloat screenX = screenBounds.origin.x;    //iPad 2 = 0
    //    CGFloat screenY = screenBounds.origin.y;    //iPad 2 = 0
    //    CGFloat screenW = screenBounds.size.width;  //iPad 2 = 768
    //    CGFloat screenH = screenBounds.size.height; //iPad 2 = 1024
    //    NSLog(@"Screen : x : %f : y : %f : w : %f h :  %f " , screenX , screenY, screenW , screenH);
    //    
    //    //Size of the workable space 
    //    CGRect frame = self.view.frame;         
    //    CGFloat frameX = frame.origin.x;         //iPad 2 = 0
    //    CGFloat frameY = frame.origin.y;         //iPad 2 = 20    
    //    CGFloat frameW = frame.size.width;       //iPad 2 = 768
    //    CGFloat frameH = frame.size.height;      //iPad 2 = 1004
    //    NSLog(@"Frame : x : %f : y : %f : w : %f h :  %f " , frameX , frameY , frameW , frameH);
    //
    //    
    //    //Size of the workable space 
    //    CGRect bounds = self.view.bounds;         
    //    CGFloat boundsX = bounds.origin.x;         //iPad 2 = 0
    //    CGFloat boundsY = bounds.origin.y;         //iPad 2 = 0    
    //    CGFloat boundsW = bounds.size.width;       //iPad 2 = 768
    //    CGFloat boundsH = bounds.size.height;      //iPad 2 = 1004
    //    NSLog(@"Brounds : x : %f : y : %f : w : %f h :  %f " , boundsX , boundsY , boundsW , boundsH);

    if(debugMode) NSLog(@"\tDetailVC :: configureView");
    
    if (self.detailItem) 
    {
        self.detailDescriptionLabel.text = [self.detailItem description];
    }
    
    //Size of the device in portrait mode 
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenW = screenBounds.size.width;  //iPad 2 = 768
    CGFloat screenH = screenBounds.size.height; //iPad 2 = 1024

    float curHeight = 0;
    
    if(debugMode) NSLog(@"\t\tFIA : x : %f : w : %f : y : %f h :  %f ", 0.0  , screenW , curHeight, screenH * FIA_PORT_RATIO );
    CGRect FIABounds = CGRectMake( 0 , curHeight , screenW , screenH * FIA_PORT_RATIO );
    FIA = [ [FreeInteractionAreaView alloc] initWithFrame:FIABounds name:@"FIA" withTouch:YES withMultiTouch:YES];
    [FIA setDebugTouch:debugTouch];
    [FIA setBackgroundColor:[UIColor blackColor]];
    self.FIA.FIAdelegate = self;
    curHeight = curHeight + screenH * FIA_PORT_RATIO;

    if(debugMode) NSLog(@"\t\tCUA : x : %f : w : %f : y : %f h :  %f " , 0.0  , screenW , curHeight, screenH * CUA_PORT_RATIO );
    CGRect CUABounds = CGRectMake( 0 , curHeight, screenW, screenH * CUA_PORT_RATIO );
    CUA = [[CustomUIArea alloc] initWithFrame:CUABounds name:@"CUA" withTouch:YES withMultiTouch:YES];
    [CUA setDebugTouch:debugTouch];
    [CUA setBackgroundColor:[UIColor grayColor]];
    self.CUA.CUAdelegate = self;

    curHeight = curHeight + screenH * CUA_PORT_RATIO;
    
    [self.view addSubview:FIA];
    [self.view addSubview:CUA];

}

#
#
#pragma mark - View lifecycle
#
#

// Drawing code.
- (void)drawRect:(CGRect)rect 
{
    NSLog(@"\tDetailVC :: drawing");
    [self.CUA setNeedsDisplay];
    [self.FIA setNeedsDisplay];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    NSLog(@"DetailVC :: didReceiveMemoryWarning - There is a memory problem!!!");
    
}
//----------------------------------------------------------------------------------------------------

- (void)viewDidLoad
{
    [super viewDidLoad];
    if(debugMode) NSLog(@"DetailVC :: viewDidLoad");

    debugMode = DEFAULT_DVC_DEBUG;
    debugTouch = DEFAULT_DEBUG_TOUCH;

    //Determine from defaults in Settings, whether to allow connection to server.
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    BOOL estConnectionToServer = [defaults boolForKey:@"connectToServer"];
    connectServer = estConnectionToServer;

    self.CUA.CUAVisable = YES;
    
    if (connectServer) [self setupConnectionToServer];
    else self.connection = nil;
    
    [self configureView];
    
    [self setupGestureSupportFIA];
    [self setupGestureSupportCUA];

    NSLog(@"Detail View Controller Loaded ..... ");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
-(void) setupConnectionToServer
{
    TCPClientOmega *newConnect = [ [TCPClientOmega alloc] initWithFrame:[[UIScreen mainScreen] bounds ]];
    self.connection = newConnect;
    self.connection.TCPClientOmegaDelegate = self;
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
- (void)viewDidUnload
{
    if(debugMode) NSLog(@"DetailVC :: viewDidUnload");
    [super viewDidUnload];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if( interfaceOrientation == UIInterfaceOrientationPortrait ||
       interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
    {
        if(debugMode) NSLog(@"DetailVC :: Portrait");
        [self setBoundsForAreas];
        [self.view setNeedsDisplay];
        return YES;   
    }
    else if ( interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
             interfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {        
        if(debugMode) NSLog(@"DetailVC :: Landscape");
        [self setBoundsForAreas];
        [self.view setNeedsDisplay];
        return YES;
    }
    else
    {
        if(debugMode) NSLog(@"DetailVC :: Orientation Error");
        return NO;   
    }
    
}
//----------------------------------------------------------------------------------------------------

#
#
#pragma mark - Split view
#
#

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
    barButtonItem.title = NSLocalizedString(@"Master", @"Master");
    [self.navigationItem setLeftBarButtonItem:barButtonItem animated:YES];
    self.masterPopoverController = popoverController;
}

- (void)splitViewController:(UISplitViewController *)splitController willShowViewController:(UIViewController *)viewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem
{
    // Called when the view is shown again in the split view, invalidating the button and popover controller.
    [self.navigationItem setLeftBarButtonItem:nil animated:YES];
    self.masterPopoverController = nil;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//- (void) setupGestureSupportFIA
//
- (void)setupGestureSupportFIA
{
    /*
     Create and configure the five recognizers. Add each to the view as a gesture recognizer.
     */
	UIGestureRecognizer *recognizer;
    
    /*
     Create a pinch recognizer and add it to the view.
     Keep a reference to the recognizer to test in gestureRecognizer:shouldReceiveTouch:.
     */
    recognizer = [ [UIPinchGestureRecognizer alloc ] initWithTarget:self.FIA action:@selector(handlePinch:) ];
    [self.FIA addGestureRecognizer:recognizer];
    
    /*
     Create a rotation gesture recognizer.
     We're only interested in receiving messages from this recognizer, and the view will take ownership of it, so we don't need to keep a reference to it.
     */
	recognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self.FIA  action:@selector(handleRotation:)];
	[self.FIA addGestureRecognizer:recognizer];
    
    /*
     Create a pan gesture recognizer.
     We're only interested in receiving messages from this recognizer, and the view will take ownership of it, so we don't need to keep a reference to it.
     */
//	recognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(handlePan:)];
//    [ (UIPanGestureRecognizer*) recognizer setMinimumNumberOfTouches:3];
//    [ (UIPanGestureRecognizer*) recognizer setMaximumNumberOfTouches:3];
//	[self.FIA addGestureRecognizer:recognizer];
    
    // -----------------------------
    // One finger, two taps
    // -----------------------------
    recognizer = [ [UITapGestureRecognizer alloc ] initWithTarget:self.FIA action:@selector(oneFingerTwoTaps:) ];
    [(UITapGestureRecognizer*) recognizer setNumberOfTapsRequired:2];
    [(UITapGestureRecognizer*) recognizer setNumberOfTouchesRequired:1];
    [self.FIA addGestureRecognizer:recognizer];
    
	// -----------------------------
	// Swipe down
	// -----------------------------
//    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeDown:)];
//    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionDown];
//    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:3];
//    [self.FIA addGestureRecognizer:recognizer];
    
    // -----------------------------
    // Swipe up
	// -----------------------------
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeUp:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionUp];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:1];
    [self.FIA addGestureRecognizer:recognizer];
    
	// -----------------------------
	// Swipe left
	// -----------------------------
//    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeLeft:)];
//    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionLeft];
//    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:3];
//    [self.FIA addGestureRecognizer:recognizer];
    
    // -----------------------------
	// Swipe right
	// -----------------------------
//    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeRight:)];
//    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionRight];
//    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:1];
//    [self.FIA addGestureRecognizer:recognizer];
    
}
//----------------------------------------------------------------------------------------------------

#
#
#pragma mark - DetailVC touch stuff
#
#

//////////////////////////////////////////////////////////////////////////////////////////////////////
//- (void) setupGestureSupportCUA
//
- (void)setupGestureSupportCUA
{
    /*
     Create and configure the five recognizers. Add each to the view as a gesture recognizer.
     */
	UIGestureRecognizer *recognizer;
    
    // -----------------------------
    // Swipe up
	// -----------------------------
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.CUA action:@selector(swipeUp:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionUp];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:1];
    [self.CUA addGestureRecognizer:recognizer];
    
    // -----------------------------
    // Swipe down
	// -----------------------------
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.CUA action:@selector(swipeDown:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionDown];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:1];
    [self.CUA addGestureRecognizer:recognizer];

    
}
//----------------------------------------------------------------------------------------------------

@end
