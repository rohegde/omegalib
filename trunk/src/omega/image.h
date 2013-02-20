/* Copyright (c) 2009 Robert Kooima                                           */
/*                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a    */
/* copy of this software and associated documentation files (the "Software"), */
/* to deal in the Software without restriction, including without limitation  */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,   */
/* and/or sell copies of the Software, and to permit persons to whom the      */
/* Software is furnished to do so, subject to the following conditions:       */
/*                                                                            */
/* The above copyright notice and this permission notice shall be included in */
/* all copies or substantial portions of the Software.                        */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    */
/* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                                  */

#ifndef UTIL3D_IMAGE_H
#define UTIL3D_IMAGE_H

#include "gl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/

void image_flip      (int, int, int, int, void *);
void image_mult_alpha(int, int, int, int, void *);

/*----------------------------------------------------------------------------*/

void *image_read_png(const char *, int *, int *, int *, int *);
void image_write_png(const char *, int,   int,   int,   int, void *);

void *image_read_jpg(const char *, int *, int *, int *, int *);
void image_write_jpg(const char *, int,   int,   int,   int, void *);

void *image_read_exr(const char *, int *, int *, int *, int *);
void image_write_exr(const char *, int,   int,   int,   int, void *);

void *image_read_tif(const char *, int *, int *, int *, int *, int);
void image_write_tif(const char *, int,   int,   int,   int,   int, void **);

/*----------------------------------------------------------------------------*/

void  *image_read(const char *, int *, int *, int *, int *);
void  image_write(const char *, int,   int,   int,   int, void *);

float *image_read_float(const char *, int *, int *, int *, int *);
void  image_write_float(const char *, int,   int,   int,   int, float *);

/*----------------------------------------------------------------------------*/

GLenum image_internal_form(int, int);
GLenum image_external_form(int);
GLenum image_external_type(int);

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif
