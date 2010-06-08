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
	//v2.add(0);  // need terminating null pointer
	return v2;
}
