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
#include "omegaOsg/OsgModule.h"
#include "omegaOsg/OsgRenderPass.h"
#include "omegaOsg/OsgSceneObject.h"

#include <osgUtil/Optimizer>
#include <osgUtil/UpdateVisitor>
#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <osg/Node>
#include <osg/FrameStamp>

#include "omegaOsg/ReaderFreeImage.h"

using namespace omegaOsg;

OsgModule* OsgModule::mysInstance = NULL;
bool OsgModule::mysAmbientOverrideHack = true;

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgModule::OsgModule():
	ServerModule("OsgModule"),
    myRepresentationSize(0.4)
{
    mysInstance = this;

    myRootNode = NULL;
    //myRootSceneObject = NULL;
    myFrameStamp = new osg::FrameStamp;
    myUpdateVisitor = new osgUtil::UpdateVisitor;
    myUpdateVisitor->setFrameStamp( myFrameStamp );

    osgDB::Registry::instance()->addReaderWriter(new ReaderFreeImage());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgModule::~OsgModule()
{
    if(myRootNode != NULL)
    {
        myRootNode->unref();
        myRootNode = NULL;
    }
    myFrameStamp->unref();
    myFrameStamp = NULL;
    myUpdateVisitor->unref();
    myUpdateVisitor = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgModule::initialize()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgModule::initializeRenderer(Renderer* r)
{
	OsgRenderPass* osgrp = new OsgRenderPass(r, "OsgRenderPass");
	osgrp->setUserData(this);
	r->addRenderPass(osgrp, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgModule::setRootNode(osg::Node* value) 
{ 
    myRootNode = value; 
    //myRootSceneObject = new OsgSceneObject(myRootNode);
    //myServer->getScene()->addObject(myRootSceneObject);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgModule::update(const UpdateContext& context)
{
    myFrameStamp->setFrameNumber(context.frameNum);
    myFrameStamp->setReferenceTime(context.time);
    myFrameStamp->setSimulationTime(context.time);

    myUpdateVisitor->reset();
    myUpdateVisitor->setFrameStamp(myFrameStamp);
    myUpdateVisitor->setTraversalNumber(context.frameNum);
    if(myRootNode != NULL)
    {
        myRootNode->accept(*myUpdateVisitor);
    }
}

