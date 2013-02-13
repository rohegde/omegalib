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
#include "omega/Texture.h"
#include "omega/PixelData.h"
#include "omega/glheaders.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Texture::Texture(GpuContext* context): 
	GpuResource(context),
	myInitialized(false),
	myTextureUnit(GpuManager::TextureUnitInvalid) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Texture::initialize(int width, int height)
{
	myWidth = width; 
	myHeight = height; 

	//Now generate the OpenGL texture object 
	glGenTextures(1, &myId);
	glBindTexture(GL_TEXTURE_2D, myId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myWidth, myHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GLenum glErr = glGetError();

	if(glErr)
	{
		const unsigned char* str = gluErrorString(glErr);
		oferror("Texture::initialize: %1%", %str);
		return;
	}

	myInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Texture::writePixels(PixelData* data)
{
	if(myInitialized && data != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, myId);
		int xoffset = 0;
		int yoffset = 0;
		int h = data->getHeight();
		int w = data->getWidth();
		byte* pixels = data->lockData();

		GLenum format = GL_RGBA;
		if(data->getFormat() == PixelData::FormatRgb) format = GL_RGB;
		if(format == GL_RGB)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
		else
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, w, h, format, GL_UNSIGNED_BYTE,(GLvoid*)pixels);
		data->unlockData();
		GLenum glErr = glGetError();

		if(glErr)
		{
			const unsigned char* str = gluErrorString(glErr);
			oferror("Texture::writePixels: %1%", %str);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Texture::bind(GpuManager::TextureUnit unit)
{
	myTextureUnit = unit;
	glActiveTexture(myTextureUnit);
	glBindTexture(GL_TEXTURE_2D, myId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Texture::unbind()
{
	myTextureUnit = GpuManager::TextureUnitInvalid;
}
