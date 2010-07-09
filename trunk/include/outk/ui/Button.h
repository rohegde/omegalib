/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "omega.h"
#include "outk/ui/AbstractButton.h"

namespace outk
{
namespace ui
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button: public AbstractButton
{
public:
	OMEGA_API Button(omega::String name);
	OMEGA_API virtual ~Button();

private:
};
}; // namespace gfx
}; // namespace outk

#endif