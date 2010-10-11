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

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class SystemManager;
	class InputManager;
	class DisplaySystem;
	class Application;
	class ChannelImpl;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Interface for objects that manage a single OpenGL context.
	class IGLContextManager
	{
	public:
		//! Makes the context current.
		virtual void makeCurrent() = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct UpdateContext
	{
		int frameNum;
		float dt;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct DrawContext
	{
		unsigned int frameNum;
		unsigned int layer;
		int viewportX;
		int viewportY;
		int viewportWidth;
		int viewportHeight;
		IGLContextManager* glContext;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ApplicationClient
	{
	public:
		ApplicationClient(Application* app): myApplication(app) {}
		virtual ~ApplicationClient() {}

		virtual void setup() {}
		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual void draw(const DrawContext& context) {}

		Application* getApplication() { return myApplication; }

	protected:
		void resetGLContext();

	private:
		Application* myApplication;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ApplicationServer
	{
	public:
		ApplicationServer(Application* app): myApplication(app) {}
		virtual ~ApplicationServer() {}

		virtual void initialize() {}
		virtual void update(const UpdateContext& context) {}
		virtual bool handleEvent(const InputEvent& evt) { return false; }

		Application* getApplication() { return myApplication; }

	private:
		Application* myApplication;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Application
	{
	public:
		static const int MaxLayers = 16;

	public:
		virtual const char* getName() { return "OmegaLib " OMEGA_VERSION; }

		//! Instantiates a new Channel instance.
		//! Users redefine this method to create instances of their own Channel objects.
		//! @param impl - the internal DisplaySystem-dependent channel implementation.
		virtual ApplicationServer* createServer() = 0;
		virtual ApplicationClient* createClient() = 0;

		//! Called once for entire application initialization tasks.
		virtual void initialize() {}

		SystemManager*  getSystemManager()  { return SystemManager::instance(); }
		InputManager*   getInputManager()   { return SystemManager::instance()->getInputManager(); }
		DisplaySystem*  getDisplaySystem() { return SystemManager::instance()->getDisplaySystem(); }
	};
}; // namespace omega

#endif