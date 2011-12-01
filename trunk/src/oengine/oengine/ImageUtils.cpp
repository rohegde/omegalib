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
#include "oengine/ImageUtils.h"
#include "omega/StringUtils.h"
#include "omega/DataManager.h"
#include "omega/SystemManager.h"
#include "FreeImage.h"

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageData::ImageData(const String& filename, int width, int height):
	myFilename(filename), myWidth(width), myHeight(height)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ImageData::setPixels(PixelData* pixels) 
{
	if(pixels == NULL) 
	{
		owarn("ImageData::setPixels: pixels = NULL");
	}
	else if(pixels->getWidth() != myWidth || pixels->getHeight() != myHeight)
	{
		ofwarn("ImageData::setPixels: wrong size. Expected %1%x%2%, got %3%x%4%", 
			%myWidth %myHeight %pixels->getWidth() %pixels->getHeight());
	}
	else
	{
		myPixels = pixels; 
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageData* ImageUtils::loadImage(const String& filename)
{
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo info = dm->getInfo(filename);

	if(info.isNull())
	{
		return NULL;
	}

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(info.path.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(format, info.path.c_str());
	
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);
	
	int width = FreeImage_GetWidth(image);
	int height = FreeImage_GetHeight(image);

	ImageData* imageData = new ImageData(filename, width, height);

	ofmsg("Image loaded: %1%. Size: %2%x%3%", %filename %width %height);
	
	byte* data = new byte[4 * width * height];
	char* pixels = (char*)FreeImage_GetBits(image);
	
	for(int j= 0; j < width * height ; j++)
	{
		data[j*4+0]= pixels[j*4+2];
		data[j*4+1]= pixels[j*4+1];
		data[j*4+2]= pixels[j*4+0];
		data[j*4+3]= pixels[j*4+3];
	}
	
	FreeImage_Unload(image);

	imageData->setPixels(new PixelData(PixelData::FormatRgb, width, height, data));

	return imageData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ByteArray* ImageUtils::encode(PixelData* data, ImageFormat format)
{
	// Allocate a freeimage bitmap and memory buffer to do the conversion.
	//FIBITMAP* fibmp = FreeImage_Allocate(data->getWidth(), data->getHeight(), 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	FIMEMORY* fmem = FreeImage_OpenMemory();

	byte* pdpixels = data->lockData();
	data->unlockData();
	FIBITMAP* fibmp = FreeImage_ConvertFromRawBits(
		pdpixels,
		data->getWidth(),
		data->getHeight(),
		data->getPitch(),
		data->getBpp(),
		0xff000000, 0x00ff0000, 0x0000ff00);

	// Encode the bitmap to a freeimage memory buffer
	FreeImage_SaveToMemory(FIF_PNG, fibmp, fmem);

	// Copy the freeimage memory buffer to omegalib bytearray
	BYTE* fmemdata = NULL;
	DWORD fmemsize = 0;
	FreeImage_AcquireMemory(fmem, &fmemdata, &fmemsize);
	ByteArray* encodedData = new ByteArray(fmemsize);
	encodedData->copyFrom(fmemdata, fmemsize);

	// Release resources
	FreeImage_CloseMemory(fmem);
	FreeImage_Unload(fibmp);

	return encodedData;
}
