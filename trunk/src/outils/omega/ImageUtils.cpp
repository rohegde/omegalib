/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#include "omega/ImageUtils.h"
#include "omega/StringUtils.h"
#include "FreeImage.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
byte* ImageUtils::loadImage(const String& filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(format, filename.c_str());
	
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);
	
	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);

	ofmsg("Image loaded: %1%. Size: %2%x%3%", %filename %w %h);
	
	GLubyte* data = new GLubyte[4*w*h];
	char* pixels = (char*)FreeImage_GetBits(image);
	
	for(int j= 0; j<w*h; j++){
		data[j*4+0]= pixels[j*4+2];
		data[j*4+1]= pixels[j*4+1];
		data[j*4+2]= pixels[j*4+0];
		data[j*4+3]= pixels[j*4+3];
	}
	
	FreeImage_Unload(image);

	return data;
}