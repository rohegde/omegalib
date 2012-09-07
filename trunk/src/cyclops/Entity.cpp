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
Entity::Entity(SceneManager* scene):
	SceneNode(scene->getEngine()),
		mySceneManager(scene),
		myOsgNode(NULL),
		myEffect(NULL),
		myStateSet(NULL),
		myAlphaUniform(NULL),
		myAlpha(1.0f),
		myOsgSceneObject(NULL)
{
	myEffect = new EffectNode();
	
	// By default attach new entities to the root node of the scene.
	Engine* engine = mySceneManager->getEngine();
	engine->getScene()->addChild(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::~Entity()
{
	mySceneManager->removeEntity(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::initialize(osg::Node* node)
{
	myOsgNode = node;

	// Create an omegalib scene node. The scene node will be used to manipulate some of this drawable object basic
	// properties like transform and visibility. The scene node also gives access to the object bounding sphere and
	// allows for simple hit tests.
	//Engine* engine = mySceneManager->getEngine();
	//mySceneNode = new SceneNode(engine);
	//engine->getScene()->addChild(mySceneNode);

	myOsgSceneObject = new OsgSceneObject(myOsgNode);
	myEffect->addChild(myOsgSceneObject->getTransformedNode());

	myStateSet = myOsgNode->getOrCreateStateSet();
	myAlphaUniform = new osg::Uniform("unif_Alpha", myAlpha);
	myStateSet->addUniform(myAlphaUniform);

	// OsgSceneObject is the 'glue point' between an osg Node and an omegalib scene node.
	addObject(myOsgSceneObject);

	// Now add this drawable object to the scene.
	mySceneManager->addEntity(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setAlpha(float value)
{
	myAlpha = value;
	if(myAlphaUniform != NULL)
	{
		myAlphaUniform->set(myAlpha);
		if(myAlpha < 1.0)
		{
			if(myStateSet->getRenderingHint() == osg::StateSet::OPAQUE_BIN)
			{
				ofmsg("Entity::setAlpha: entity %1% switched to transparent bin", %getName());
			}
			myStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			myStateSet->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}
		else
		{
			if(myStateSet->getRenderingHint() == osg::StateSet::TRANSPARENT_BIN)
			{
				ofmsg("Entity::setAlpha: entity %1% switched to opaque bin", %getName());
			}
			myStateSet->setRenderingHint(osg::StateSet::OPAQUE_BIN);
			myStateSet->setMode(GL_BLEND, osg::StateAttribute::OFF);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::hasEffect()
{
	return (myEffect != NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setEffect(const String& effectDefinition)
{
	myEffect->setDefinition(effectDefinition);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::followCamera(Camera* cam)
{
	if(myTracker == NULL)
	{
		myTracker = new omegaToolkit::TrackedObject();
		ModuleServices::addModule(myTracker);
		myTracker->setSceneNode(this);
	}
	myTracker->setReferenceCamera(cam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::followTrackable(int trackableId)
{
	if(myTracker == NULL)
	{
		myTracker = new omegaToolkit::TrackedObject();
		ModuleServices::addModule(myTracker);
		myTracker->setSceneNode(this);
	}
	myTracker->setTrackableSourceId(trackableId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setFollowOffset(const Vector3f offset)
{
	myTracker->setOffset(offset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::unfollow()
{
	if(myTracker != NULL)
	{
		ModuleServices::removeModule(myTracker);
		myTracker = NULL;
	}
}
