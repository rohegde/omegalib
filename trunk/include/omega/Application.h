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
 * Omegalib configuration
 *********************************************************************************************************************/
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "osystem.h"
#include "SystemManager.h"
#include "InputEvent.h"
#include "DrawContext.h"
#include "Application.h"
#include "UpdateContext.h"

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

	//! Called once for entire application initialization tasks.
	virtual void initialize() {}

	//! Called once for every initialized pipe (a graphic card, or any other resource
	//! seen as one logical graphic card)
	virtual void initializeWindow() {}

	virtual void draw(const DrawContext& context) {}
	virtual bool handleEvent(const InputEvent& evt) { return false; }
	virtual void update(const UpdateContext& context) {}

	SystemManager*  getSystemManager()  { return SystemManager::instance(); }
	InputManager*   getInputManager()   { return SystemManager::instance()->getInputManager(); }
	DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
};

}; // namespace omega

#endif