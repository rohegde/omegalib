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

#
#
#pragma mark Memory management
#
#
//////////////////////////////////////////////////////////////////////////////////////////////////
//
- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}


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
        debugMode = DEFAULT_CUA_DEBUG;
        
        // Initialization code
        UIView *background = [[UIView alloc] initWithFrame:frame];
        [self addSubview:background];     
        
        //setup the hit and GUI area subviews
        [self setupsubViews: frame];

        //If there is no GUI, do not show the CUA.
        if( self.GUITableData == nil ) CUAVisable = NO;
        else CUAVisable = YES;
        
        //setup notification for when a GUI spec recieved
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(setupCUAFromGUISpec:) name:@"SetupCUAFromGUISpec" object:nil];
        
        
        NSLog(@"Custom User Interface Area Started ..... \n");
    }
    return self;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
-(void)setupsubViews:(CGRect)frame
{   
    hitArea = CUA_HIT_AREA;

    //Bookeeping for subviews 
    if(debugMode) NSLog(@"\tCUA : Setting up subviews");

    CGFloat boundsX = self.frame.origin.x;
    CGFloat boundsY = self.frame.origin.x;
    CGFloat boundsW = self.frame.size.width;
    CGFloat boundsH = self.frame.size.height;

    CGFloat curH = boundsY;

    //The hit box area
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
    
    //The hit GUI Table Area as 
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
    [ self.GUITable setDataSource:self];
	[ self.GUITable setDelegate:self];

    //Setup the GUITableData
    GUITableData = [[NSMutableArray alloc]init];    
    [GUITableData addObjectsFromArray:[NSArray arrayWithObjects: @"0:1:Button:PushMe:PushMe", @"1:2:Slider:SliderDescr:0:10:5", @"2:3:Switch:On/Off:", nil]]; 
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
    if( GUITableData == nil ) return 0;
	else return [GUITableData count];
}

