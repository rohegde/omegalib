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
#ifndef __OPQ_H__
#define __OPQ_H__

#include "osystem.h"
#include "ocfg.h"

//#define OM_ID_MOUSE (OM_DC_POINTER << 8)

void opqInit(const OMConfig* cfg);
void opqCleanup();
void opqStart();
void opqStop();


#endif