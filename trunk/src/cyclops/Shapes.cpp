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
#include "cyclops/Shapes.h"

#include <osgUtil/TangentSpaceGenerator>
#include <osgwTools/Shapes.h>

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
SphereShape::SphereShape(SceneManager* scene, float radius, int subdivisions, Vector2f tiling):
	DrawableObject(scene), 
		myRadius(radius),
		mySubdivisions(subdivisions),
		myTiling(tiling)
{
	osg::Geode* node = new osg::Geode();

	osg::Geometry* sphere = osgwTools::makeGeodesicSphere(radius, subdivisions);
	sphere->setColorArray(NULL);
	sphere->setColorBinding(osg::Geometry::BIND_OFF);

	osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
	tsg->generate(sphere, 0);
	osg::Vec4Array* a_tangent = tsg->getTangentArray();
	sphere->setVertexAttribArray (6, a_tangent);
	sphere->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet* fx = node->getOrCreateStateSet();
	fx->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(tiling[0], tiling[1])));

	node->addDrawable(sphere);
	sphere->setStateSet(fx);

	tsg->unref();

	initialize(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PlaneShape::PlaneShape(SceneManager* scene, float width, float height, Vector2f tiling):
	DrawableObject(scene), 
		myHeight(height),
		myWidth(width),
		myTiling(tiling)
{
	// The plane is always created on the XY plane.
	Vector3f startCorner(- width/2, -height/2, 0);
	Vector3f u(width, 0, 0);
	Vector3f v(0, height, 0);

	osg::Geometry* plane = osgwTools::makePlane(
		OOSG_VEC3(startCorner), 
		OOSG_VEC3(u), 
		OOSG_VEC3(v));
	plane->setColorArray(NULL);
	plane->setColorBinding(osg::Geometry::BIND_OFF);

	osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
	tsg->generate(plane, 0);
	osg::Vec4Array* a_tangent = tsg->getTangentArray();
	plane->setVertexAttribArray (6, a_tangent);
	plane->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);

	osg::Geode* node = new osg::Geode();
	osg::StateSet* fx = node->getOrCreateStateSet();
	fx->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(tiling[0], tiling[1])));
	plane->setStateSet(fx);
	node->addDrawable(plane);
	tsg->unref();

	initialize(node);
}
