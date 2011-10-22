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
#include "omega/RenderTarget.h"
#include "oosg/SceneView.h"
#include "oosg/OsgRenderPass.h"

using namespace oosg;

OMEGA_DEFINE_TYPE(OsgRenderPass, RenderPass);

Lock sInitLock;
Lock sDrawLock;

///////////////////////////////////////////////////////////////////////////////////////////////
inline osg::Matrix buildOsgMatrix( const omega::Matrix4f& matrix )
{
    return osg::Matrix( matrix( 0, 0 ), matrix( 1, 0 ),
                        matrix( 2, 0 ), matrix( 3, 0 ),
                        matrix( 0, 1 ), matrix( 1, 1 ),
                        matrix( 2, 1 ), matrix( 3, 1 ),
                        matrix( 0, 2 ), matrix( 1, 2 ),
                        matrix( 2, 2 ), matrix( 3, 2 ),
                        matrix( 0, 3 ), matrix( 1, 3 ),
                        matrix( 2, 3 ), matrix( 3, 3 ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgRenderPass::~OsgRenderPass()
{
	mySceneView->unref();
	mySceneView = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgRenderPass::initialize() 
{
	sInitLock.lock();

	RenderPass::initialize();

	myModule = (OsgModule*)getUserData();
	mySceneView = new SceneView();
    mySceneView->setDefaults( SceneView::STANDARD_SETTINGS );
	mySceneView->setClearColor(osg::Vec4(0.1, 0.1, 0.1, 0.0));
    mySceneView->init();
	mySceneView->getState()->setContextID(getClient()->getId());
    mySceneView->getRenderStage()->setColorMask(onew(osg::ColorMask)(255, 255, 255, true));

	sInitLock.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgRenderPass::render(EngineClient* client, const DrawContext& context)
{
	if(context.task == DrawContext::SceneDrawTask)
	{
		sDrawLock.lock();
		mySceneView->setViewport( context.viewport.x(), context.viewport.y(), context.viewport.width(), context.viewport.height() );
		mySceneView->setProjectionMatrix(buildOsgMatrix(context.projection.matrix()));
		mySceneView->setViewMatrix(buildOsgMatrix(context.modelview.matrix()));
		mySceneView->setDrawBufferValue(context.drawBuffer->getGLId());

		osg::Node* root = myModule->getRootNode();
		mySceneView->setSceneData(root);
		mySceneView->setFrameStamp(myModule->getFrameStamp());

		mySceneView->cull();
		mySceneView->draw();
		sDrawLock.unlock();
	}
}
