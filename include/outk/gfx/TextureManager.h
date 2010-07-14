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
#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include "omega.h"

#include "boost/unordered_map.hpp"

namespace outk
{
namespace gfx
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct Texture
	{
		GLuint id;
		GLubyte* data;
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