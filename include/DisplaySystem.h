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

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class SystemManager;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DisplaySystem
{
public:
	// Supported display system parameters.
	enum DisplayParam { GlobalWidth, GlobalHeight, LocalWidth, LocalHeight };

public:
	virtual ~DisplaySystem() {}

	// Initializes the display system
	virtual void Initialize(SystemManager* sys) {}

	// Starts display system rendering. This call does not return until the current omegalib application sends an
	// exit request to the system manager.
	virtual void Run() = 0;

	// Layer and view management.
	virtual void SetLayerEnabled(int layerNum, const char* viewName, bool enabled) {}
	virtual bool IsLayerEnabled(int layerNum, const char* viewName) { return true;}
	virtual bool IsLayerEnabled(int layerNum) { return IsLayerEnabled(layerNum, GetActiveView());}
	virtual const char* GetActiveView() { return "<undefined>"; }
	
	// Returns the value of the specified parameter for the display system.
	virtual float GetValue(DisplayParam param) = 0;

	virtual void Cleanup() {}

	virtual unsigned int GetId() { return -1; }

protected:
	DisplaySystem() {}
};

}; // namespace omega

#endif