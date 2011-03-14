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
#include "omega/Application.h"
#include "omega/Config.h"
#include "omega/DataManager.h"
#include "omega/FilesystemDataSource.h"
#include "omega/StringUtils.h"

namespace omega
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	FILE* sLogFile = NULL;

	GLEWContext* sGlewContext;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	GLEWContext* glewGetContext()
	{
		return sGlewContext;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void glewSetContext(const GLEWContext* context)
	{
		sGlewContext = (GLEWContext*)context;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void ologopen(const char* filename)
	{
		if(!filename) filename = "./log.txt";
		sLogFile = fopen(filename, "w");
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void ologclose()
	{
		if(sLogFile)
		{
			fflush(sLogFile);
			fclose(sLogFile);
			sLogFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void omsg(const String& str)
	{
		printf("%s\n", str.c_str());
		if(sLogFile)
		{
			fprintf(sLogFile, "%s\n", str.c_str());
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void owarn(const String& str)
	{
		printf("WARNING: %s\n", str.c_str());
		if(sLogFile)
		{
			fprintf(sLogFile, "WARNING: %s\n", str.c_str());
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void oerror(const String& str)
	{
		printf("ERROR: %s\n", str.c_str());
		if(sLogFile)
		{
			fprintf(sLogFile, "ERROR: %s\n", str.c_str());
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void oabort(const char* file, int line, const char* reason)
	{
		String filename;
		String path;

		StringUtils::splitFilename(file, filename, path);

		ofmsg("Assertion failed at %1%:%2% - %3%", %file %line %reason);

		abort();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void oexit(int code)
	{
		exit(code);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void omain(Application& app, const char* configFile, const char* logFile, DataSource* dataSource)
	{
		ologopen(logFile);

		Config* cfg = new Config(configFile);

		SystemManager* sys = SystemManager::instance();
		DataManager* dm = sys->getDataManager();
		// Add a default filesystem data source using current work dir.
		dm->addSource(new FilesystemDataSource("./"));

		// Add optional data source.
		if(dataSource != NULL)
		{
			dm->addSource(dataSource);
		}

		sys->setup(cfg);
		sys->setApplication(&app);
		sys->initialize();
		sys->run();

		sys->cleanup();

		ologclose();
	}
}