- (void) UIElementReaction:(id)sender
{
    NSString *msg;
    
    if( [sender isKindOfClass:[UIButton class] ] )
    {
        UIButton *temp = sender;
        msg = [NSString stringWithFormat:@"gi:%d:1", temp.tag];
    }
    else if( [sender isKindOfClass:[UISlider class] ] )
    {
        UISlider *temp = sender;
        msg = [NSString stringWithFormat:@"gi:%d:%d", temp.tag , (int)temp.value];
    }
    else if( [sender isKindOfClass:[UISwitch class] ] )
    {
        UISwitch *temp = sender;
        msg = [NSString stringWithFormat:@"gi:%d:%d", temp.tag , temp.isOn];
    }
    else 
    {
        msg = [NSString stringWithString:@"CUA :: ERROR : Unhandeled call back "];        
    }

    [[NSNotificationCenter defaultCenter] postNotificationName:@"UIElementSendMsg" object:msg];

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  - (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
//
//      Load the cells in the CUA tableView
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{	    
    BOOL debugTabelViewCells = NO;
    if( debugTabelViewCells ) NSLog(@"\tCUA :: Cell Gen :: Loading cells for tableview");
    if( debugTabelViewCells ) NSLog(@"\tCUA :: section : row :: %d:%d" , indexPath.section , indexPath.row);
    
    //Get each UIElement that is seperated by ':'
    NSString* UIElement = [GUITableData objectAtIndex: indexPath.row];
    NSString* seperator = [NSString stringWithUTF8String:":"];
    NSArray *SpecsArray = [ UIElement componentsSeparatedByString:seperator ];
    
    //TODO::Check that the msg coming in is good.
    
    //Element Type 
    CUATypes guiElementType = [ [SpecsArray objectAtIndex:0] integerValue];

    //Unique ident
    NSInteger guiElementId = [ [SpecsArray objectAtIndex:1] integerValue];

    //Get cell with unique ident 
  	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:[NSString stringWithFormat:@"Cell %i",guiElementId]];

    //If there is no cell with the specified unique ident
    if (cell == nil)     
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:[NSString stringWithFormat:@"Cell %i" , guiElementId]];        
        if( debugTabelViewCells ) NSLog(@"\t\t Generating new cell : %@ " , [cell reuseIdentifier] );
    }

    CGFloat cellH = tableView.rowHeight;
    CGFloat indent = 500;
    
    cell.textLabel.text = [SpecsArray objectAtIndex:2];
    cell.detailTextLabel.text = [SpecsArray objectAtIndex:3];;

    if ([cell.contentView subviews])
    {
        for (UIView *subview in [cell.contentView subviews]) 
        {
            [subview removeFromSuperview];
        }
    }
        
    switch (guiElementType)
    {
        case CUA_BUTTON :
        {
            CGFloat buttonH = 35.0;
            CGFloat buttonW = 150.0;
            CGFloat marginH = (cellH - buttonH) * 0.5;
            
            UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
            [button addTarget:self action:@selector(UIElementReaction:) forControlEvents:UIControlEventTouchDown];
            [button setTitle:[SpecsArray objectAtIndex:4] forState:UIControlStateNormal];
            [button setTag:guiElementId];
            button.frame = CGRectMake(indent, marginH , buttonW , buttonH);
            [cell.contentView addSubview:button];
            break;
        }   
        case CUA_SLIDER :
        {
            CGFloat sliderH = 35.0;
            CGFloat sliderW = 200.0;
            CGFloat marginH = (cellH - sliderH) * 0.5;

            UISlider *slider = [[UISlider alloc] initWithFrame:CGRectMake(indent, marginH, sliderW, sliderH)];
            [slider addTarget:self action:@selector(UIElementReaction:) forControlEvents:UIControlEventValueChanged];
            [slider setBackgroundColor:[UIColor clearColor]];
            [slider setTag:guiElementId];
            slider.minimumValue = [[SpecsArray objectAtIndex:4] integerValue];
            slider.maximumValue = [[SpecsArray objectAtIndex:5] integerValue];
            slider.value = [[SpecsArray objectAtIndex:6] integerValue];
            slider.continuous = NO;
            [cell.contentView addSubview:slider];
            break;
        }   
        case CUA_TOGGLE :
        {
            CGFloat switchH = 35.0;
            CGFloat switchW = 200.0;
            CGFloat marginH = (cellH - switchH) * 0.5;

            UISwitch *toggle = [[UISwitch alloc] initWithFrame:CGRectMake(indent, marginH, switchW, switchH)];
            [toggle addTarget: self action: @selector(UIElementReaction:) forControlEvents:UIControlEventValueChanged];
            [toggle setTag:guiElementId];
            [toggle setOn:[[SpecsArray objectAtIndex:4] integerValue]];
            [cell.contentView addSubview: toggle];
            break;
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

    [cell setSelectionStyle:UITableViewCellSelectionStyleNone];
	
	//TODO::Delete when done : NSLog(@" Section : %d : Index/Row : %d : %@" , [ indexPath indexAtPosition: 0] , [ indexPath indexAtPosition: 1] , [GUITableData objectAtIndex: indexPath.row]);

	return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

#
#
#pragma mark Handle Notification
#
#
- (void) setupCUAFromGUISpec:(NSNotification *) notification
{
    bool debugCUANotifcation = NO;
    //Make sure it is the right notifcation
    if ([[notification name] isEqualToString:@"SetupCUAFromGUISpec"])
    {
        //Get the object that the notifcation is sending 
        id object = [notification object];
        
        //If it is a string
        if( [object isKindOfClass:[NSString class] ] )
        {
            //report the string
            if( debugCUANotifcation ) NSLog(@"\tCUA :: Spec recv'ed : %@" , object);        
            [self determineGUISpec:object];
        }
    }

}


#
#
#pragma mark Handle GUI Spec Parse
#
#
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
-(void) determineGUISpec:(NSString*) GUISpec
{
    bool debugdetermineGUISpec = NO;

    //The seperator that is btn ea UIElement
    NSString* seperator = [NSString stringWithUTF8String:"|"];
     
    //Get each UIElement that is seperated by '|'
    NSArray *UIElementSpecsArray = [ GUISpec componentsSeparatedByString:seperator ];
    
    //Clear the old GUI Table Data
    [self.GUITableData removeAllObjects];

    //Add in the new
    [self.GUITableData addObjectsFromArray:UIElementSpecsArray];
    [self.GUITableData removeObjectAtIndex:(self.GUITableData.count-1)];
    
    for( int iTableCell = 0; iTableCell < self.GUITableData.count ; iTableCell++ )
    {
        NSString* cellStr = [self.GUITableData objectAtIndex:iTableCell];
        if( debugdetermineGUISpec ) NSLog(@"\t\t%d : %@" , iTableCell , cellStr);
    }

    [self.GUITable reloadData];    
    [self.GUITable setNeedsLayout];

    [[NSNotificationCenter defaultCenter] postNotificationName:@"DetailVCRedrawCUA" object:nil];

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
        [[NSNotificationCenter defaultCenter] postNotificationName:@"DetailVCRedrawCUA" object:nil];
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
        [[NSNotificationCenter defaultCenter] postNotificationName:@"DetailVCRedrawCUA" object:nil];
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
