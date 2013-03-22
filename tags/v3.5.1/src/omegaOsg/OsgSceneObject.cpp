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
#include "omegaOsg/OsgSceneObject.h"
#include "omega/SceneNode.h"

#include <osg/Node> 
#include <osg/MatrixTransform>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>

using namespace omegaOsg;

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgSceneObject::OsgSceneObject(osg::Node* node): myNode(node), myInitialized(false)
{
    myTransform = new osg::MatrixTransform();
    myTransform->addChild( node );
	myTransform->setDataVariance( osg::Object::DYNAMIC );

	const osg::BoundingSphere& bs = node->getBound();
	Vector3f center(bs.center()[0], bs.center()[1], bs.center()[2]);

	float fradius = bs.radius();
	if(fradius == -1) fradius = 0;

	Vector3f radius(fradius, fradius, fradius);

	//ofmsg("&OsgRenderable center: %1%, size: %2%", %center %bs.radius());

	myBBox.setExtents(center - radius, center + radius);
	requestBoundingBoxUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgSceneObject::~OsgSceneObject()
{
	myTransform = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::onAttached(SceneNode* node)
{
	node->addListener(this);
	// Force and update of the visible and selected states for this object.
	onVisibleChanged(node, node->isVisible());
	onSelectedChanged(node, node->isSelected());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::onDetached(SceneNode* node)
{
	node->removeListener(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::update(const UpdateContext& context)
{
	SceneNode* node = getOwner();
	const AffineTransform3& xform =  node->getFullTransform();
	const Matrix4f& m = xform.matrix();
	osg::Matrix oxform;
	oxform.set(m.data());
	myTransform->setMatrix( oxform );
	requestBoundingBoxUpdate();

	//const osg::BoundingSphere& bs = myNode->getBound();
	//Vector3f center(bs.center()[0], bs.center()[1], bs.center()[2]);

	//float fradius = bs.radius();
	//if(fradius == -1) fradius = 0;

	//Vector3f radius(fradius, fradius, fradius);

	//ofmsg("&OsgRenderable center: %1%, size: %2%", %center %bs.radius());

	//myBBox.setExtents(center - radius, center + radius);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* OsgSceneObject::getBoundingBox()
{
	return &myBBox;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::onVisibleChanged(SceneNode* source, bool value) 
{
	myTransform->setNodeMask(value ? ~0 : 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::onSelectedChanged(SceneNode* source, bool value) 
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool OsgSceneObject::intersectRay(const Ray& ray, Vector3f* hitPoint)
{
	Vector3f rstart = ray.getOrigin();
	Vector3f rend = ray.getPoint(1000);

	osg::Vec3d orig(rstart[0], rstart[1], rstart[2]);
	osg::Vec3d end(rend[0], rend[1], rend[2]);


	Ref<osgUtil::LineSegmentIntersector> lsi = new osgUtil::LineSegmentIntersector(orig, end);
	osgUtil::IntersectionVisitor iv(lsi);

	myTransform->accept(iv);

	if(!lsi->containsIntersections()) return false;

	osgUtil::LineSegmentIntersector::Intersection i = lsi->getFirstIntersection();
	
	osg::Vec3d intersect = i.getWorldIntersectPoint();

	hitPoint->x() = intersect[0];
	hitPoint->y() = intersect[1];
	hitPoint->z() = intersect[2];
	return true;
}
