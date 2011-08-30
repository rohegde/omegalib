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
bool ImageUtils::loadImage(const String& filename, ImageData* data)
{
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo info = dm->getInfo(filename);

	if(info.isNull())
	{
		return false;
	}

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(info.path.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(format, info.path.c_str());
	
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);
	
	data->width = FreeImage_GetWidth(image);
	data->height = FreeImage_GetHeight(image);

	ofmsg("Image loaded: %1%. Size: %2%x%3%", %filename %data->width %data->height);
	
	data->data = onew(GLubyte[4 * data->width * data->height]);
	char* pixels = (char*)FreeImage_GetBits(image);
	
	for(int j= 0; j < data->width * data->height ; j++)
	{
		data->data[j*4+0]= pixels[j*4+2];
		data->data[j*4+1]= pixels[j*4+1];
		data->data[j*4+2]= pixels[j*4+0];
		data->data[j*4+3]= pixels[j*4+3];
	}
	
	FreeImage_Unload(image);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Texture* ImageUtils::createTexture(TextureManager* mng, const String& name, const String& filename)
{
	ImageData id;
	if(loadImage(filename, &id))
	{
		Texture* tex = mng->createTexture(name, id.width, id.height, id.data);
		odelete(id.data);
		return tex;
	}
	return NULL;
}
