/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/PixelData.h"
#include "omega/glheaders.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
PixelData::PixelData(Format fmt, int width, int height, byte* data, uint usageFlags):
	myUsageFlags(usageFlags),
	myData(data),
	myWidth(width),
	myHeight(height),
	myFormat(fmt),
	mySize(0),
	myDeleteDisabled(false),
	myDirty(true)
{
	setDirty(true);
	updateSize();

	if(checkUsage(PixelBufferObject))
	{
		// This pixel data storage is handled through an opengl pixel buffer object.
		glGenBuffers(1, &myPBOId);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, myPBOId);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, mySize, data, GL_STREAM_DRAW);
	}
	else
	{
		// If no user pointer is passed, allocate memory. Otherwise, use user pointer and
		// Disable deallocation.
		if(myData == NULL) myData = (byte*)malloc(mySize);
		else myDeleteDisabled = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PixelData::~PixelData()
{
	if(!myDeleteDisabled)
	{
		if(myData != NULL)
		{
			//ofmsg("PixelData::~PixelData: deleting %1%x%2% image", %myWidth %myHeight);
			free(myData);
		}
	}
	if(checkUsage(PixelBufferObject))
	{
		glDeleteBuffers(1, &myPBOId);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::updateSize()
{
	switch(myFormat)
	{
	case FormatRgb:
		mySize = myWidth * myHeight * 3;
		break;
	case FormatRgba:
		mySize = myWidth * myHeight * 4;
		break;
	case FormatMonochrome:
		mySize = myWidth * myHeight;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::resize(int width, int height)
{
	if(width != myWidth || height != myHeight)
	{
		myLock.lock();

		myWidth = width;
		myHeight = height;

		if(!myDeleteDisabled) free(myData);
		updateSize();
		myData = (byte*)malloc(mySize);

		setDirty(true);
		myLock.unlock();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int PixelData::getPitch()
{
	switch(myFormat)
	{
	case FormatRgb:
		return (myWidth) * 3;
	case FormatRgba:
		return myWidth * 4;
	case FormatMonochrome:
		return myWidth;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int PixelData::getBpp()
{
	switch(myFormat)
	{
	case FormatRgb:
		return 24;
	case FormatRgba:
		return 32;
	case FormatMonochrome:
		return 8;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
byte* PixelData::map()
{
	myLock.lock();
	if(checkUsage(PixelBufferObject))
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, myPBOId);
        byte* ptr = (byte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		return ptr;
	}
	return myData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::unmap()
{
	if(checkUsage(PixelBufferObject))
	{
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
	myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
byte* PixelData::bind(const GpuContext* context)
{
	if(checkUsage(PixelBufferObject))
	{
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, myPBOId);
		return NULL;
	}
	myLock.lock();
	return myData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::unbind()
{
	if(checkUsage(PixelBufferObject))
	{
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
	myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getRedMask()
{
	switch(myFormat)
	{
	case FormatRgb:	return 0x0000ff;
	case FormatRgba: return 0xff000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getGreenMask()
{
	switch(myFormat)
	{
	case FormatRgb: return 0x00ff00;
	case FormatRgba: return 0x00000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getBlueMask()
{
	switch(myFormat)
	{
	case FormatRgb: return 0xff0000;
	case FormatRgba: return 0x00000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getAlphaMask()
{
	switch(myFormat)
	{
	case FormatRgb: return 0x000000;
	case FormatRgba: return 0xff000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::copyFrom(PixelData* other)
{
	if(other != NULL && other->getSize() == mySize)
	{
		void* meptr = map();
		void* otherptr = other->map();
		memcpy(meptr, otherptr, mySize);
		unmap();
		unmap();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::refreshTexture(Texture* texture, const DrawContext& context)
{
	if(!texture->isInitialized()) texture->initialize(myWidth, myHeight);
	texture->writePixels(this);
}

