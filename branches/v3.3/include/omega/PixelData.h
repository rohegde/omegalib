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
#ifndef __PIXEL_DATA_H__
#define __PIXEL_DATA_H__

#include "osystem.h"
#include "omega/TextureSource.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API PixelData: public TextureSource
	{
	public:
		enum Format { FormatRgb, FormatRgba, FormatMonochrome};
	public:
		PixelData(Format fmt, int width, int height, byte* data = NULL);
		virtual ~PixelData();

		byte* lockData();
		void unlockData();

		void resize(int width, int height);

		int getWidth() { return myWidth; }
		int getHeight() { return myHeight; }
		Format getFormat() { return myFormat; }
		size_t getSize() { return mySize; }

		int getPitch();
		int getBpp();

		uint getRedMask();
		uint getGreenMask();
		uint getBlueMask();
		uint getAlphaMask();

		void setDeleteDisabled(bool value) { myDeleteDisabled = value; }
		bool isDeleteDisabled() { return myDeleteDisabled; }

		bool isDirty() { return myDirty; }
		void setDirty(bool value) { myDirty = value; }



	protected:
		void refreshTexture(Texture* texture, const DrawContext& context);

	private:
		Lock myLock;
		Format myFormat;
		byte* myData;
		int myWidth;
		int myHeight;
		size_t mySize;
		bool myDeleteDisabled;
		bool myDirty;
	};
}; // namespace omega

#endif