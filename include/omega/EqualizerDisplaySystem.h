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
	OMEGA_API EqualizerDisplaySystem();
	OMEGA_API virtual ~EqualizerDisplaySystem();

	OMEGA_API virtual void initialize(SystemManager* sys); 
	OMEGA_API virtual void run(); 
	OMEGA_API virtual void cleanup(); 

	OMEGA_API virtual unsigned int getId() { return Id; }

	OMEGA_API virtual Observer* getObserver(int observerId);

	// Layer and view management.
	OMEGA_API virtual void setLayerEnabled(int layerNum, const char* viewName, bool enabled);
	OMEGA_API virtual bool isLayerEnabled(int layerNum, const char* viewName);

public:
	static const unsigned int Id; 

private:
	EqualizerView* findView(const char* viewName);

private:
	SystemManager* mySys;

	// Equalizer stuff.
    EqualizerNodeFactory* myNodeFactory;
	eq::Config* myConfig;

	// Observers.
	std::vector<Observer*> myObservers;
};

}; // namespace omega

#endif