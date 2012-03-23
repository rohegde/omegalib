/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
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
#ifndef __EQUALIZER_DISPLAY_SYSTEM_H__
#define __EQUALIZER_DISPLAY_SYSTEM_H__

#include "DisplaySystem.h"
#include "Application.h"

// Equalizer includes
#include <eq/eq.h>

namespace omega
{
//typedef co::base::uint128_t uint128_t;

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
class EqualizerDisplaySystem: public DisplaySystem
{
public:
	OMEGA_API EqualizerDisplaySystem();
	OMEGA_API ~EqualizerDisplaySystem();

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

	virtual DisplaySystemType getId() { return DisplaySystem::Equalizer; }

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