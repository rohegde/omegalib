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
	OMEGA_API Config(): myArgv(NULL), myArgc(1), myCfgFile(NULL) {}
	OMEGA_API Config(int argc, char** argv): myCfgFile(NULL) { myArgc = argc; myArgv = argv; }
	OMEGA_API Config(const char* filename): myArgv(NULL), myArgc(1), myCfgFile(NULL) { myCfgFilename = filename; }
	OMEGA_API ~Config() {}

	OMEGA_API const char* GetConfigFilename() { return myCfgFilename.c_str(); }
	OMEGA_API void SetConfigFilename(const char* value) { myCfgFilename = value; }

	OMEGA_API const char* GetDisplayConfig() { return myDisplayConfig.c_str(); }
	OMEGA_API void SetDisplayConfig(const char* value) { myDisplayConfig = value; }

	OMEGA_API int GetArgc() { return myArgc; }
	OMEGA_API char** GetArgv() { return myArgv; }

	OMEGA_API bool Load();

	// Read string value.
	OMEGA_API const char* GetValue(const char* name, const char* defaultValue);

	// read int value.
	OMEGA_API int GetValue(const char* name, int defaultValue);

	// Read float value
	OMEGA_API float GetValue(const char* name, float defaultValue);

	// Read bool value
	OMEGA_API bool GetValue(const char* name, bool defaultValue);

	// Read string array.
	OMEGA_API bool GetArray(const char* name, std::vector<const char*>& data);

	// Read int array.
	OMEGA_API bool GetArray(const char* name, std::vector<int>& data);

	// Read float array.
	OMEGA_API bool GetArray(const char* name, std::vector<float>& data);

private:
	std::string myDisplayConfig;
	char** myArgv;
	int myArgc;

	std::string myCfgFilename;
	libconfig::Config* myCfgFile;
};

}; // namespace omega

#endif