/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Omegalib configuration
 *********************************************************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "osystem.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Config
{
public:
	Config();
	~Config();

	const char* GetDisplayConfigName() { return myDisplayConfigName.c_str(); }
	void SetDisplayConfigName(const char* value) { myDisplayConfigName = value; }

private:
	std::string myDisplayConfigName;
};

}; // namespace omega

#endif