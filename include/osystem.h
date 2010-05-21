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

#ifdef WIN32
#include <windows.h>
#endif

// OpenGL includes
#include <GL/gl.h>
#include <GL/glut.h>

#define STDLEN 256 /* Standard string length */

#endif