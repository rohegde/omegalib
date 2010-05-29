/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Log class members implementation. See Log.h for more details.
 *********************************************************************************************************************/
#include "osystem.h"

#define LOG_STR_LEN 1024

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE* Log::myLogFile = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Initialize(const char* filename)
{
	if(!filename) filename = "./log.txt";
	myLogFile = fopen(filename, "w");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Terminate()
{
	if(myLogFile)
	{
		fflush(myLogFile);
		fclose(myLogFile);
		myLogFile = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Message(const char* fmt, ...)
{
	char str[LOG_STR_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	strcat(str, "\n");

	printf(str);
	if(myLogFile)
	{
		fprintf(myLogFile, str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Warning(const char* fmt, ...)
{
	char tmpstr[LOG_STR_LEN];
	char str[LOG_STR_LEN];

	va_list args;
	va_start(args, fmt);
	vsprintf(tmpstr, fmt, args);

	sprintf(str, "WARNING: %s\n", tmpstr);

	printf(str);
	if(myLogFile)
	{
		fprintf(myLogFile, str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Error(const char* fmt, ...)
{
	char tmpstr[LOG_STR_LEN];
	char str[LOG_STR_LEN];

	va_list args;
	va_start(args, fmt);
	vsprintf(tmpstr, fmt, args);

	sprintf(str, "WARNING: %s\n", tmpstr);

	printf(str);
	if(myLogFile)
	{
		fprintf(myLogFile, str);
	}
}

