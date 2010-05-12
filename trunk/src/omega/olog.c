/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Simple logging support. (non-reentrant for now)
 *********************************************************************************************************************/
#include "olog.h"

#define LOG_STR_LEN 1024

FILE* logFile = NULL;

/**********************************************************************************************************************
*/
void ologInitFileOutput(const char* filename)
{
	if(!filename) filename = "./log.txt";
	logFile = fopen(filename, "w");
}

/**********************************************************************************************************************
*/
void ologCleanup()
{
	if(logFile)
	{
		fflush(logFile);
		fclose(logFile);
		logFile = NULL;
	}
}

/**********************************************************************************************************************
*/
void ologMsg(const char* fmt, ...)
{
	char str[LOG_STR_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	strcat(str, "\n");

	printf(str);
	if(logFile)
	{
		fprintf(logFile, str);
	}
}

/**********************************************************************************************************************
*/
void ologWarning(const char* fmt, ...)
{
	char tmpstr[LOG_STR_LEN];
	char str[LOG_STR_LEN];

	va_list args;
	va_start(args, fmt);
	vsprintf(tmpstr, fmt, args);

	sprintf(str, "WARNING: %s\n", tmpstr);

	printf(str);
	if(logFile)
	{
		fprintf(logFile, str);
	}
}

/**********************************************************************************************************************
*/
void ologError(const char* fmt, ...)
{
	char tmpstr[LOG_STR_LEN];
	char str[LOG_STR_LEN];

	va_list args;
	va_start(args, fmt);
	vsprintf(tmpstr, fmt, args);

	sprintf(tmpstr, "ERROR: %s\n", tmpstr);

	printf(str);
	if(logFile)
	{
		fprintf(logFile, str);
	}
}
