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
#include "omega/PixelData.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
PixelData::PixelData(Format fmt, int width, int height, byte* data):
	myData(data),
	myWidth(width),
	myHeight(height),
	myFormat(fmt),
	mySize(0)
{
	if(myData == NULL)
	{
		switch(myFormat)
		{
		case FormatRgb:
			mySize = width * height * 3;
			break;
		case FormatRgba:
			mySize = width * height * 4;
			break;
		case FormatMonochrome:
			mySize = width * height;
			break;
		}
		myData = (byte*)malloc(mySize);
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
byte* PixelData::lockData()
{
	myLock.lock();
	return myData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void PixelData::unlockData()
{
	myLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getRedMask()
{
	switch(myFormat)
	{
	case FormatRgb:
		return 0x0000ff;
	case FormatRgba:
		return 0xff000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getGreenMask()
{
	switch(myFormat)
	{
	case FormatRgb:
		return 0x00ff00;
	case FormatRgba:
		return 0x00000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getBlueMask()
{
	switch(myFormat)
	{
	case FormatRgb:
		return 0xff0000;
	case FormatRgba:
		return 0x00000000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
uint PixelData::getAlphaMask()
{
	switch(myFormat)
	{
	case FormatRgb:
		return 0x000000;
	case FormatRgba:
		return 0xff000000;
	}
	return 0;
}
