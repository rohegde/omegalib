/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __ENGINE_CLIENT_H__
#define __ENGINE_CLIENT_H__

#include "osystem.h"
//#include "omega/Application.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class TextureManager;
	class GpuManager;
	class FontManager;
	namespace scene
	{
		class SceneManager;
		class MeshManager;
		class EffectManager;
	};
	namespace ui
	{
		class UIManager;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API EngineClient//: public ApplicationClient
	{
	public:
		enum DrawFlags { DrawScene = 1 << 1, DrawUI = 1 << 2 };

	public:
		EngineClient(ApplicationClient* client):
		  myClient(client),
		  myGpuManager(NULL),
		  myTextureManager(NULL),
		  myFontManager(NULL),
		  mySceneManager(NULL),
		  myMeshManager(NULL),
		  myEffectManager(NULL),
		  myUIManager(NULL) {}

		void initialize();
		ApplicationClient* getApplicationClient();
		void update(const UpdateContext& context);
		bool handleEvent(const Event& evt, UpdateContext& context);
		bool handleEvent(const Event& evt, DrawContext& context);
		void draw(const DrawContext& context, uint flags);

		//! Manager getters.
		//@{
		GpuManager* getGpuManager();
		TextureManager* getTextureManager();
		scene::SceneManager* getSceneManager();
		scene::EffectManager* getEffectManager();
		scene::MeshManager* getMeshManager();
		ui::UIManager* getUIManager();
		FontManager* getFontManager();
		//@}

	private:
		ApplicationClient* myClient;
		GpuManager* myGpuManager;
		TextureManager* myTextureManager;
		FontManager* myFontManager;
		scene::SceneManager* mySceneManager;
		scene::MeshManager* myMeshManager;
		scene::EffectManager* myEffectManager;
		ui::UIManager* myUIManager;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ApplicationClient* EngineClient::getApplicationClient()
	{ return myClient; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline GpuManager* EngineClient::getGpuManager()
	{ return myGpuManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline TextureManager* EngineClient::getTextureManager()
	{ return myTextureManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline scene::SceneManager* EngineClient::getSceneManager()
	{ return mySceneManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline scene::MeshManager* EngineClient::getMeshManager()
	{ return myMeshManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline FontManager* EngineClient::getFontManager()
	{ return myFontManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline scene::EffectManager* EngineClient::getEffectManager()
	{ return myEffectManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ui::UIManager* EngineClient::getUIManager()
	{ return myUIManager; }
}; // namespace omega

#endif