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
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Material>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaToolkit.h>
#include <omegaOsg.h>

// begin of #include from osgBullet example
#include <btBulletCollisionCommon.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/Utils.h>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/GUIEventHandler>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osgwTools/Shapes.h>
#include <osgwTools/Version.h>

#include <osg/io_utils>
#include <iostream>
// end of #include from osgBullet exaple

using namespace omega;
using namespace omegaToolkit;
using namespace omegaOsg;

String sModelName;
float sModelSize = 1.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OsgViewer: public EngineModule
{
public:
	OsgViewer()
	{
		myOsg = new OsgModule();
		ModuleServices::addModule(myOsg);
		myColWorld = initCollision();
	}

	virtual btCollisionWorld* initCollision();

	virtual void initialize();
	//virtual void update(const UpdateContext& context);
	//virtual void handleEvent(const Event& evt) {}

private:
	btCollisionWorld* myColWorld;
	Ref<OsgModule> myOsg;
	Ref<SceneNode> mySceneNode;
	//Actor* myInteractor;
	//osg::ref_ptr<osg::Light> myLight;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
btCollisionWorld* OsgViewer::initCollision()
{
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher( collisionConfiguration );

    btVector3 worldAabbMin( -10, -10, -10 );
    btVector3 worldAabbMax( 10, 10, 10 );
    btBroadphaseInterface* inter = new btAxisSweep3( worldAabbMin, worldAabbMax, 1000 );

    btCollisionWorld* collisionWorld = new btCollisionWorld( dispatcher, inter, collisionConfiguration );

    return( collisionWorld );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewer::initialize()
{
	// The node containing the scene
	//osg::ref_ptr<osg::Node> node = NULL;

	// The root node (we attach lights and other global state properties here)
	// Set the root to be a lightsource to attach a light to it to illuminate the scene
	//osg::Group* root = new osg::Group();
	osg::ref_ptr< osg::Group > root = new osg::Group();

	// Load osg object
	//if(SystemManager::settingExists("config/scene"))
	//{
	//	Setting& sscene = SystemManager::settingLookup("config/scene");
	//	sModelName = Config::getStringValue("filename", sscene, sModelName);
	//	sModelSize = Config::getFloatValue("size", sscene, sModelSize);
	//}

	// 1st node, static
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable( osgwTools::makeBox( osg::Vec3( 1, 1, 1) ) );
	root->addChild( geode.get() );

	if (geode == NULL) 
	{
		owarn("!Failed to load model!");
		return;
	}

	// make it a bullet object
	btCollisionObject* btBoxObject = new btCollisionObject();
	btBoxObject->setCollisionShape( osgbCollision::btBoxCollisionShapeFromOSG( geode ) ); // <-- here a Bullet object's shape is defined as a leaf node of OSG
    btBoxObject->setCollisionFlags( btCollisionObject::CF_STATIC_OBJECT );
    myColWorld->addCollisionObject( btBoxObject );

	// 2nd node
	// uncomment it after this app can compile successfully
	geode = new osg::Geode;
    geode->addDrawable( osgwTools::makeBox( osg::Vec3( 1, 1, 1 ) ) );
    osg::Matrix transMatrix = osg::Matrix::translate( 3., 0., 0. );
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform( transMatrix );
	mt->addChild( geode.get() );
    root->addChild( mt.get() );

	btBoxObject = new btCollisionObject();
    btBoxObject->setCollisionShape( osgbCollision::btBoxCollisionShapeFromOSG( geode ) ); // <-- here a Bullet object's shape is defined as a leaf node of OSG
    btBoxObject->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT );
    btBoxObject->setWorldTransform( osgbCollision::asBtTransform( transMatrix ) );
    myColWorld->addCollisionObject( btBoxObject );
	// following two lines are why this object is draggable
    //mm->setCollisionObject( btBoxObject );
    //mm->setMatrixTransform( mt );
	//*/ //end //

	//Optimize scenegraph
	//osgUtil::Optimizer optOSGFile;
	//optOSGFile.optimize(root);

	// Create an omegalib scene node and attach the osg node to it. This is used to interact with the 
	// osg object through omegalib interactors.
	OsgSceneObject* oso = new OsgSceneObject(root);
	mySceneNode = new SceneNode(getEngine());
	mySceneNode->addComponent(oso);
	mySceneNode->setBoundingBoxVisible(true);
	//mySceneNode->setBoundingBoxVisible(false);
	getEngine()->getScene()->addChild(mySceneNode);
	getEngine()->getDefaultCamera()->focusOn(getEngine()->getScene());

    // Set the interactor style used to manipulate meshes.
	//if(SystemManager::settingExists("config/interactor"))
	//{
	//	Setting& sinteractor = SystemManager::settingLookup("config/interactor");
	//	myInteractor = ToolkitUtils::createInteractor(sinteractor);
	//	if(myInteractor != NULL)
	//	{
	//		ModuleServices::addModule(myInteractor);
	//	}
	//}

	//if(myInteractor != NULL)
	//{
	//	myInteractor->setSceneNode(mySceneNode);
	//}

	// Set the osg node as the root node
	myOsg->setRootNode(oso->getTransformedNode());

	/*/ Setup shading
	myLight = new osg::Light;
    myLight->setLightNum(0);
    myLight->setPosition(osg::Vec4(0.0, 2, 1, 1.0));
    myLight->setAmbient(osg::Vec4(0.1f,0.1f,0.2f,1.0f));
    myLight->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	myLight->setSpotExponent(0);
	myLight->setSpecular(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
	//*/
	//osg::LightSource* ls = new osg::LightSource();
	//ls->setLight(myLight);
	//ls->setLocalStateSetModes(osg::StateAttribute::ON);
	//ls->setStateSetModes(*root->getOrCreateStateSet(), osg::StateAttribute::ON);

	//root->addChild(ls);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//void OsgViewer::update(const UpdateContext& context) 
//{
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<OsgViewer> app("osgBulletTest");
    return omain(app, argc, argv);
}