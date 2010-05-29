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
	virtual ~DisplaySystem() {}

	// Initializes the display system
	virtual void Initialize(SystemManager* sys) {}

	// Starts display system rendering. This call does not return until the current omegalib application sends an
	// exit request to the system manager.
	virtual void Run() = 0;

	virtual void Cleanup() {}

	virtual unsigned int GetId() { return -1; }

protected:
	DisplaySystem() {}
};

}; // namespace omega

#endif