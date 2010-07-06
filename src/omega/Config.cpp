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
#include "omega/Config.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<char*> Config::stringToArgv(std::string appName, std::string args)
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
bool Config::load()
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
OMEGA_API Setting& Config::getRootSetting()
{
	return myCfgFile->getRoot();
}
