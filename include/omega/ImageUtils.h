/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __IMAGE_UTILS_H__
#define __IMAGE_UTILS_H__

#include "osystem.h"
#include "omega/PixelData.h"
#include "omega/AsyncTask.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Loads and manages image data.
	class OMEGA_API ImageUtils
	{
	public:
		enum ImageFormat { 
			FormatPng,
			FormatJpeg
		};

		struct LoadImageAsyncTaskData
		{
			LoadImageAsyncTaskData() {}
			LoadImageAsyncTaskData(const String& _path, bool _isFullPath):
				path(_path), isFullPath(_isFullPath) {}
				
			Ref<PixelData> image;
			String path;
			bool isFullPath;
		};

		typedef AsyncTask<LoadImageAsyncTaskData> LoadImageAsyncTask;
	public:
		//! Preallocated memory management
		//@{
		//! Preallocate memory blocks for image loading and processing.
		//! @return true if preallocation succeeded. False otherwise.
		static bool preallocateBlocks(size_t size, int numBlocks);
		static size_t getPreallocatedBlockSize() { return sPreallocBlockSize; }
		static int getNumPreallocatedBlocks();
		static void* getPreallocatedBlock(int blockIndex);
		//! Specified which preallocated block to use when loading images. Pass -1 to disable 
		//! preallocated block usage on image loading.
		static void setLoadPreallocatedBlock(int blockId);
		static int getLoadPreallocatedBlock() { return sLoadPreallocBlock; }
		//@}

		//! Load an image from a file.
		static Ref<PixelData> loadImage(const String& filename, bool hasFullPath = false);
		//! Load image from a file (async)
		static LoadImageAsyncTask* loadImageAsync(const String& filename, bool hasFullPath = false);

		static ByteArray* encode(PixelData* data, ImageFormat format);

		static void internalInitialize();
		static void internalDispose();

	private:
		static Vector<void*> sPreallocBlocks;
		static size_t sPreallocBlockSize;
		static int sLoadPreallocBlock;
		static Thread* sImageLoaderThread;

	private:
		ImageUtils() {}
	};
}; // namespace omega

#endif