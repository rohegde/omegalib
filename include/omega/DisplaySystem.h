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
	enum DisplaySystemType { Invalid, Equalizer, Glut };

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

	virtual DisplaySystemType getId() { return Invalid; }

protected:
	DisplaySystem() {}
};

}; // namespace omega

#endif