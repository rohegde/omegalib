/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "outk/gfx/TextureManager.h"

#include "FreeImage.h"

using namespace omega;
using namespace outk::gfx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TextureManager::TextureManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TextureManager::~TextureManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::loadTexture(omega::String textureName, omega::String filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(format, filename.c_str());
	
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);
	
	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);

	omsg("Image loaded: %s. Size: %dx%d", filename, w, h);
	
	GLubyte* data = new GLubyte[4*w*h];
	char* pixels = (char*)FreeImage_GetBits(image);
	
	for(int j= 0; j<w*h; j++){
		data[j*4+0]= pixels[j*4+2];
		data[j*4+1]= pixels[j*4+1];
		data[j*4+2]= pixels[j*4+0];
		data[j*4+3]= pixels[j*4+3];
	}
	
	FreeImage_Unload(image);

	//Now generate the OpenGL texture object 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid*)data );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	GLenum huboError = glGetError();
	if(huboError)
	{
		oerror("There was an error loading the texture");
		return;
	}

	Texture* tx = new Texture();
	tx->id = textureID;
	tx->data = data;

	// TODO: Check for already existing textures with same name & notify + deallocate.

	myTextures[textureName] = tx;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Texture* TextureManager::getTexture(omega::String fontName)
{
	return myTextures[fontName];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::cleanup()
{
}