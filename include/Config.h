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

	const char* GetDisplayConfig() { return myDisplayConfig.c_str(); }
	void SetDisplayConfig(const char* value) { myDisplayConfig = value; }

	int GetArgc() { return myArgc; }
	char** GetArgv() { return myArgv; }

	static std::vector<char*> StringToArgv(std::string appName, std::string args);

private:
	std::string myDisplayConfig;
	char** myArgv;
	int myArgc;
};

}; // namespace omega

#endif