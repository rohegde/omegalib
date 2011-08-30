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
#include "omega/StringUtils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
Config::Config(const String& filename): 
	myCfgFile(NULL), 
	myCfgFilename(filename),
	myIsLoaded(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::load()
{
	if(!myIsLoaded)
	{
		myCfgFile = new libconfig::Config();
		bool useFile = true;

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataStream* stream = NULL;
		if(myCfgFilename[0] != '@')
		{
			stream = dm->openStream(myCfgFilename, DataStream::Read);
			if(stream == NULL)
			{
				oferror("Config::Load - Opening file failed: %1%", %myCfgFilename);
				return false;
			}
		}
		else
		{
			useFile = false;
		}

		ofmsg("Opened config file: %1%", %myCfgFilename);

		try
		{
			if(useFile) 
			{
				myCfgFile->read(stream->getCFile());
			}
			else
			{
				const char* str = &myCfgFilename.c_str()[1];
				myCfgFile->readString(str);
			}
		}
		catch(libconfig::ParseException e)
		{
			oferror("Config loading: %1% at line %2%", %e.getError() %e.getLine());
			if(useFile)
			{
				dm->deleteStream(stream);
			}
			return false;
		}
		if(useFile)
		{
			dm->deleteStream(stream);
		}

		myIsLoaded = true;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Setting& Config::getRootSetting()
{
	return myCfgFile->getRoot();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::exists(const String& path)
{
	return myCfgFile->exists(path);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Setting& Config::lookup(const String& path)
{
	return myCfgFile->lookup(path);
}

