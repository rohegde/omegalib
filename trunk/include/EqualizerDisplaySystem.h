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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward Declarations.
class EqualizerNodeFactory;
class EqualizerView;

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
			omsg(myStringStream.str().c_str());
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

	virtual unsigned int GetId() { return Id; }

	virtual void SetObserver(int observerId, float x, float y, float z, float yaw, float pitch, float roll);

	// Layer and view management.
	virtual void SetLayerEnabled(int layerNum, const char* viewName, bool enabled);
	virtual bool IsLayerEnabled(int layerNum, const char* viewName);

public:
	static const unsigned int Id; 

private:
	EqualizerView* FindView(const char* viewName);

private:
	SystemManager* mySys;

	// Equalizer stuff.
    EqualizerNodeFactory* myNodeFactory;
	eq::Config* myConfig;
};

}; // namespace omega

#endif