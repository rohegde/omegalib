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
#include "omega/Utils.h"
#include "omega/Application.h"
#include "omega/Config.h"
#include "omega/DataManager.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
#define LOG_STR_LEN 65536

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
FILE* sLogFile = NULL;

#ifdef OMEGA_USE_DISPLAY
GLEWContext* sGlewContext;

///////////////////////////////////////////////////////////////////////////////////////////////////
GLEWContext* glewGetContext()
{
	return sGlewContext;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void glewSetContext(const GLEWContext* context)
{
	sGlewContext = (GLEWContext*)context;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
void ologopen(const char* filename)
{
	if(!filename) filename = "./log.txt";
	sLogFile = fopen(filename, "w");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ologclose()
{
	if(sLogFile)
	{
		fflush(sLogFile);
		fclose(sLogFile);
		sLogFile = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void omsg(const char* fmt, ...)
{
	char str[LOG_STR_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	strcat(str, "\n");

	printf(str);
	if(sLogFile)
	{
		fprintf(sLogFile, str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void owarn(const char* fmt, ...)
{
	char tmpstr[LOG_STR_LEN];
	char str[LOG_STR_LEN];

	va_list args;
	va_start(args, fmt);
	vsprintf(tmpstr, fmt, args);

	sprintf(str, "WARNING: %s\n", tmpstr);

	printf(str);
	if(sLogFile)
	{
		fprintf(sLogFile, str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void oerror(const char* fmt, ...)
{
	char tmpstr[LOG_STR_LEN];
	char str[LOG_STR_LEN];

	va_list args;
	va_start(args, fmt);
	vsprintf(tmpstr, fmt, args);

	sprintf(str, "ERROR: %s\n", tmpstr);

	printf(str);
	if(sLogFile)
	{
		fprintf(sLogFile, str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void oexit(int code)
{
	exit(code);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void omain(Application& app, const char* configFile, const char* logFile, int dataSources, ...)
{
	ologopen(logFile);

	Config* cfg = new Config(configFile);

	SystemManager* sys = SystemManager::instance();

	// Set the data sources.
	va_list ap;
    va_start(ap, dataSources); 
    for(int j = 0; j < dataSources; j++)
	{
		DataSource* src = va_arg(ap, DataSource*);
		sys->getDataManager()->addSource(src);
	}
    va_end(ap);

	sys->setup(cfg);
	sys->setApplication(&app);
	sys->initialize();
	sys->run();

	sys->cleanup();

	ologclose();
}
