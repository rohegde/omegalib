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

#include "omega.h"
#include "outk/gfx/TextureManager.h"
#include "outk/ui/Widget.h"

namespace outk
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Image: public Widget
	{
	public:
		OUTK_API Image(omega::String name);
		OUTK_API virtual ~Image();

		outk::gfx::Texture* getTexture() { return myTexture; }
		void setTexture(outk::gfx::Texture* value) { myTexture = value; }

	protected:
		virtual void draw();

	protected:
		outk::gfx::Texture* myTexture;
	};
}; // namespace gfx
}; // namespace outk

#endif