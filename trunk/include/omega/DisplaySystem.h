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
#ifndef __DISPLAY_SYSTEM_H__
#define __DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "Observer.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class SystemManager;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DisplaySystem
{
public:
	virtual ~DisplaySystem() {}

	// sets up the display system. Called before initalize.
	virtual void setup(Setting& setting) {}

	// initializes the display system
	virtual void initialize(SystemManager* sys) {}

	// Starts display system rendering. This call does not return until the current omegalib application sends an
	// exit request to the system manager.
	virtual void run() = 0;

	// Layer and view management.
	virtual void setLayerEnabled(int layerNum, const char* viewName, bool enabled) {}
	virtual bool isLayerEnabled(int layerNum, const char* viewName) { return true;}

	virtual Observer* getObserver(int observerId) { return NULL; }
	
	virtual void cleanup() {}

	virtual unsigned int getId() { return -1; }

protected:
	DisplaySystem() {}
};

}; // namespace omega

#endif