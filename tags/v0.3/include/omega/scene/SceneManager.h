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
#include "omega/GpuManager.h"
#include "omega/scene/SceneNode.h"

namespace omega
{
namespace scene
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API SceneManager
	{
	public:
		SceneManager(omega::GpuManager* gpu): 
		  myGpuMng(gpu), 
		  myRoot(NULL),
		  myViewTransform(Matrix4f::IDENTITY),
		  myBackgroundColor(0.1f, 0.1f, 0.1f, 1.0f) {}

		void initialize();

		GpuManager* getGpuManager();
		SceneNode* getRootNode();
		const Matrix4f& getViewTransform();

		void setBackgroundColor(const Color& value);
		Color getBackgroundColor();

		void draw(const Recti& viewport);

	private:
		omega::GpuManager* myGpuMng;
		SceneNode* myRoot;
		Matrix4f myViewTransform;
		Color myBackgroundColor;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline GpuManager* SceneManager::getGpuManager() 
	{ return myGpuMng; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline SceneNode* SceneManager::getRootNode() 
	{ return myRoot; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Matrix4f& SceneManager::getViewTransform() 
	{ return myViewTransform; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void SceneManager::setBackgroundColor(const Color& value)
	{ myBackgroundColor = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color SceneManager::getBackgroundColor()
	{ return myBackgroundColor; }
}; // namespace scene
}; // namespace omega

#endif