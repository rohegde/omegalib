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
	virtual const char* GetName() { return "OmegaLib " OM_VERSION; }

	virtual void Draw(DrawContext& context) {}
	virtual bool HandleEvent(const InputEvent& evt) { return false; }
	virtual void Update(float dt) {}

	SystemManager*  GetSystemManager()  { return SystemManager::GetInstance(); }
	InputManager*   GetInputManager()   { return SystemManager::GetInstance()->GetInputManager(); }
	DisplaySystem*  GetDisplaySystem() { return SystemManager::GetInstance()->GetDisplaySystem(); }
};

}; // namespace omega

#endif