/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010-2013							Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "ApplicationBase.h"
#include "Color.h"
//#include "Color.h"

namespace omega
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class SystemManager;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OMEGA_API DisplaySystem: public ReferenceType
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

	virtual void cleanup() {}

	virtual DisplaySystemType getId() { return Invalid; }

	virtual void killCluster() {}

	//! Re-applies the display settings to the display system. Depending on the display system,
	//! some settings may not be re-applied at runtime.
	virtual void refreshSettings() {}

	//! Returns the size of the display canvas.
	virtual Vector2i getCanvasSize() = 0;
	//! Returns a view ray given a pointer position in pixel coordinates
	virtual Ray getViewRay(Vector2i position) = 0;
	//! Computes a view ray from a pointer or wand event. Returns true if the ray has been generated succesfully, 
	//! false otherwise (i.e. because the event is not a wand or pointer event)
	virtual bool getViewRayFromEvent(const Event& evt, Ray& ray, bool normalizedPointerCoords = false) { return false; }

	virtual DisplayConfig& getDisplayConfig() { return myDisplayConfig; }

	const Color& getBackgroundColor() { return myBackgroundColor; }
	void setBackgroundColor(const Color& value) { myBackgroundColor = value; }

	void setNearFarZ(float nearz, float farz) { myNearZ = nearz; myFarZ = farz; }
	float getNearZ() { return myNearZ; }
	float getFarZ() { return myFarZ; }

	void drawFps(bool value) { myDrawFps = value; }
	bool isDrawFpsEnabled() { return myDrawFps; }

protected:
	DisplaySystem(): myBackgroundColor(0.2f, 0.2f, 0.2f), myNearZ(0), myFarZ(0), myDrawFps(false) {}

	DisplayConfig myDisplayConfig;

private:
	Color myBackgroundColor;
	float myNearZ;
	float myFarZ;
	bool myDrawFps;
};

}; // namespace omega

#endif