/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include <osg/Node>
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
		myOsgSceneObject(NULL),
		myCastShadow(true)
{
	// By default attach new entities to the root node of the scene.
	myEffect = new EffectNode();
	Engine* engine = mySceneManager->getEngine();
	//engine->getScene()->addChild(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::~Entity()
{
	removeListener(mySceneManager);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::initialize(osg::Node* node)
{
	myOsgNode = node;

	// Make sure the shadow caster flags are up to date.
	castShadow(myCastShadow);

	// Create an omegalib scene node. The scene node will be used to manipulate some of this drawable object basic
	// properties like transform and visibility. The scene node also gives access to the object bounding sphere and
	// allows for simple hit tests.
	//Engine* engine = mySceneManager->getEngine();
	//mySceneNode = new SceneNode(engine);
	//engine->getScene()->addChild(mySceneNode);

	myOsgSceneObject = new OsgSceneObject(myOsgNode);
	myEffect->addChild(myOsgSceneObject->getTransformedNode());

	// OsgSceneObject is the 'glue point' between an osg Node and an omegalib scene node.
	addComponent(myOsgSceneObject);

	// Now add this drawable object to the scene.
	addListener(mySceneManager);
	getEngine()->getScene()->addChild(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::hasEffect()
{
	return (myEffect != NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::setEffect(const String& effectDefinition)
{
	if(myEffect != NULL)
	{
		myEffect->setDefinition(effectDefinition);
	}
	else
	{
		ofwarn("Entity:setEffect: entity '%1' does not support effects", %getName());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Material* Entity::getMaterial()
{
	return myEffect->getMaterial();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::castShadow(bool value)
{
	myCastShadow = value;
	if(myOsgNode != NULL)
	{
		if(!myCastShadow)
		{
			myOsgNode->setNodeMask(0xffffffff & ~SceneManager::CastsShadowTraversalMask);
		}
		else
		{
			myOsgNode->setNodeMask(0xffffffff);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::doesCastShadow()
{
	return myCastShadow;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
omegaToolkit::ui::Menu* Entity::getContextMenu()
{
	return myContextMenu;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
omegaToolkit::ui::Menu* Entity::createContextMenu()
{
	myContextMenu = mySceneManager->createContextMenu(this);
	return myContextMenu;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::deleteContextMenu()
{
	mySceneManager->deleteContextMenu(this);
	myContextMenu = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Group* Entity::findSubGroup(const String& path)
{
	osg::Group* target = myOsgNode->asGroup();

	// If the head node is not a group, it has no parts for sure.
	if(target == NULL) return NULL;

	// If a non-empty path has been specified, follow it to find a sub-node amd enumerate its pieces.
	if(path != "")
	{
		Vector<String> pathParts = StringUtils::split(path, "/");
		foreach(String pathPart, pathParts)
		{
			osg::Group* newTarget = NULL;
			for(int i = 0; i < target->getNumChildren(); i++)
			{
				if(target->getChild(i)->getName() == pathPart)
				{
					newTarget = target->getChild(i)->asGroup();
					if(newTarget == NULL) return NULL;
				}
			}
			// Have we found the node in the path?
			if(newTarget != NULL) 
			{
				target = newTarget;
			}
			else
			{
				ofwarn("Entity::listPieces: could not find %1% in path %2% for entity %3%", %pathPart %path %getName());
				return NULL;
			}
		}
	}
	return target;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<String> Entity::listPieces(const String& path)
{
	osg::Group* target = findSubGroup(path);
	Vector<String> pieces;
	if(target != NULL)
	{
		// If we are here, we have found a target node to enumerate pieces of.
		for(int i = 0; i < target->getNumChildren(); i++)
		{
			osg::Group* piece = target->getChild(i)->asGroup();
			if(piece != NULL) pieces.push_back(piece->getName());
		}
	}
	return pieces;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* Entity::getPiece(const String& path)
{
	osg::Group* target = findSubGroup(path);
	if(target != NULL)
	{
		SceneNode* sn = new SceneNode(getEngine());
		OsgSceneObject* oso = new OsgSceneObject(target);
		// Use local transforms, since the osg node is already part of a transform hierarchy.
		oso->useLocalTransform(true);
		sn->addComponent(oso);


		addChild(sn);

		// If the osg node is a transform node, copy its transformation to the scene node to
		// preserve it.
		osg::MatrixTransform* mtf = dynamic_cast<osg::MatrixTransform*>(target);
		if(mtf != NULL)
		{
			osg::Vec3d t = mtf->getMatrix().getTrans();
			osg::Vec3d s = mtf->getMatrix().getScale();
			osg::Quat o = mtf->getMatrix().getRotate();
			sn->setPosition(t[0], t[1], t[2]);
			sn->setOrientation(o.w(), o.x(), o.y(), o.z());
			sn->setScale(s[0], s[1], s[2]);
		}
		return sn;
	}
	return NULL;
}
