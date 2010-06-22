/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Implementation of Config class methods. See Config.h for detailed information.
 *********************************************************************************************************************/
#include "Config.h"

// Libconfig
#include "libconfig.hh"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<char*> Config::StringToArgv(std::string appName, std::string args)
{
	std::istringstream ss(args);
	std::string arg;
	std::vector<std::string> v1;
	std::vector<char*> v2;
	v2.push_back(_strdup(appName.c_str()));
	while (ss >> arg)
	{
	   v1.push_back(arg); 
	   v2.push_back(_strdup(v1.back().c_str()));
	}
	return v2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::Load()
{
	myCfgFile = new libconfig::Config();
	FILE* stream = fopen(myCfgFilename.c_str(), "r");
	if(stream == NULL)
	{
		oerror("Config::Load - Opening file failed: %s", myCfgFilename.c_str());
		return false;
	}

	omsg("Opened config file: %s", myCfgFilename.c_str());

	try
	{
		myCfgFile->read(stream);
	}
	catch(libconfig::ParseException e)
	{
		oerror("Config loading: %s at line %d", e.getError(), e.getLine());
		fclose(stream);
		return false;
	}
	fclose(stream);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* Config::GetValue(const char* name, const char* defaultValue)
{
	if(!myCfgFile) return defaultValue;
	if(myCfgFile->exists(name)) return (const char*)myCfgFile->lookup(name);
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Config::GetValue(const char* name, int defaultValue)
{
	if(!myCfgFile) return defaultValue;
	if(myCfgFile->exists(name)) return (int)myCfgFile->lookup(name);
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Config::GetValue(const char* name, float defaultValue)
{
	if(!myCfgFile) return defaultValue;
	if(myCfgFile->exists(name)) return (float)myCfgFile->lookup(name);
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::GetValue(const char* name, bool defaultValue)
{
	if(!myCfgFile) return defaultValue;
	if(myCfgFile->exists(name)) return (bool)myCfgFile->lookup(name);
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::GetArray(const char* name, std::vector<const char*>& data)
{
	if(!myCfgFile) return false;
	if(!myCfgFile->exists(name)) return false;
	libconfig::Setting& ar = myCfgFile->lookup(name);
	for(int i = 0; i < ar.getLength(); i++)
	{
		data.push_back((const char*)ar[i]);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::GetArray(const char* name, std::vector<int>& data)
{
	if(!myCfgFile) return false;
	if(!myCfgFile->exists(name)) return false;
	libconfig::Setting& ar = myCfgFile->lookup(name);
	for(int i = 0; i < ar.getLength(); i++)
	{
		data.push_back((int)ar[i]);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::GetArray(const char* name, std::vector<float>& data)
{
	if(!myCfgFile) return false;
	if(!myCfgFile->exists(name)) return false;
	libconfig::Setting& ar = myCfgFile->lookup(name);
	for(int i = 0; i < ar.getLength(); i++)
	{
		data.push_back((float)ar[i]);
	}
	return true;
}

