/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __EQUALIZER_DISPLAY_SYSTEM_H__
#define __EQUALIZER_DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "Application.h"
#include "Observer.h"

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward Declarations.
class EqualizerNodeFactory;
class ViewImpl;
class ConfigImpl;

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
class DisplaySystem
{
public:
	OMEGA_API DisplaySystem();
	OMEGA_API ~DisplaySystem();

	// sets up the display system. Called before initalize.
	OMEGA_API void setup(Setting& setting);

	OMEGA_API void initialize(SystemManager* sys); 
	OMEGA_API void run(); 
	OMEGA_API void cleanup(); 

	OMEGA_API String getDisplayConfig() { return myDisplayConfig; }
	OMEGA_API void setDisplayConfig(const String& value) { myDisplayConfig = value; }

	OMEGA_API Observer* getObserver(int observerId);

	// Layer and view management.
	OMEGA_API void setLayerEnabled(int layerNum, const char* viewName, bool enabled);
	OMEGA_API bool isLayerEnabled(int layerNum, const char* viewName);

private:
	void initLayers();
	void initObservers();

private:
	SystemManager* mySys;

	// Display config
	Setting* mySetting;
	String myDisplayConfig;

	// Equalizer stuff.
    EqualizerNodeFactory* myNodeFactory;
	ConfigImpl* myConfig;

	// Observers.
	std::vector<Observer*> myObservers;
};

}; // namespace omega

#endif