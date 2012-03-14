//
//  OmegaProtocol.h
//  Porthole
//
//  Created by Dennis Chau (Koracas) on 10/23/11.
//  Copyright (c) 2011 All rights reserved.
//

#ifndef OmegaViewer_OmegaProtocol_h
#define OmegaViewer_OmegaProtocol_h

typedef enum
{ 
    Pointer = 0, Mocap=1, Keyboard=2, Controller=3, UI=4, Generic=5 
} OVServiceTypes; 


typedef enum 
{ 
    //! Select: generated when the source of the event gets selected or activated.
    //! Used primarily for use iterface controls.
    Select = 0,
    //! Toggle: generated when some boolean state in the event source changes. Can represent
    //! state changes in physical switches and buttons, or in user interface controls like
    //! check boxes and radio buttons.
    Toggle = 1,
    //!ChangeValue: generated when the source of an event changes it's internal value or state.
    //! Different from Update because ChangeValue is not usually fired at regular intervals,
    //! while Update events are normally sent at a constant rate.
    ChangeValue = 2,
    //! Update: Generated when the soruce of an event gets updated (what 'update') means depends
    //! on the event source.
    Update = 3,
    //! Move: Generated whenever the source of an event moves.
    Move = 4, 
    //! Down: generated when the source of an event goes to a logical 'down' state (i.e. touch on a surface or 
    //! a mouse button press count as Down events)
    Down = 5, 
    //! Up: generated when the source of an event goes to a logical 'up' state (i.e. remove touch from a surface or 
    //! a mouse button release count as Up events)
    Up = 6, 
    //! Trace: generated when a new object is identified by the device managed by the input service 
    //! (i.e head tracking, or a mocap system rigid body).
    Trace = 7, 
    //! Trace: generated when a traced object is lost by the device managed by the input service 
    //! (i.e head tracking, or a mocap system rigid body).
    Untrace = 8,
    
    //! Click: generated on a down followed by an immediate up event.
    //! parameters: position
    Click = 9,
    //! DoubleClick: generated by a sequence of quick down/up/down/up events.
    //! parameters: position.
    DoubleClick = 10,
    //! MoveLeft: generated when the source of event goes toward the left of the screen.
    //! parameters: position.
    MoveLeft = 11,
    //! MoveRight: generated when the source of event goes toward the right of the screen.
    //! parameters: position.
    MoveRight = 12,
    //! MoveUp: generated when the source of event goes toward the top of the screen.
    //! parameters: position.
    MoveUp = 13,
    //! MoveDown: generated when the source of event goes toward the bottom of the screen.
    //! parameters: position.
    MoveDown = 14,
    //! Zoom: zoom event.
    Zoom = 15,
    //! SplitStart: generated at the start of a split/zoom gesture.
    //! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
    SplitStart = 16,
    //! SplitEnd: generated at the end of a split/zoom gesture.
    //! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
    SplitEnd = 17,
    //! Split: generated during a split/zoom gesture. 
    //! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions), value[0] (delta distance) value[1] (delta ratio) .
    Split = 18,
    //! RotateStart: generated at the start of a rotation gesture.
    //! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
    RotateStart = 19,
    //! RotateEnd: generated at the end of a rotation gesture.
    //! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions) .
    RotateEnd = 20,
    //! Rotate: generated when an event source is stationary while a second source is rotating around the first.
    //! parameters: position (center of gesture) pointSet[0, 1] (individual finger positions), rotation[0] (degrees).
    Rotate = 21,
    //! Null: generic null value for event type.
    Null = 22
}
OVEventTypes;

#endif