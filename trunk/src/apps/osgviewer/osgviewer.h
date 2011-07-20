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
#ifndef __MESHVIEWER_H__
#define __MESHVIEWER_H__

#include "omega/Application.h"
#include "omega/EngineClient.h"

using namespace omega;
using namespace omega::scene;
using namespace omega::ui;

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

///////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewerServer: public ApplicationServer
{
public:
	OsgViewerServer(Application* app): ApplicationServer(app) {}
	virtual ~OsgViewerServer() {}

	virtual void initialize();
	virtual void finalize();
	virtual void update(const UpdateContext& context);
	virtual bool handleEvent(const Event& evt, const UpdateContext& context);

private:
    osg::ref_ptr< osg::Node > createSceneGraph();
    osg::ref_ptr< osg::Node > createSceneGraph( osg::ref_ptr<osg::Image> );
    osg::ref_ptr< osg::Group > initSceneGraph();
    osg::ref_ptr< osg::LightSource > createLightSource();
	osg::ref_ptr< osg::Node > readModel( const std::string& filename );

private:
    //co::base::a_int32_t myContextID;
    osg::ref_ptr< osg::Node > myModel;
    osg::ref_ptr< osg::FrameStamp > myFrameStamp;
    osg::ref_ptr< osg::NodeVisitor > myUpdateVisitor;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewerClient: public EngineClient
{
public:
	OsgViewerClient(Application* app): 
	  EngineClient(app) {}

	virtual void initialize();
	virtual void finalize();

	virtual bool handleEvent(const Event& evt , UpdateContext &context );
    void draw( const DrawContext& context);
	void update(const UpdateContext& context);

private:
	osg::ref_ptr< SceneView > mySceneView;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new OsgViewerClient(this); }
	virtual ApplicationServer* createServer() { return new OsgViewerServer(this); }
};

#endif
