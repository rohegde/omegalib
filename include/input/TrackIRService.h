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

#include "osystem.h"
#include "InputManager.h"

#include "optitrack.h"
#import  "optitrack.tlb"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TrackIRService: public InputService
{
public:

	virtual void Initialize();
	virtual void Start();
	virtual void Poll();
	virtual void Stop();
	virtual void Dispose();

private:
    CComPtr<INPCamera>	myCamera;
	CComPtr<INPVector3> myVector;
};

}; // namespace omega

#endif