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
#ifndef __OSGVIEWER_H__
#define __OSGVIEWER_H__

#include "omega.h"
#include "omega/scene.h"

#include "oosg/OsgEntity.h"
#include "oosg/OsgRenderPass.h"

using namespace omega;
using namespace omega::scene;
using namespace omega::ui;
using namespace oosg;

///////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewerServer: public ApplicationServer
{
public:
	OsgViewerServer(Application* app): ApplicationServer(app) {}
	virtual ~OsgViewerServer() {}

	virtual void initialize();
	virtual void finalize();
	virtual void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);

	OsgEntity* getEntity() { return myEntity; }

private:
	OsgEntity* myEntity;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewerClient: public EngineClient
{
public:
	OsgViewerClient(ApplicationServer* server): 
	  EngineClient(server),
	  myServer((OsgViewerServer*)server) {}

	virtual void initialize();
	virtual void finalize();

	virtual void handleEvent(const Event& evt);
    void draw( const DrawContext& context);
	void update(const UpdateContext& context);

private:
	OsgRenderPass* myRenderPass;
	OsgViewerServer* myServer;
	SceneNode* myEntityNode;
	Actor* myCurrentInteractor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient(ApplicationServer* server) { return new OsgViewerClient(server); }
	virtual ApplicationServer* createServer() { return new OsgViewerServer(this); }
};

#endif