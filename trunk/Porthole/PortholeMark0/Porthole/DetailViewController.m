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


#
#
#pragma mark - Managing the detail item
#
#

- (void) sendMsgAsService:(int)serviceType event:(int)eventType param:(NSArray*)eventParam from:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        //If the app is suppose to connect to a server, send the TCP Msg 
        if( connectServer )[self.connection sendEventService:serviceType event:eventType param:eventParam];
	}    
}
- (void) sendMsgAsService:(int)service event:(int)event sid:(int)srcId value:(float)val from:(FreeInteractionAreaView *)requestor
{
	if (requestor == self.FIA) 
    {
        //If the app is suppose to connect to a server, send the TCP Msg 
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

- (void)configureView
{
    if(debugMode) NSLog(@"DetailVC :: configureView");
    
    if (self.detailItem) 
    {
        self.detailDescriptionLabel.text = [self.detailItem description];
    }
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenH = screenBounds.size.height;
    CGFloat screenW = screenBounds.size.width;
    
    float curHeight = 0;
    
    CGRect FIABounds = CGRectMake( 0 , curHeight , screenW , screenH * FIAHeight );
    FIA = [ [FreeInteractionAreaView alloc] initWithFrame:FIABounds name:@"FIA" bounds:FIABounds withTouch:YES withMultiTouch:YES];
    [FIA setDebugTouch:debugTouch];
    [FIA setBackgroundColor:[UIColor grayColor]];
    FIA.delegate = self;
    curHeight = curHeight + screenH * FIAHeight;

    
    CGRect CUABounds = CGRectMake( 0 , curHeight, screenW, screenH * CUAHeight );
    CUA = [[CustomUIArea alloc] initWithFrame:CUABounds name:@"CUA" bounds:CUABounds withTouch:YES withMultiTouch:NO];
    curHeight = curHeight + screenH * CUAHeight;
    [CUA setBackgroundColor:[UIColor purpleColor]];
    
    [self.view addSubview:FIA];
    [self.view addSubview:CUA];
    
}

#
#
#pragma mark - View lifecycle
#
#

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    if(debugMode) NSLog(@"DetailVC :: didReceiveMemoryWarning - There is a memory problem!!!");
    
}
//----------------------------------------------------------------------------------------------------

- (void)viewDidLoad
{
    debugMode = DEFAULT_DVC_DEBUG;
    connectServer = DEFAULT_SERVER_ON;
    debugTouch = DEFAULT_DEBUG_TOUCH;
    FIAHeight = 0.6;
    CUAHeight = 0.4;
    
    if(debugMode) NSLog(@"DetailVC :: viewDidLoad");
    
    [super viewDidLoad];
    
    if (connectServer) [self setupConnectionToServer];
    else self.connection = nil;
    
    [self configureView];
    
    [self setupGestureSupport];
    
}

- (void)viewDidUnload
{
    if(debugMode) NSLog(@"DetailVC :: viewDidUnload");
    [super viewDidUnload];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    //    // Return YES for supported orientations
    //    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) 
    //    {
    //    } else 
    //    {
    //       return YES;
    //    }
    if( interfaceOrientation == UIInterfaceOrientationPortrait)
    {
        NSLog(@"DetailVC :: Portrait");
        [self setBoundsPortrait];
        [self.FIA setNeedsDisplay];
        [self.CUA setNeedsDisplay];
        return YES;   
    }
    else if ( interfaceOrientation == UIInterfaceOrientationLandscapeLeft)
    {        
        NSLog(@"DetailVC :: Landscape left");
        [self setBoundsLandscape];
        [self.FIA setNeedsDisplay];
        [self.CUA setNeedsDisplay];
        return YES;
    }
    else if ( interfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
        NSLog(@"DetailVC :: Landscape right");
        [self setBoundsLandscape];
        [self.FIA setNeedsDisplay];
        [self.CUA setNeedsDisplay];
        return YES;
    }
    else if ( interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
    {
        NSLog(@"DetailVC :: Portrait Upside Down");
        [self setBoundsPortrait];
        [self.FIA setNeedsDisplay];
        [self.CUA setNeedsDisplay];
        return YES;       
    }
    else
    {
        NSLog(@"DetailVC :: Orientation Error");
        return NO;   
    }
    
}
//----------------------------------------------------------------------------------------------------

#
#
#pragma mark - Set bounds of the FIA and CUA subview
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
-(void)setBoundsLandscape
{
    CGRect viewFrame = self.view.frame;
    CGFloat viewH = viewFrame.size.height;
    CGFloat viewW = viewFrame.size.width;
    CGFloat viewX = viewFrame.origin.x;
    CGFloat viewY = viewFrame.origin.y;
    
    //In landscape modes, there is no CUA and FIA is the whole area
    NSLog(@" %.2f , %.2f :  %.2f x %.2f" , viewX , viewY , viewW , viewH);
    [self.FIA setFrame:CGRectMake( viewX , viewY , viewW , viewH)];
    
    [self.CUA setHidden:YES];
    [self.CUA setFrame:CGRectMake( viewX , viewY , 0 , 0 )];
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
-(void)setBoundsPortrait
{
    CGRect viewFrame = self.view.frame;
    CGFloat viewH = viewFrame.size.height;
    CGFloat viewW = viewFrame.size.width;
    CGFloat viewX = viewFrame.origin.x;
    CGFloat viewY = viewFrame.origin.y;
    
    NSLog(@" %.2f , %.2f :  %.2f x %.2f" , viewX , viewY , viewW , viewH);
    
    [self.FIA setFrame:CGRectMake( viewX , viewY , viewW , viewH * FIAHeight )];
    viewY = viewY + viewH * FIAHeight;
    
    [self.CUA setHidden:NO];
    [self.CUA setFrame:CGRectMake( viewX , viewY , viewW , viewH * CUAHeight )];
    viewY = viewY + viewH * CUAHeight;
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
// 
//
-(void) setupConnectionToServer
{
    TCPClientOmega *newConnect = [ [TCPClientOmega alloc] initWithFrame:[[UIScreen mainScreen] bounds ]];
    self.connection = newConnect;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//- (void) setupGestureSupport
//
- (void) setupGestureSupport
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
	recognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(handlePan:)];
    [ (UIPanGestureRecognizer*) recognizer setMinimumNumberOfTouches:3];
    [ (UIPanGestureRecognizer*) recognizer setMaximumNumberOfTouches:3];
	[self.FIA addGestureRecognizer:recognizer];
    
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
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeDown:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionDown];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:2];
    [self.FIA addGestureRecognizer:recognizer];
    
    // -----------------------------
    // Swipe up
	// -----------------------------
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeUp:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionUp];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:2];
    [self.FIA addGestureRecognizer:recognizer];
    
	// -----------------------------
	// Swipe left
	// -----------------------------
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeLeft:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionLeft];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:2];
    [self.FIA addGestureRecognizer:recognizer];
    
    // -----------------------------
	// Swipe right
	// -----------------------------
    recognizer = [ [UISwipeGestureRecognizer alloc] initWithTarget:self.FIA action:@selector(swipeRight:)];
    [(UISwipeGestureRecognizer*) recognizer setDirection:UISwipeGestureRecognizerDirectionRight];
    [(UISwipeGestureRecognizer*) recognizer setNumberOfTouchesRequired:2];
    [self.FIA addGestureRecognizer:recognizer];
    
}
//----------------------------------------------------------------------------------------------------
@end
