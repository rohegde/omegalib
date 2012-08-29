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
#include "omegaOsg/OsgSceneObject.h"
#include "omega/SceneNode.h"

#include <osg/Node>
#include <osg/MatrixTransform>

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

	ofmsg("&OsgRenderable center: %1%, size: %2%", %center %bs.radius());

	myBBox.setExtents(center - radius, center + radius);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
OsgSceneObject::~OsgSceneObject()
{
	myTransform = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::update(SceneNode* node)
{
	node->addListener(this);
	const AffineTransform3& xform =  node->getFullTransform();
	const Matrix4f& m = xform.matrix();
	osg::Matrix oxform;
	oxform.set(m.data());
	myTransform->setMatrix( oxform );
 }

///////////////////////////////////////////////////////////////////////////////////////////////////
const AlignedBox3* OsgSceneObject::getBoundingBox()
{
	return &myBBox;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::onVisibleChanged(SceneNode* source, bool value) 
{
	myNode->setNodeMask(value ? ~0 : 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgSceneObject::onSelectedChanged(SceneNode* source, bool value) 
{}
