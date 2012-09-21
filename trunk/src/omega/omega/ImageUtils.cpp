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
#include "omega/SystemManager.h"
#include "FreeImage.h"

using namespace omega;

// Vector of preallocated memory blocks for image loading.
Vector<void*> ImageUtils::sPreallocBlocks;
size_t ImageUtils::sPreallocBlockSize;
int ImageUtils::sLoadPreallocBlock = -1;

Lock sImageQueueLock;
Queue< Ref<ImageUtils::LoadImageAsyncTask> > sImageQueue;
bool sShutdownLoaderThread = false;

Thread* ImageUtils::sImageLoaderThread = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
class ImageLoaderThread: public Thread
{
public:
	ImageLoaderThread()
	{}

	virtual void threadProc()
	{
		omsg("ImageLoaderThread: start");

		while(!sShutdownLoaderThread)
		{
			if(sImageQueue.size() > 0)
			{
				sImageQueueLock.lock();

				Ref<ImageUtils::LoadImageAsyncTask> task = sImageQueue.front();
				sImageQueue.pop();

				Ref<PixelData> res = ImageUtils::loadImage(task->getData().path, task->getData().isFullPath);
				if(!sShutdownLoaderThread)
				{
					task->getData().image = res;
					task->notifyComplete();
				}

				sImageQueueLock.unlock();
			}
			osleep(100);
		}

		omsg("ImageLoaderThread: shutdown");
	}

private:
};

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ImageUtils::preallocateBlocks(size_t size, int numBlocks)
{
	ofmsg("ImageUtils::preallocateBlocks: allocating %1% blocks of size %2%Kb", %numBlocks %(size / 1024));
	for(int i = 0; i < numBlocks; i++)
	{
		void* block = malloc(size);
		if(block != NULL)
		{
			sPreallocBlocks.push_back(block);
		}
		else
		{
			ofwarn("ImageUtils::preallocateBlocks: failed allocating block %1%", %i);
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int ImageUtils::getNumPreallocatedBlocks()
{
	return sPreallocBlocks.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void* ImageUtils::getPreallocatedBlock(int blockIndex)
{
	if(blockIndex < sPreallocBlocks.size()) return sPreallocBlocks[blockIndex];
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ImageUtils::setLoadPreallocatedBlock(int blockId)
{
	sLoadPreallocBlock = blockId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ImageUtils::internalInitialize()
{
	FreeImage_Initialise();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ImageUtils::internalDispose()
{
	FreeImage_DeInitialise();

	// Clean up preallocated memory blocks.
	foreach(void* ptr, sPreallocBlocks)
	{
		free(ptr);
	}
	sPreallocBlocks.empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ImageUtils::LoadImageAsyncTask* ImageUtils::loadImageAsync(const String& filename, bool hasFullPath)
{
	if(sImageLoaderThread == NULL)
	{
		sImageLoaderThread = new ImageLoaderThread();
		sImageLoaderThread->start();
	}

	sImageQueueLock.lock();
	LoadImageAsyncTask* task = new LoadImageAsyncTask();
	task->setData( LoadImageAsyncTask::Data(filename, hasFullPath) );
	task->setTaskId(filename);
	sImageQueue.push(task);
	sImageQueueLock.unlock();
	return task;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Ref<PixelData> ImageUtils::loadImage(const String& filename, bool hasFullPath)
{
	String path;
	if(!hasFullPath)
	{
		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo info = dm->getInfo(filename);

		if(info.isNull())
		{
			ofwarn("ImageUtils::loadImage: could not load %1%: file not found.", %filename);
			return NULL;
		}
		path = info.path;
	}
	else
	{
		path = filename;
	}

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(format, path.c_str());
	if(image == NULL)
	{
		ofwarn("ImageUtils::loadImage: could not load %1%: unsupported file format, corrupted file or out of memory.", %filename);
		return NULL;
	}

	uint bpp = FreeImage_GetBPP(image);
	int width = FreeImage_GetWidth(image);
	int height = FreeImage_GetHeight(image);

	// If blockId is not -1, use a preallocated memory block to load this image.
	byte* pdata = NULL;
	if(sLoadPreallocBlock != -1) pdata = (byte*)getPreallocatedBlock(sLoadPreallocBlock);

	Ref<PixelData> pixelData;
	int pixelOffset;
	if(bpp == 24)
	{
		pixelData = new PixelData(PixelData::FormatRgb, width, height, pdata);
		pixelOffset = 3;
	}
	else if(bpp == 32)
	{
		pixelData = new PixelData(PixelData::FormatRgba, width, height, pdata);
		pixelOffset = 4;
	}
	else if(bpp == 8)
	{
		// COnvert 8 bit palettized images to 24 bits.
		FIBITMAP* temp = image;
		image = FreeImage_ConvertTo24Bits(image);
		FreeImage_Unload(temp);
		pixelData = new PixelData(PixelData::FormatRgb, width, height, pdata);
		pixelOffset = 3;
	}
	else
	{
		ofwarn("ImageUtils::loadImage: unhandled bpp (%1%) while loading %2%", %bpp %filename);
		return NULL;
	}

	ofmsg("Image loaded: %1%. Size: %2%x%3%", %filename %width %height);
	
	byte* data = pixelData->lockData();
	
	for(int i = 0; i < height; i++)
	{
		char* pixels = (char*)FreeImage_GetScanLine(image, i);
		for(int j = 0; j < width; j++)
		{
			int k = i * width + j;

			data[k * pixelOffset + 0] = pixels[j * pixelOffset + 2];
			data[k * pixelOffset + 1] = pixels[j * pixelOffset + 1];
			data[k * pixelOffset + 2] = pixels[j * pixelOffset + 0];
			if(bpp == 32) data[k * pixelOffset + 3] = pixels[j * pixelOffset + 3];
			//data[j * pixelOffset + 3] = pixels[j * pixelOffset + 3];
		}
	}
	pixelData->unlockData();
	
	FreeImage_Unload(image);

	return pixelData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ByteArray* ImageUtils::encode(PixelData* data, ImageFormat format)
{
	switch (format){
	// PNG
	case FormatPng :
		{
			// Allocate a freeimage bitmap and memory buffer to do the conversion.
			//FIBITMAP* fibmp = FreeImage_Allocate(data->getWidth(), data->getHeight(), 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
			FIMEMORY* fmem = FreeImage_OpenMemory();

			// For some reason it looks like color masks are ignored right now. Maybe it is just when encoding to png.
			byte* pdpixels = data->lockData();
			FIBITMAP* fibmp = FreeImage_ConvertFromRawBits(
				pdpixels,
				data->getWidth(),
				data->getHeight(),
				data->getPitch(),
				data->getBpp(),
				data->getRedMask(), data->getGreenMask(), data->getBlueMask());

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

			data->unlockData();

			return encodedData;
		}
	// JPEG
	case FormatJpeg :
	// JPEG is default
	default:
		{
			// Allocate a freeimage bitmap and memory buffer to do the conversion.
			//FIBITMAP* fibmp = FreeImage_Allocate(data->getWidth(), data->getHeight(), 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
			FIMEMORY* fmem = FreeImage_OpenMemory();

			// For some reason it looks like color masks are ignored right now. Maybe it is just when encoding to png.
			byte* pdpixels = data->lockData();
			FIBITMAP* fibmp = FreeImage_ConvertFromRawBits(
				pdpixels,
				data->getWidth(),
				data->getHeight(),
				data->getPitch(),
				data->getBpp(),
				data->getRedMask(), data->getGreenMask(), data->getBlueMask());

			// Encode the bitmap to a freeimage memory buffer
			FreeImage_SaveToMemory(FIF_JPEG, fibmp, fmem);

			// Copy the freeimage memory buffer to omegalib bytearray
			BYTE* fmemdata = NULL;
			DWORD fmemsize = 0;
			FreeImage_AcquireMemory(fmem, &fmemdata, &fmemsize);
			ByteArray* encodedData = new ByteArray(fmemsize);
			encodedData->copyFrom(fmemdata, fmemsize);

			// Release resources
			FreeImage_CloseMemory(fmem);
			FreeImage_Unload(fibmp);

			data->unlockData();

			return encodedData;
		}

	}
}
