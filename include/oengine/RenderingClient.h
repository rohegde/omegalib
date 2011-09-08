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
#ifndef __RENDERINGCLIENT_H__
#define __RENDERINGCLIENT_H__

#include "oenginebase.h"
#include "omega/Application.h"
#include "omega/GpuManager.h"
#include "oengine/SceneNode.h"
#include "oengine/Actor.h"
#include "oengine/Light.h"

namespace oengine {
	class Renderer;
	class EngineServer;
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API RenderingClient: public DynamicObject
	{
	public:
		static const int MaxLights = 8;

	public:
		SceneManager(GpuManager* gpu): 
		  myGpuMng(gpu), 
		  myRoot(NULL),
		  myAmbientLightColor(0.3f, 0.3f, 0.4f, 1.0f) {}

		void initialize();

		GpuManager* getGpuManager();
		SceneNode* getRootNode();

		void setAmbientLightColor(const Color& value);
		Color getAmbientLightColor();

		void draw(const DrawContext& context);

		void addRenderPass(RenderPass* pass, bool addToFront = false);
		void removeRenderPass(RenderPass* pass);

		Light* getLight(int num);

		Renderer* getRenderer();

	private:
		GpuManager* myGpuMng;
		SceneNode* myRoot;

		Light myLights[MaxLights];
		Color myAmbientLightColor;

		List<RenderPass*> myRenderPassList;

		Renderer* myDefaultRenderer;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline GpuManager* SceneManager::getGpuManager() 
	{ return myGpuMng; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneNode* SceneManager::getRootNode() 
	{ return myRoot; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneManager::setAmbientLightColor(const Color& value)
	{ myAmbientLightColor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color SceneManager::getAmbientLightColor()
	{ return myAmbientLightColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Light* SceneManager::getLight(int num)
	{ return &myLights[num]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Renderer* SceneManager::getRenderer()
	{ return myDefaultRenderer; }
}; // namespace oengine

#endif