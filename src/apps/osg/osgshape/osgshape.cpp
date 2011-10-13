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

osg::Geode* createShapes()
{
    osg::Geode* geode = new osg::Geode();

    
    // ---------------------------------------
    // Set up a StateSet to texture the objects
    // ---------------------------------------
    osg::StateSet* stateset = new osg::StateSet();

    //osg::Image* image = osgDB::readImageFile( "Images/lz.rgb" );
    //if (image)
    //{
    //    osg::Texture2D* texture = new osg::Texture2D;
    //    texture->setImage(image);
    //    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    //    stateset->setTextureAttributeAndModes(0,texture, osg::StateAttribute::ON);
    //}
    
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    
    geode->setStateSet( stateset );

    
    float radius = 0.8f;
    float height = 1.0f;
    
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),radius),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2*radius),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),radius,height),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.0f),radius,height),hints));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(8.0f,0.0f,0.0f),radius,height),hints));

    osg::HeightField* grid = new osg::HeightField;
    grid->allocate(38,39);
    grid->setXInterval(0.28f);
    grid->setYInterval(0.28f);
    
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
    (*vertices)[0].set(9.0+0.0f,-1.0f+2.0f,-1.0f+0.0f);
    (*vertices)[1].set(9.0+1.0f,-1.0f+0.0f,-1.0f+0.0f);
    (*vertices)[2].set(9.0+2.0f,-1.0f+2.0f,-1.0f+0.0f);
    (*vertices)[3].set(9.0+1.0f,-1.0f+1.0f,-1.0f+2.0f);
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

int main(int, char **)
{
    // construct the viewer.
    osgViewer::Viewer viewer;

    // add model to viewer.
    viewer.setSceneData( createShapes() );

    return viewer.run();
}
