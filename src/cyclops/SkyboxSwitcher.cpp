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
#include "cyclops/SkyboxSwitcher.h"

using namespace omega;
using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
SkyboxSwitcher* SkyboxSwitcher::createAndInitialize(SceneManager* sceneMng)
{
	SkyboxSwitcher* instance = new SkyboxSwitcher(sceneMng);
	ModuleServices::addModule(instance);
	instance->doInitialize(Engine::instance());
	return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SkyboxSwitcher::SkyboxSwitcher(SceneManager* sceneMng):
	EngineModule("SkyboxSwitcher"),
	mySceneManager(sceneMng),
	myActiveSkybox(-1)
{
	mySkybox = new Skybox();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SkyboxSwitcher::~SkyboxSwitcher()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SkyboxSwitcher::initialize()
{
	// Parse the skybox data from the settings
	omsg("SkyboxSwitcher initializing...");
	if(SystemManager::settingExists("config/SkyboxSwitcher"))
	{
		Setting& sSkyboxes = SystemManager::settingLookup("config/SkyboxSwitcher");
		for(int i = 0; i < sSkyboxes.getLength(); i++)
		{
			Setting& sSkybox = sSkyboxes[i];
			SkyboxInfo info;

			String type = Config::getStringValue("type", sSkybox, "cube");
			String filename = Config::getStringValue("filename", sSkybox, "cube");
			info.filename = filename;
			if(type == "cube")
			{
				info.type = Skybox::CubeMap;
				String ext = Config::getStringValue("extension", sSkybox, "jpg");
				info.extension = ext;
			}
			else if(type == "pano")
			{
				info.type = Skybox::Pano;
			}
			mySkyboxInfoVector.push_back(info);
		}
	}

	mySceneManager->setSkyBox(mySkybox);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SkyboxSwitcher::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SkyboxSwitcher::handleEvent(const Event& evt)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SkyboxSwitcher::handleCommand(const String& cmd)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SkyboxSwitcher::setActiveSkybox(int index)
{
	myActiveSkybox = index;
	if(myActiveSkybox >= 0 && myActiveSkybox < getNumSkyboxes())
	{
		SkyboxInfo& info = mySkyboxInfoVector[myActiveSkybox];
		if(info.type == Skybox::CubeMap)
		{
			mySkybox->loadCubeMap(info.filename, info.extension);
		}
		else
		{
			mySkybox->loadPano(info.filename);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int SkyboxSwitcher::getActiveSkybox()
{
	return myActiveSkybox;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int SkyboxSwitcher::getNumSkyboxes()
{
	return mySkyboxInfoVector.size();
}