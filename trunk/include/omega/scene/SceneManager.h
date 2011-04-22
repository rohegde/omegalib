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
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "omega/osystem.h"
#include "omega/Application.h"
#include "omega/GpuManager.h"
#include "omega/scene/SceneNode.h"
#include "omega/scene/Actor.h"
#include "omega/scene/Light.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API SceneManager: public DynamicObject
	{
	public:
		static const int MaxLights = 8;

	public:
		SceneManager(omega::GpuManager* gpu): 
		  myViewTransform(AffineTransform3::Identity()),
		  myGpuMng(gpu), 
		  myRoot(NULL),
		  myBackgroundColor(0.1f, 0.1f, 0.1f, 1.0f) {}

		void initialize();

		GpuManager* getGpuManager();
		SceneNode* getRootNode();
		const AffineTransform3& getViewTransform();

		void setBackgroundColor(const Color& value);
		Color getBackgroundColor();

		void draw(const DrawContext& context);
		void update(const UpdateContext& context);
		bool handleEvent(const Event& evt, UpdateContext& context);
		bool handleEvent(const Event& evt, DrawContext& context);

		void addActor(Actor* actor);
		void removeActor(Actor* actor);

		void addRenderPass(RenderPass* pass, bool addToFront = false);
		void removeRenderPass(RenderPass* pass);

		Light* getLight(int num);

	private:
		AffineTransform3 myViewTransform;

		omega::GpuManager* myGpuMng;
		SceneNode* myRoot;
		Color myBackgroundColor;

		Light myLights[MaxLights];
		List<Actor*> myActors;
		List<RenderPass*> myRenderPassList;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline GpuManager* SceneManager::getGpuManager() 
	{ return myGpuMng; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneNode* SceneManager::getRootNode() 
	{ return myRoot; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AffineTransform3& SceneManager::getViewTransform() 
	{ return myViewTransform; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneManager::setBackgroundColor(const Color& value)
	{ myBackgroundColor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color SceneManager::getBackgroundColor()
	{ return myBackgroundColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Light* SceneManager::getLight(int num)
	{ return &myLights[num]; }
}; // namespace scene
}; // namespace omega

#endif