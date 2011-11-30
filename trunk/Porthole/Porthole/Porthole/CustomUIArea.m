//
//  TCPClient.h
//  Porthole
//
//  Created by Dennis Chau on 10/23/11.
//  Copyright (c) 2011 Univeristy of Illinois at Chicago. All rights reserved.
//

#import "CustomUIArea.h"

@implementation CustomUIArea

@synthesize CUAVisable;
@synthesize hitArea;
@synthesize debugMode;

@synthesize hitBox;
@synthesize GUITable;
@synthesize GUITableData;

@synthesize CUAdelegate;

#
#
#pragma mark Memory management
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////
//
#
#
#pragma mark View Setup
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////
//
- (id)initWithFrame:(CGRect)frame name:(NSString*)theName withTouch:(BOOL)touch withMultiTouch:(BOOL)mTouch
{
    self = [super initWithFrame:frame name:theName withTouch:touch withMultiTouch:mTouch];
    if (self) 
    {
        // Initialization code
        UIView *background = [[UIView alloc] initWithFrame:frame];
        [self addSubview:background];     
        
        CUAVisable = YES;
        debugMode = DEFAULT_CUA_DEBUG;
        
        //setup the hit and GUI area subviews
        [self setupsubViews: frame];

        
        NSLog(@"Custom User Interface Area Started ..... \n");
    }
    return self;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
-(void)setupsubViews:(CGRect)frame
{   
    hitArea = CUA_HIT_AREA;

    if(debugMode) NSLog(@"\tCUA : Setting up subviews");

    CGFloat boundsX = self.frame.origin.x;
    CGFloat boundsY = self.frame.origin.x;
    CGFloat boundsW = self.frame.size.width;
    CGFloat boundsH = self.frame.size.height;

    CGFloat curH = boundsY;

    CGFloat hitAreaH = boundsH * hitArea;
    if(debugMode) NSLog(@"\t\thitArea : x : %.2f : w : %.2f : y : %.2f : h : %.2f" , boundsX , boundsW , curH , hitAreaH );
    CGRect hitBoxRect = CGRectMake( boundsX , curH , boundsW , hitAreaH );
    self.hitBox = [ [UIView alloc] initWithFrame:hitBoxRect];
	UIView *containerView = [[UIView alloc] initWithFrame:CGRectMake(0 , 0 , boundsW , hitAreaH)];
	UILabel *headerLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, boundsW , hitAreaH)];
	headerLabel.text = NSLocalizedString(@"Header for GUI", @"");
    headerLabel.textAlignment = UITextAlignmentCenter;
	headerLabel.textColor = [UIColor whiteColor];
	headerLabel.shadowColor = [UIColor blackColor];
	headerLabel.shadowOffset = CGSizeMake(0, 1);
	headerLabel.font = [UIFont boldSystemFontOfSize:18];
	headerLabel.backgroundColor = [UIColor darkGrayColor];
	[containerView addSubview:headerLabel];
	self.hitBox = containerView;
    [self addSubview:hitBox];

    curH = curH + hitAreaH;
    
    int cellSize = 75;
    CGFloat GUIAreaH = boundsH * (1 - hitArea);
    if(debugMode) NSLog(@"\t\tGUIArea : x : %.2f : w : %.2f : y : %.2f : h : %.2f" , boundsX , boundsW , curH , GUIAreaH );
    CGRect GUIAreaRet = CGRectMake( boundsX , curH , boundsW , GUIAreaH );
    self.GUITable = [[UITableView alloc]initWithFrame:GUIAreaRet style:UITableViewStylePlain];
	self.GUITable.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.GUITable.rowHeight = cellSize;
    self.GUITable.backgroundColor = [UIColor blackColor];

    [self addSubview:GUITable];
    curH = curH + GUIAreaH;    

    GUITableData = [[NSMutableArray alloc]init];    
    [GUITableData addObjectsFromArray:[NSArray arrayWithObjects: @"Button", @"Slider", @"Switch", nil]]; 
    
    [ self.GUITable setDataSource:self];
	[ self.GUITable setDelegate:self];
}

