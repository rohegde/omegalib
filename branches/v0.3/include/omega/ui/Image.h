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
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "omega/TextureManager.h"
#include "omega/ui/Widget.h"

namespace omega
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Image: public Widget
	{
	public:
		OUTILS_API Image(omega::String name);
		OUTILS_API virtual ~Image();

		omega::Texture* getTexture() { return myTexture; }
		void setTexture(omega::Texture* value) { myTexture = value; }

	protected:
		virtual void draw();

	protected:
		omega::Texture* myTexture;
	};
}; // namespace gfx
}; // namespace omega

#endif