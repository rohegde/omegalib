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
#include <osgUtil/Optimizer>
#include <osgDB/Archive>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osgAnimation/Animation>

#include "cyclops/SceneManager.h"
#include "cyclops/Entity.h"

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct AnimationManagerFinder : public osg::NodeVisitor 
{ 
    osgAnimation::BasicAnimationManager* am; 
    
    AnimationManagerFinder(): am(NULL) {setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN); } 
    
    void apply(osg::Node& node) { 
    
        if (am != NULL) 
            return; 
    
        if (node.getUpdateCallback()) {        
            am = dynamic_cast<osgAnimation::BasicAnimationManager*>(node.getUpdateCallback()); 
            return; 
        } 
        
        traverse(node); 
    } 
}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(SceneManager* mng, ModelAsset* asset, int id):
	mySceneManager(mng), 
	myAsset(asset), 
	myId(id), 
	myOsgSwitch(NULL), 
	myCurrentModelIndex(0),
	myAnimationManager(NULL),
	myAnimations(NULL)
{
	ServerEngine* engine = mng->getEngine();

	mySceneNode = new SceneNode(engine);
	mySceneNode->setSelectable(true);
	engine->getScene()->addChild(mySceneNode);

	OsgSceneObject* oso = NULL;
	if(asset->numNodes == 1)
	{
		// Single model asset
		oso = new OsgSceneObject(myAsset->nodes[0]);
		myOsgNode = oso->getTransformedNode();
	}
	else
	{
		// Multi model asset
		myOsgSwitch = new osg::Switch();
		int i = 0;
		foreach(osg::Node* n, myAsset->nodes)
		{
			myOsgSwitch->addChild(n);
			myOsgSwitch->setChildValue(n, i++);
		}
		oso = new OsgSceneObject(myOsgSwitch);
		myOsgNode = oso->getTransformedNode();
	}

	AnimationManagerFinder amf;
	myOsgNode->accept(amf);
	if(amf.am != NULL)
	{
		myAnimationManager = amf.am;
		myAnimations = &myAnimationManager->getAnimationList();
		ofmsg("Entity id %1%: found %2% animations.", %myId %getNumAnimations());

		loopAnimation(0);
	}

	mySceneNode->addObject(oso);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setCurrentModelIndex(int index)
{
	if(myOsgSwitch != NULL && index < getNumModels())
	{
		myCurrentModelIndex = index;
		myOsgSwitch->setSingleChildOn(index);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Entity::getCurrentModelIndex()
{
	if(myOsgSwitch != NULL)
	{
		return myCurrentModelIndex;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::hasAnimations()
{
	return myAnimationManager != NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Entity::getNumAnimations()
{
	if(hasAnimations())
	{
		return myAnimations->size();
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::playAnimation(int id)
{
	if(hasAnimations())
	{
		if(id < getNumAnimations())
		{
			osgAnimation::Animation* anim = myAnimations->at(id);
			anim->setPlayMode(osgAnimation::Animation::ONCE);
			myAnimationManager->playAnimation(anim);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::loopAnimation(int id)
{
	if(hasAnimations())
	{
		if(id < getNumAnimations())
		{
			osgAnimation::Animation* anim = myAnimations->at(id);
			anim->setPlayMode(osgAnimation::Animation::LOOP);
			myAnimationManager->playAnimation(anim);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::pauseAnimation(int id)
{
	if(hasAnimations())
	{
		if(id < getNumAnimations())
		{
			osgAnimation::Animation* anim = myAnimations->at(id);
			myAnimationManager->stopAnimation(anim);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::stopAllAnimations()
{
	if(hasAnimations())
	{
		myAnimationManager->stopAll();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setEventCallbacks(const EntityEventCallbacks& eec)
{
	//myCallbacks = eec;
	//if(eec.onAdd != "")
	//{
	//	eec.onAdd = ostr(eec.onAdd, %myId);
	//}
}

