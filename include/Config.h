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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations.
namespace libconfig
{
	class Config;
};

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Config
{
public:
	static std::vector<char*> StringToArgv(std::string appName, std::string args);

public:
	Config(): myArgv(NULL), myArgc(1) {}
	Config(int argc, char** argv) { myArgc = argc; myArgv = argv; }
	Config(const char* filename) { myCfgFilename = filename; }
	~Config() {}

	const char* GetConfigFilename() { return myCfgFilename.c_str(); }
	void SetConfigFilename(const char* value) { myCfgFilename = value; }

	const char* GetDisplayConfig() { return myDisplayConfig.c_str(); }
	void SetDisplayConfig(const char* value) { myDisplayConfig = value; }

	int GetArgc() { return myArgc; }
	char** GetArgv() { return myArgv; }

	void Load();

	// Read string value.
	const char* GetValue(const char* name, const char* defaultValue);

	// read int value.
	int GetValue(const char* name, int defaultValue);

	// Read float value
	float GetValue(const char* name, float defaultValue);

	// Read bool value
	bool GetValue(const char* name, bool defaultValue);

	// Read string array.
	bool GetArray(const char* name, std::vector<const char*>& data);

	// Read int array.
	bool GetArray(const char* name, std::vector<int>& data);

	// Read float array.
	bool GetArray(const char* name, std::vector<float>& data);

private:
	std::string myDisplayConfig;
	char** myArgv;
	int myArgc;

	std::string myCfgFilename;
	libconfig::Config* myCfgFile;
};

}; // namespace omega

#endif