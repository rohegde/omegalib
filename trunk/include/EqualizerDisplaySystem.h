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
#ifndef __EQUALIZER_DISPLAY_SYSTEM_H__
#define __EQUALIZER_DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "Application.h"
#include "DisplaySystem.h"

#define OM_INIT_EQUALIZER_LOG() eq::base::Log::setOutput(std::ostream(new EqualizerLogStreamBuf())); 

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This class is used to route equalizer log into the omega log system.
class EqualizerLogStreamBuf: public std::streambuf
{
protected:
	virtual int overflow ( int c = EOF )
	{
		if(c == '\n')
		{
			omega::Log::Message(myStringStream.str().c_str());
			myStringStream.str(""); 
		}
		else
		{
			myStringStream << (char)c;
		}
		return 0;
	}
private:
    std::ostringstream myStringStream;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EqualizerDisplaySystem: public DisplaySystem
{
public:
	EqualizerDisplaySystem();
	virtual ~EqualizerDisplaySystem();

	virtual void Initialize(SystemManager* sys); 
	virtual void Run(); 
	virtual void Cleanup(); 

	virtual float GetValue(DisplayParam param);
	virtual unsigned int GetId() { return Id; }

public:
	static const unsigned int Id; 

private:
	SystemManager* mySys;
};

}; // namespace omega

#endif