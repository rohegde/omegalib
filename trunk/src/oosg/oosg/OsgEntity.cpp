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
#include "omega/StringUtils.h"
#include "omega/DataManager.h"
#include "oosg/OsgEntity.h"
#include "oosg/OsgRenderable.h"

#include <osgUtil/Optimizer>
#include <osgUtil/UpdateVisitor>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/FrameStamp>

using namespace oosg;

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgEntity::OsgEntity():
	myRepresentationSize(0.1)
{
    myFrameStamp = new osg::FrameStamp;
    myUpdateVisitor = new osgUtil::UpdateVisitor;
    myUpdateVisitor->setFrameStamp( myFrameStamp );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgEntity::~OsgEntity()
{
	if(myModel != NULL)
	{
		myModel->unref();
		myModel = NULL;
	}
    myFrameStamp->unref();
	myFrameStamp = NULL;
    myUpdateVisitor->unref();
	myUpdateVisitor = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgEntity::load(const String& filename)
{
	myModel = NULL;
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo cfgInfo = dm->getInfo(filename);
	if(!cfgInfo.isNull())
	{
		myModel = osgDB::readNodeFile(cfgInfo.path);
		if ( myModel == NULL) 
		{
			oferror("Failed to load model: %1%", %filename);
		}

		//Optimize scenegraph
		osgUtil::Optimizer optOSGFile;
		optOSGFile.optimize(myModel);
	}
	else
	{
		oferror("File not found: %1%", %filename);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgEntity::addToScene(SceneNode* node)
{
	oassert(node);

	OsgRenderable* renderable;
	BoundingSphere* bsphere = onew(BoundingSphere)();
	bsphere->setVisible(false);
	bsphere->setDrawOnSelected(true);
	node->addRenderable(bsphere);

	renderable = onew(OsgRenderable)(myModel);
	node->addRenderable(renderable);

	node->update(true, false);
	const Sphere& bs = node->getBoundingSphere();

	float scale = myRepresentationSize / bs.getRadius();
	node->scale(scale, scale, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgEntity::update(const UpdateContext& context)
{
	myFrameStamp->setFrameNumber(context.frameNum);

	const double time = static_cast< double >( context.time ) / 1000.;
    myFrameStamp->setReferenceTime(time);
    myFrameStamp->setSimulationTime(time);
	myUpdateVisitor->setTraversalNumber(context.frameNum);
    myModel->accept(*myUpdateVisitor);
    myModel->getBound();
}

