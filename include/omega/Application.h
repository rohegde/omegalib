/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Omegalib configuration
 *********************************************************************************************************************/
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "osystem.h"
#include "SystemManager.h"
#include "InputEvent.h"
#include "DrawContext.h"
#include "Application.h"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class SystemManager;
class InputManager;
class DisplaySystem;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Application
{
public:
	static const int MaxLayers = 16;

public:
	virtual const char* getName() { return "OmegaLib " OMEGA_VERSION; }

	virtual void draw(DrawContext& context) {}
	virtual bool handleEvent(const InputEvent& evt) { return false; }
	virtual void update(float dt) {}

	SystemManager*  getSystemManager()  { return SystemManager::instance(); }
	InputManager*   getInputManager()   { return SystemManager::instance()->getInputManager(); }
	DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
};

}; // namespace omega

#endif