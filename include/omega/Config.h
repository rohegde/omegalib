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
	static std::vector<char*> stringToArgv(String appName, String args);

public:
	OMEGA_API Config(): myArgv(NULL), myArgc(1), myCfgFile(NULL), myIsLoaded(false) {}
	OMEGA_API Config(int argc, char** argv): myCfgFile(NULL), myIsLoaded(false) { myArgc = argc; myArgv = argv; }
	OMEGA_API Config(const char* filename): myArgv(NULL), myArgc(1), myCfgFile(NULL), myIsLoaded(false) { myCfgFilename = filename; }
	OMEGA_API ~Config() {}

	OMEGA_API const char* getConfigFilename() { return myCfgFilename.c_str(); }
	OMEGA_API void setConfigFilename(const char* value) { myCfgFilename = value; }

	OMEGA_API int getArgc() { return myArgc; }
	OMEGA_API char** getArgv() { return myArgv; }

	OMEGA_API bool load();

	OMEGA_API bool isLoaded() { return myIsLoaded; }

	OMEGA_API Setting& getRootSetting();

private:
	bool myIsLoaded;
	char** myArgv;
	int myArgc;

	String myCfgFilename;
	libconfig::Config* myCfgFile;
};

}; // namespace omega

#endif