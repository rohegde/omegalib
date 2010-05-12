/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Simple logging support. (non-reentrant for now)
 *********************************************************************************************************************/
#ifndef __OLOG_H__
#define __OLOG_H__

#include "osystem.h"

void ologInitFileOutput(const char* filename);
void ologCleanup();
void ologMsg(const char* fmt, ...);
void ologWarning(const char* fmt, ...);
void ologError(const char* fmt, ...);

#endif