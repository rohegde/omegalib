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
#ifndef __SKYBOX_SWITCHER_H__
#define __SKYBOX_SWITCHER_H__

#include "cyclops/SceneManager.h"
#include "cyclops/Skybox.h"

namespace cyclops
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! #PYAPI Implements a panorama viewer, adding a skybox to the scene whose image can be switched
	//! at runtime.
	class CY_API SkyboxSwitcher: public EngineModule
	{
	public:
		struct SkyboxInfo
		{
			Skybox::Type type;
			String filename;
			String extension;
			float horizDegrees;
			float vertDegrees;
			float horizStart;
			float vertStart;
		};

	public:
		static SkyboxSwitcher* createAndInitialize(SceneManager* sceneManager);
		virtual ~SkyboxSwitcher();

		void initialize();
		void update(const UpdateContext& context);
		void handleEvent(const Event& evt);
		void handleCommand(const String& cmd);

		Skybox* getSkybox() { return mySkybox; }

		//! #PYAPI Sets the active skybox image index
		void setActiveSkybox(int index);
		//! #PYAPI Returns the active skybox image index
		int getActiveSkybox();
		//! #PYAPI Returns the number of available skybox images
		int getNumSkyboxes();

	private:
		SceneManager* mySceneManager;

		Ref<Skybox> mySkybox;
		int myActiveSkybox;
		Vector<SkyboxInfo> mySkyboxInfoVector;

	private:
		SkyboxSwitcher(SceneManager* sceneMng);
	};
};
#endif