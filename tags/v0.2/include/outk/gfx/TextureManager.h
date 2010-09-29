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
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "omega.h"

#include "boost/unordered_map.hpp"

namespace outk
{
namespace gfx
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Texture
	{
	public:
		Texture(): 
		  myInitialized(false), 
		  myDirty(true),
		  myData(NULL) {}

		bool isDirty() { return myDirty; }
		void setDirty() { myDirty = true; }

		byte* getData() { return myData; }
		void setData(byte* data, int width, int height) { myData = data; myWidth = width; myHeight = height; }

		int getWidth() { return myWidth; }
		int getHeight() { return myHeight; }

		GLuint getGLTexture() { if(myDirty) refresh(); return myId; }

		OUTK_API virtual void refresh();

	private:
		bool myInitialized;
		bool myDirty;
		GLuint myId;
		GLubyte* myData;
		int myWidth;
		int myHeight;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! A dictionary containing <String, Texture*> pairs.
	typedef boost::unordered_map<omega::String, Texture*> TextureDictionary;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Loads images and manages OpenGL textures.
	class TextureManager
	{
	public:
		OUTK_API TextureManager();
		OUTK_API ~TextureManager();

		OUTK_API void cleanup();

		OUTK_API void loadTexture(omega::String textureName, omega::String filename);
		OUTK_API Texture* getTexture(omega::String fontName);

	private:
		TextureDictionary myTextures;
	};
};
}; // namespace omega

#endif