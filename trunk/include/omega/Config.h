/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
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
class OMEGA_API Config
{
public:
	static std::vector<char*> stringToArgv(String appName, String args);

public:
	Config(): myArgv(NULL), myArgc(1), myCfgFile(NULL), myIsLoaded(false) {}
	Config(int argc, char** argv): myCfgFile(NULL), myIsLoaded(false) { myArgc = argc; myArgv = argv; }
	Config(const char* filename);
	~Config() {}

	const char* getConfigFilename() { return myCfgFilename.c_str(); }
	void setConfigFilename(const char* value) { myCfgFilename = value; }

	int getArgc() { return myArgc; }
	char** getArgv() { return myArgv; }

	bool load();

	bool isLoaded() { return myIsLoaded; }

	Setting& getRootSetting();

private:
	bool myIsLoaded;
	char** myArgv;
	int myArgc;

	String myCfgFilename;
	libconfig::Config* myCfgFile;
};

}; // namespace omega

#endif