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

#include "oengine/oenginebase.h"
#include "oengine/SceneManager.h"
#include "oengine/FontManager.h"
#include "oengine/MeshManager.h"
#include "oengine/EffectManager.h"
#include "oengine/ui/UiManager.h"

#include "omega/Application.h"
#include "omega/TextureManager.h"

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API EngineClient: public ApplicationClient
	{
	public:
		EngineClient(ApplicationServer* server):
		  ApplicationClient(server),
		  myGpuManager(NULL),
		  myTextureManager(NULL),
		  myFontManager(NULL),
		  mySceneManager(NULL),
		  myMeshManager(NULL),
		  myEffectManager(NULL),
		  myUiManager(NULL),
		  myTextureBackgroundEnabled(false),
		  myLeftBackgroundTexture(NULL),
		  myRightBackgroundTexture(NULL) {}

		virtual void initialize();
		virtual void handleEvent(const Event& evt);
		virtual void update(const UpdateContext& context);
		virtual void draw(const DrawContext& context);

		//! Manager getters.
		//@{
		GpuManager* getGpuManager();
		TextureManager* getTextureManager();
		SceneManager* getSceneManager();
		EffectManager* getEffectManager();
		MeshManager* getMeshManager();
		ui::UiManager* getUiManager();
		FontManager* getFontManager();
		bool isTextureBackgroundEnabled();
		void setTextureBackgroundEnabled(bool value);
		void setTextureBackground(DrawContext::Eye eye, Texture* texture);	
		Texture* getTextureBackground(DrawContext::Eye eye);	
		//@}

	private:
		void drawBackGrd( const DrawContext& theContext );
	
		GpuManager* myGpuManager;
		TextureManager* myTextureManager;
		FontManager* myFontManager;
		SceneManager* mySceneManager;
		MeshManager* myMeshManager;
		EffectManager* myEffectManager;
		ui::UiManager* myUiManager;

		bool myTextureBackgroundEnabled;
		Texture* myLeftBackgroundTexture;
		Texture* myRightBackgroundTexture;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline GpuManager* EngineClient::getGpuManager()
	{ return myGpuManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline TextureManager* EngineClient::getTextureManager()
	{ return myTextureManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneManager* EngineClient::getSceneManager()
	{ return mySceneManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline MeshManager* EngineClient::getMeshManager()
	{ return myMeshManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline FontManager* EngineClient::getFontManager()
	{ return myFontManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline EffectManager* EngineClient::getEffectManager()
	{ return myEffectManager; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline ui::UiManager* EngineClient::getUiManager()
	{ return myUiManager; }
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool EngineClient::isTextureBackgroundEnabled()
	{ return myTextureBackgroundEnabled; }
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void EngineClient::setTextureBackgroundEnabled(bool value)
	{ myTextureBackgroundEnabled = value; }
}; // namespace oengine

#endif
