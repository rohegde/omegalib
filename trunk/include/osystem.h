/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * This file contains system / platform defines and is included in every other file.
 *********************************************************************************************************************/
#ifndef __OSYSTEM_H__
#define __OSYSTEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>

#include <eq/eq.h>

#ifdef WIN32
#include <windows.h>
#endif

// OpenGL includes
#include <GL/gl.h>
#include <GL/glut.h>

#include "Log.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The current omegalib version string.
#define OM_VERSION "0.1"

#define OM_STRLEN 256 /* Standard string length */

#define OID(s) (unsigned int)(s[0] | s[1] << 8 | s[2] << 16 | s[3] << 24)

#endif