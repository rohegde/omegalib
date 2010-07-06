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
#ifndef __TRACKIR__SERVICE_H__
#define __TRACKIR__SERVICE_H__

#include "omega/osystem.h"
#include "omega/InputManager.h"

#include "optitrack/optitrack.h"
#import  "optitrack/optitrack.tlb"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OptiTrackService: public InputService
{
public:
	// Allocator function
	static OptiTrackService* New() { return new OptiTrackService(); }

public:

	OMEGA_API virtual void initialize();
	OMEGA_API virtual void start();
	OMEGA_API virtual void poll();
	OMEGA_API virtual void stop();
	OMEGA_API virtual void dispose();

private:
    CComPtr<INPCamera>	myCamera;
	CComPtr<INPVector3> myVector;
};

}; // namespace omega

#endif