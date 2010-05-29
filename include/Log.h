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
#ifndef __OLOG_H__
#define __OLOG_H__

#define ODBG(str) Log::Message(str);
#define OASSERT(c) if(!(c)) { Log::Error("Assertion failed at %s:%d - %s", __FILE__, __LINE__, ##c); exit(1); }

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Log
{
public:
	static void Initialize(const char* filename = NULL);
	static void Terminate();

	static void Message(const char* fmt, ...);
	static void Warning(const char* fmt, ...);
	static void Error(const char* fmt, ...);
private:
	Log() {}
	~Log() {}

private:
	static FILE* myLogFile;
};

}; // namespace omega

#endif