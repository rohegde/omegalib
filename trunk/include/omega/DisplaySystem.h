/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2011							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __DISPLAY_SYSTEM_H__
#define __DISPLAY_SYSTEM_H__

#include "osystem.h"
#include "Observer.h"
#include "Application.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class SystemManager;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OMEGA_API DisplaySystem
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

	//! Set the client modelview, projection and viewport
	void setClientTransforms(ApplicationClient* client, const Matrix4f& modelview, const Matrix4f& projection, const Recti& viewport)
	{
		client->setModelViewMatrix(modelview);
		client->setProjectionMatrix(projection);
		client->setViewport(viewport);
	}

protected:
	DisplaySystem() {}
};

}; // namespace omega

#endif