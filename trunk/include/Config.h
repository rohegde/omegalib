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
	Config(): myArgv(NULL), myArgc(1) {}
	Config(int argc, char** argv) { myArgc = argc; myArgv = argv; }
	~Config() {}

	const char* GetDisplayConfigName() { return myDisplayConfigName.c_str(); }
	void SetDisplayConfigName(const char* value) { myDisplayConfigName = value; }

	int GetArgc() { return myArgc; }
	char** GetArgv() { return myArgv; }

private:
	std::string myDisplayConfigName;
	char** myArgv;
	int myArgc;
};

}; // namespace omega

#endif