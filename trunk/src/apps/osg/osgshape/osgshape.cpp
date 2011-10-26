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
 *-------------------------------------------------------------------------------------------------
 * Original code from OpenSceneGraph example, osgshape.
 *************************************************************************************************/
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osgUtil/ShaderGen>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Math>

// for the grid data..
#include "terrain_coords.h"

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>
#include <oosg.h>

osg::Geode* createShapes()
{
    osg::Geode* geode = new osg::Geode();

    osg::StateSet* stateset = new osg::StateSet();
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    geode->setStateSet( stateset );

    float radius = 0.08f;
    float height = 0.1f;
    
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),radius),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.2f,0.0f,0.0f),2*radius),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.4f,0.0f,0.0f),radius,height),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.6f,0.0f,0.0f),radius,height),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(0.8f,0.0f,0.0f),radius,height),hints));

    osg::HeightField* grid = new osg::HeightField;
    grid->allocate(38,39);
    grid->setXInterval(0.028f);
    grid->setYInterval(0.028f);
    
    for(unsigned int r=0;r<39;++r)
    {
        for(unsigned int c=0;c<38;++c)
        {
            grid->setHeight(c,r,vertex[r+c*39][2]);
        }
    }
    geode->addDrawable(new osg::ShapeDrawable(grid));
    
    osg::ConvexHull* mesh = new osg::ConvexHull;
    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0].set(0.9+0.0f,-0.1f+0.2f,-0.1f+0.0f);
    (*vertices)[1].set(0.9+0.1f,-0.1f+0.0f,-0.1f+0.0f);
    (*vertices)[2].set(0.9+0.2f,-0.1f+0.2f,-0.1f+0.0f);
    (*vertices)[3].set(0.9+0.1f,-0.1f+0.2f,-0.1f+0.2f);
    osg::UByteArray* indices = new osg::UByteArray(12);
    (*indices)[0]=0;
    (*indices)[1]=2;
    (*indices)[2]=1;
    (*indices)[3]=0;
    (*indices)[4]=1;
    (*indices)[5]=3;
    (*indices)[6]=1;
    (*indices)[7]=2;
    (*indices)[8]=3;
    (*indices)[9]=2;
    (*indices)[10]=0;
    (*indices)[11]=3;
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    geode->addDrawable(new osg::ShapeDrawable(mesh));

    return geode;
}

using namespace omega;
using namespace oengine;
using namespace oosg;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OsgShape: public EngineServer
{
public:
	OsgShape(Application* app): EngineServer(app) {}
	virtual void initialize();
	virtual void update(const UpdateContext& context);

private:
	OsgModule* myOsg;
	SceneNode* mySceneNode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OsgShape::initialize()
{
	EngineServer::initialize();

	myOsg = new OsgModule();
	myOsg->initialize(this);

	myOsg->setRootNode(createShapes());

	//getDefaultCamera()->focusOn(getScene(0), 0.2f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OsgShape::update(const UpdateContext& context)
{
	EngineServer::update(context);
	myOsg->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<OsgShape> app;
	const char* cfgName = "system/desktop.cfg";
	if(argc == 2) cfgName = argv[1];
	omain(app, cfgName, "osgshape.log", new FilesystemDataSource(OMEGA_DATA_PATH));
	return 0;
}
