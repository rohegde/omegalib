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
#ifndef __CY_ENTITY__
#define __CY_ENTITY__

#include "cyclopsConfig.h"
#include "SkyBox.h"

#include <osg/Texture2D>
#include <osg/Light>
#include <osg/Group>
#include <osg/Switch>
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgAnimation/BasicAnimationManager>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaOsg.h>
#include <omegaToolkit.h>

#include "omega/PortholeTabletService.h"
#include "SceneManager.h"

namespace cyclops {
	using namespace omega;
	using namespace omegaOsg;

	class SceneManager;

	///////////////////////////////////////////////////////////////////////////////////////////////
	struct EntityEventCallbacks
	{
		String onAdd;
		String onUpdate;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class CY_API Entity
	{
	public:
		Entity(SceneManager* mng, ModelAsset* asset, int id);

		osg::Node* getOsgNode() { return myOsgNode; }
		SceneNode* getSceneNode() { return mySceneNode; }
		ModelAsset* getAsset() { return myAsset; }
		int getId() { return myId; }

		int getNumModels() { return myAsset->numNodes; }
		void setCurrentModelIndex(int index);
		int getCurrentModelIndex();

		void setTag(const String& value) { myTag = value; }
		const String& getTag() { return myTag; } 

		//! Animation support
		//@{
		bool hasAnimations();
		int getNumAnimations();
		void playAnimation(int id);
		void loopAnimation(int id);
		void pauseAnimation(int id);
		void stopAllAnimations();
		//@}

		void setEventCallbacks(const EntityEventCallbacks& eec);

	private:
		SceneManager* mySceneManager;

		osg::Node* myOsgNode;
		osg::Switch* myOsgSwitch;
		int myCurrentModelIndex;

		SceneNode* mySceneNode;
		ModelAsset* myAsset;
		int myId;
		String myTag;

		// osg animation stuff
		osgAnimation::BasicAnimationManager* myAnimationManager;
		const osgAnimation::AnimationList* myAnimations;

		// Callbacks
		EntityEventCallbacks myCallbacks;
	};
};

#endif
