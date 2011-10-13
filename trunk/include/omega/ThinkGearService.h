/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto							anishimoto42@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * 
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __THINKGEAR__SERVICE_H__
#define __THINKGEAR__SERVICE_H__

#include "omega/osystem.h"
#include "omega/ServiceManager.h"
#include "omega/thinkgear.h"

#include <stdlib.h>
#include <stdio.h>

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OMEGA_API ThinkGearService: public Service
{
public:
	// Allocator function
	static ThinkGearService* New() { return new ThinkGearService(); }

public:
	void setup(Setting& settings);
	virtual void initialize();
	virtual void poll();
	virtual void dispose();

	void generateEvent(int);

	//! Sets the data update interval, in seconds. This is the interval at which this service will generate events
	//! If set to zero, the service will generate events as fast as possible.
	void setUpdateInterval(float value);
	//! @see setUpdateInterval
	float getUpdateInterval();

private:
	static ThinkGearService* mysInstance;
	float myUpdateInterval;

	static int driverVersion;

	static const char *comPortName;
	static int connectionID;

};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void ThinkGearService::setUpdateInterval(float value) 
	{ myUpdateInterval = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float ThinkGearService::getUpdateInterval() 
	{ return myUpdateInterval; }
}; // namespace omega

#endif