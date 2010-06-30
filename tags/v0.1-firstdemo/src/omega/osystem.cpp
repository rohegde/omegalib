/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/Utils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LOG_STR_LEN 1024

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE* sLogFile = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ologopen(const char* filename)
{
	if(!filename) filename = "./log.txt";
	sLogFile = fopen(filename, "w");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ologclose()
{
	if(sLogFile)
	{
		fflush(sLogFile);
		fclose(sLogFile);
		sLogFile = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oerror(const char* fmt, ...)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oexit(int code)
{
	exit(code);
}
