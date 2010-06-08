/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include "osystem.h"

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct InputEvent
{
	enum Type {Move, Down, Up, Trace, Untrace};
	enum Class {Pointer, Mocap, Touch, Keyboard};

    unsigned int id;
	unsigned int flags;
	unsigned int timestamp;
    enum InputType type;
    enum InputClass source;
    float x;
	float y;
	float z;
    float rx;
	float ry;
	float rz;
};

}; // namespace omega

#endif