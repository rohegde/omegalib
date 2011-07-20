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
#include "SceneView.h"
#include "osgviewer.h"
#include "omega/FilesystemDataSource.h"
#include "omega/StringUtils.h"
#include "omega/DataManager.h"

#include <osg/Light>
#include <osg/LightSource>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgUtil/UpdateVisitor>

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> OsgViewerServer::readModel( const std::string& filename )
{
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo cfgInfo = dm->getInfo(filename);
	if(!cfgInfo.isNull())
	{
		osg::ref_ptr<osg::Node> root = osgDB::readNodeFile( cfgInfo.path );
		if ( !root.valid( )) 
		{
			oferror("Failed to load model: %1%", %filename);
			return root;
		}

		//Optimize scenegraph
		osgUtil::Optimizer optOSGFile;
		optOSGFile.optimize( root.get( ));
		return root;
	}
	else
	{
		oferror("File not found: %1%", %filename);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::initialize() 
{
    //Config* config = static_cast<Config*>( getConfig( ));
    //if( !config->mapData( initID ))
    //    return false;

    myFrameStamp = new osg::FrameStamp;
    myUpdateVisitor = new osgUtil::UpdateVisitor;
    myUpdateVisitor->setFrameStamp( myFrameStamp );
    //_contextID = 0;

    // load model at first config run
    if( !myModel )
    {
        const std::string& modelFile = "osg/cessna.osg"; 
        if( !modelFile.empty( ))
        {
            myModel = readModel( modelFile );

            if( myModel.valid( ))
            {
                osg::Matrix matrix;
                matrix.makeRotate( -osg::PI_2, osg::Vec3( 1., 0., 0. ));
				matrix.makeScale( 0.01, 0.01, 0.01 );

                osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
                transform->setMatrix( matrix );
                transform->addChild( myModel );
                transform->setDataVariance( osg::Object::STATIC );

				const osg::BoundingSphere& bs = transform->getBound();
				float x = bs.center()[0];
				float y = bs.center()[1];
				float z = bs.center()[2];
				float r = bs.radius();

				ofmsg("Model %1% bounding sphere center: (%2% %3% %4%) radius: %5%", %modelFile %x %y %z %r);

                myModel = transform;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::finalize()
{
    //_contextID = 0;
    myFrameStamp = 0;
    myUpdateVisitor = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerServer::update(const UpdateContext& context) 
{
	myFrameStamp->setFrameNumber(context.frameNum);

	const double time = static_cast< double >( context.time ) / 1000.;
    myFrameStamp->setReferenceTime(time);
    myFrameStamp->setSimulationTime(time);
	myUpdateVisitor->setTraversalNumber(context.frameNum);
    myModel->accept(*myUpdateVisitor);
    myModel->getBound();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool OsgViewerServer::handleEvent(const Event& evt, const UpdateContext& context) 
{ 
	return false; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::initialize()
{
	EngineClient::initialize();
        
    mySceneView =  new SceneView;
    mySceneView->setDefaults( SceneView::STANDARD_SETTINGS );
	mySceneView->setClearColor(osg::Vec4(0.1, 0.1, 0.1, 1.0));
    //_sceneView->setFrameStamp( node->getFrameStamp( ));
    mySceneView->init();
    //_sceneView->getState()->setContextID( node->getUniqueContextID( ));
    mySceneView->getRenderStage()->setColorMask(onew(osg::ColorMask)());

    //osg::ref_ptr< osg::Node > model = node->getModel();
    //_sceneView->setSceneData( model );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::finalize()
{
	mySceneView = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::draw( const DrawContext& context)
{
    EngineClient::draw( context );
    
	mySceneView->setViewport( context.viewport.x(), context.viewport.y(), context.viewport.width(), context.viewport.height() );

    // - Stereo
    //mySceneView->setDrawBufferValue( getDrawBuffer( ));
    //const eq::ColorMask& colorMask = getDrawBufferMask();

    //osgUtil::RenderStage* stage = mySceneView->getRenderStage();
    //osg::ref_ptr< osg::ColorMask > osgMask = stage->getColorMask();
    //osgMask->setMask( colorMask.red, colorMask.green, colorMask.blue, true );

    // - Frustum (Projection matrix)
    //const eq::Frustumf& frustum = getFrustum();
    //mySceneView->setProjectionMatrixAsFrustum( 
    //    frustum.left(), frustum.right(), frustum.bottom(), frustum.top(),
    //    frustum.near_plane(), frustum.far_plane( ));

	mySceneView->setProjectionMatrix(buildOsgMatrix(context.projection.matrix()));

    // - Camera (Model Matrix)
    //const Pipe *pipe = static_cast< const Pipe* >( getPipe( ));
    //const FrameData& frameData = pipe->getFrameData();
    
    //const eq::Vector3f position = frameData.getCameraPosition();
    //const eq::Vector3f lookAt = frameData.getCameraLookAtPoint();
    //const eq::Vector3f upVector = frameData.getCameraUpVector();

    //const osg::Vec3f pos( position.x(), position.y(), position.z( ));
    //const osg::Vec3f look( lookAt.x(), lookAt.y(), lookAt.z( ));
    //const osg::Vec3f up( upVector.x(), upVector.y(), upVector.z( ));
  
    //mySceneView->setViewMatrixAsLookAt( pos, look, up );

	mySceneView->setViewMatrix(buildOsgMatrix(context.modelview.matrix()));

    // - Frustum position (View Matrix)
    //osg::Matrix headView = mySceneView->getViewMatrix();
    //headView.postMult( buildOsgMatrix( context.modelview ));
    //mySceneView->setViewMatrix( headView );

    // - Render
    mySceneView->cull();
    mySceneView->draw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool OsgViewerClient::handleEvent(const Event& evt , UpdateContext &context )
{
    return EngineClient::handleEvent( evt , context );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OsgViewerClient::update(const UpdateContext& context)
{
	return EngineClient::update( context );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	OsgViewerApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "meshviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "osgviewer.log", new FilesystemDataSource("./../../data/"));

	return 0;
}