#
#
#pragma mark - UITableView Delgates
#
#

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	
	return [GUITableData count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
	
  	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:[NSString stringWithFormat:@"Cell %i",indexPath.section]];

    CGFloat cellH = tableView.rowHeight;
    CGFloat indent = 200.0;
    
	if (cell == nil) 
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:[NSString stringWithFormat:@"Cell %i",indexPath.section]];        
        
        cell.textLabel.text = [GUITableData objectAtIndex: indexPath.row];
        cell.detailTextLabel.text = @"  Here be dragons bro dude";

        switch (indexPath.row)
        {
            case 0 :
            {
                CGFloat buttonH = 35.0;
                CGFloat buttonW = 150.0;
                CGFloat marginH = (cellH - buttonH) * 0.5;
                
                UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
                [button addTarget:self action:@selector(buttonAction:) forControlEvents:UIControlEventTouchDown];
                [button setTitle:@"Show View" forState:UIControlStateNormal];
                button.frame = CGRectMake(indent, marginH , buttonW , buttonH);
                [cell addSubview:button];
                break;
            }   
            case 1 :
            {
                CGFloat sliderH = 35.0;
                CGFloat sliderW = 200.0;
                CGFloat marginH = (cellH - sliderH) * 0.5;

                UISlider *slider = [[UISlider alloc] initWithFrame:CGRectMake(indent, marginH, sliderW, sliderH)];
                [slider addTarget:self action:@selector(sliderAction:) forControlEvents:UIControlEventValueChanged];
                [slider setBackgroundColor:[UIColor clearColor]];
                slider.minimumValue = 0.0;
                slider.maximumValue = 50.0;
                slider.continuous = YES;
                slider.value = 25.0;
                [cell addSubview:slider];
                break;
            }   
            case 2 :
            {
                CGFloat switchH = 35.0;
                CGFloat switchW = 200.0;
                CGFloat marginH = (cellH - switchH) * 0.5;

                UISwitch *toggle = [[UISwitch alloc] initWithFrame:CGRectMake(indent, marginH, switchW, switchH)];
                [toggle addTarget: self action: @selector(toggleAction:) forControlEvents:UIControlEventValueChanged];
                [cell addSubview: toggle];
                break;
            }   

        }        
	}
    
    //set the colors for the cell
    UIView *background = [[UIImageView alloc] initWithFrame:cell.bounds];
    background.backgroundColor = [UIColor blackColor];
    [cell setBackgroundView:background];
    
    cell.textLabel.textColor = [UIColor whiteColor];
    cell.textLabel.backgroundColor = [UIColor blackColor];
    
    cell.detailTextLabel.textColor = [UIColor whiteColor];
    cell.detailTextLabel.backgroundColor = [UIColor blackColor];
	
	//TODO::Delete when done : NSLog(@" Section : %d : Index/Row : %d : %@" , [ indexPath indexAtPosition: 0] , [ indexPath indexAtPosition: 1] , [GUITableData objectAtIndex: indexPath.row]);

	return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

#
#
#pragma mark - Draw functions
#
#

// Only override drawRect: if you perform custom drawing.
- (void)drawRect:(CGRect)rect
{
    if( debugMode)  NSLog(@"\tCUA :: Redrawing");

}

#
#
#pragma mark - Needed for simple touch interaction
#
#

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [super touchesBegan:touches withEvent:event ];        
    
    if(!touchAble) return;

    if( debugTouch) NSLog(@"\tCUA : Touch : A touch has begun");
    
    UITouch *touch = [[event allTouches] anyObject];
    if ([touch tapCount] == 2) 
    {
        
    }    
    else
    {
//        CGPoint point = [ touch locationInView:self];
//        if( debugTouch && touchAble) NSLog(@"\tCUA : Touch : Touch Began at : %f,%f", point.x , point.y );            
    }
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{	
    [super touchesBegan:touches withEvent:event ];        
//    if( debugTouch) NSLog(@"\tCUA : Touch : Touch moved ");      
}
//----------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{	
    [super touchesBegan:touches withEvent:event ];        
    
    UITouch *touch = [[event allTouches] anyObject];
    if ([touch tapCount] == 2) 
    {
//        if( debugTouch && touchAble ) NSLog(@"\tCUA : Touch : A double touch has ended");
    }
    else
    {
//        if( debugTouch && touchAble ) NSLog(@"\tCUA : Touch : A single touch has ended");        
    }    
}
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [super touchesBegan:touches withEvent:event ];
//    if( debugTouch && multiTouchAble ) NSLog(@"\tCUA : Touch : A touch event has been canceled");        
}
//----------------------------------------------------------------------------------------------------


#
#
#pragma mark - Callback for gesture interaction
#
#

-(void)handlePinch:(UIPinchGestureRecognizer *) recognizer
{
    [super handlePinch:recognizer];
    
    if(!multiTouchAble) return;
//    if( debugTouch ) NSLog(@"\tCUA : Gesture : Pinch");    
}

-(void)handlePan:(UIPanGestureRecognizer *) recognizer
{
    [super handlePan:recognizer];        
//   if( debugTouch ) NSLog(@"\tCUA : Gesture : Pan");            
}

-(void)handleRotation:(UIRotationGestureRecognizer *) recognizer
{
    [super handleRotation:recognizer];        
//    if( debugTouch ) NSLog(@"\tCUA : Gesture : Rotation");            
}

-(void)oneFingerTwoTaps:(UITapGestureRecognizer *) recognizer
{
    [super oneFingerTwoTaps:recognizer];        
//    if( debugTouch ) NSLog(@"\tCUA : Gesture : 1 finger 2 taps");            
}

-(void)swipeUp:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeDown:recognizer];
    
    CGPoint touchPt = [ recognizer locationOfTouch:0 inView:self ];            

    //Check that it is the top of the CUA
    if( [self.hitBox hitTest:touchPt withEvent:nil] )
    {
        if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe up");        
        CUAVisable = YES;
        [self.CUAdelegate CUAFlagRedraw:self];
    }
}

-(void)swipeDown:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeDown:recognizer];

    CGPoint touchPt = [recognizer locationOfTouch:0 inView:self];            
    //Check that it is the top of the CUA
    if( [self.hitBox hitTest:touchPt withEvent:nil] )
    {
        if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe down");        
        CUAVisable = NO;
        [self.CUAdelegate CUAFlagRedraw:self];
    }
}

-(void)swipeRight:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeRight:recognizer];    
//    if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe right");        
}

-(void)swipeLeft:(UISwipeGestureRecognizer *) recognizer
{
    [super swipeLeft:recognizer];        
//    if( debugTouch ) NSLog(@"\tCUA : Gesture : Swipe left");            
}

@end
