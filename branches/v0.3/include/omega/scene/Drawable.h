/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * DrawContext
 *********************************************************************************************************************/
#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "omega/osystem.h"

namespace omega
{
namespace scene
{
	class Drawable
	{
	public:
		virtual void draw() = 0;

	private:
	};

}; // namespace scene
}; // namespace omega

#endif