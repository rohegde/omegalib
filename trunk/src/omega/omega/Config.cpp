/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#include "omega/Config.h"
#include "omega/SystemManager.h"
#include "omega/DataManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<char*> Config::stringToArgv(std::string appName, std::string args)
{
	std::istringstream ss(args);
	std::string arg;
	std::vector<std::string> v1;
	std::vector<char*> v2;
	v2.push_back(strdup(appName.c_str()));
	while (ss >> arg)
	{
	   v1.push_back(arg); 
	   v2.push_back(strdup(v1.back().c_str()));
	}
	return v2;
}

Config::Config(const char* filename): myArgv(NULL), myArgc(1), myCfgFile(NULL), myIsLoaded(false)
{
	oassert(filename != NULL);
	myCfgFilename = filename; 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::load()
{
	if(!myIsLoaded)
	{
		myCfgFile = new libconfig::Config();

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataStream* stream = dm->openStream(myCfgFilename, DataStream::Read);
		if(stream == NULL)
		{
			oerror("Config::Load - Opening file failed: %s", myCfgFilename.c_str());
			return false;
		}

		omsg("Opened config file: %s", myCfgFilename.c_str());

		try
		{
			myCfgFile->read(stream->getCFile());
		}
		catch(libconfig::ParseException e)
		{
			oerror("Config loading: %s at line %d", e.getError(), e.getLine());
			dm->deleteStream(stream);
			return false;
		}
		dm->deleteStream(stream);

		myIsLoaded = true;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OMEGA_API Setting& Config::getRootSetting()
{
	return myCfgFile->getRoot();
}
