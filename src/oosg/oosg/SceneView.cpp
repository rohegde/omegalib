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
 * Original code adapted from Equalizer/osg
 * -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *                           2010 Stefan Eilemann <eile@eyescale.ch>
 *************************************************************************************************/
#include "oosg/SceneView.h"

#include <osgUtil/UpdateVisitor>
#include <osgUtil/GLObjectsVisitor>

#include <osg/Timer>
#include <osg/GLExtensions>
#include <osg/GLObjects>
#include <osg/Notify>
#include <osg/Texture>
#include <osg/AlphaFunc>
#include <osg/TexEnv>
#include <osg/ColorMatrix>
#include <osg/LightModel>
#include <osg/CollectOccludersVisitor>

#include <osg/GLU>

#include <iterator>

#include <osgwQuery/QueryUtils.h>
#include <osgwQuery/QueryBenchmarks.h>
#include <osgwQuery/QueryStats.h>

// Define this to enable occlusion cullling.
// #define USE_OCCLUSION_CULLING

using namespace osg;
using namespace osgUtil;

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneView::SceneView( osg::DisplaySettings* ds)
{
    _displaySettings = ds;

    _lightingMode=NO_SCENEVIEW_LIGHT;
    
    _prioritizeTextures = false;
    
    setCamera(new Camera);
    _camera->setViewport(new Viewport);
	_camera->setClearMask(0);
    
    _initCalled = false;

    _camera->setDrawBuffer(GL_BACK);

    _requiresFlush = true;
    
    _activeUniforms = DEFAULT_UNIFORMS;
    
    _previousFrameTime = 0;
    _previousSimulationTime = 0;
    
    _dynamicObjectCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneView::SceneView(const SceneView& rhs, const osg::CopyOp& copyop):
    osg::Object(rhs,copyop),
    osg::CullSettings(rhs)
{
    _displaySettings = rhs._displaySettings;

    _lightingMode = rhs._lightingMode;
    
    _prioritizeTextures = rhs._prioritizeTextures;
    
    _camera = rhs._camera;
    _cameraWithOwnership = rhs._cameraWithOwnership;
    
    _initCalled = false;

    _requiresFlush = rhs._requiresFlush;
    
    _activeUniforms = rhs._activeUniforms;
    
    _previousFrameTime = 0;
    _previousSimulationTime = 0;
    
    _dynamicObjectCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneView::~SceneView()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::setDefaults(unsigned int options)
{
    osg::CullSettings::setDefaults();

    //_camera->getProjectionMatrix().makePerspective(50.0f,1.4f,1.0f,10000.0f);
    //_camera->getViewMatrix().makeIdentity();

    if (!_globalStateSet) _globalStateSet = new osg::StateSet;
    else _globalStateSet->clear();

    if ((options & HEADLIGHT) || (options & SKY_LIGHT))
    {
        #if defined(OSG_GL_FIXED_FUNCTION_AVAILABLE)
            _lightingMode=(options&HEADLIGHT) ? HEADLIGHT : SKY_LIGHT;
            _light = new osg::Light;
            _light->setLightNum(0);
            _light->setAmbient(Vec4(0.00f,0.0f,0.00f,1.0f));
            _light->setDiffuse(Vec4(0.8f,0.8f,0.8f,1.0f));
            _light->setSpecular(Vec4(1.0f,1.0f,1.0f,1.0f));


            _globalStateSet->setAssociatedModes(_light.get(),osg::StateAttribute::ON);

            // enable lighting by default.
            _globalStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
        #endif
        
        #if !defined(OSG_GLES1_AVAILABLE) && !defined(OSG_GLES2_AVAILABLE) && defined(OSG_GL_FIXED_FUNCTION_AVAILABLE)
            osg::LightModel* lightmodel = new osg::LightModel;
            lightmodel->setAmbientIntensity(osg::Vec4(0.1f,0.1f,0.1f,1.0f));
            _globalStateSet->setAttributeAndModes(lightmodel, osg::StateAttribute::ON);
        #endif
    }
    else
    {
        _lightingMode = NO_SCENEVIEW_LIGHT;
    }
 
    _renderInfo.setState(new State);
    
    _stateGraph = new StateGraph;
    _renderStage = new RenderStage;

    _updateVisitor = new UpdateVisitor;

    _cullVisitor = CullVisitor::create();

    _cullVisitor->setStateGraph(_stateGraph.get());
    _cullVisitor->setRenderStage(_renderStage.get());

    _globalStateSet->setGlobalDefaults();

    // Do not clear the frame buffer - the omegalib engine takes care of this.
	_camera->setClearMask(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::setCamera(osg::Camera* camera, bool assumeOwnershipOfCamera)
{
    if (camera)
    {
        _camera = camera;
    }
    else
    {
        osg::notify(osg::NOTICE)<<"Warning: attempt to assign a NULL camera to SceneView not permitted."<<std::endl;
    }
    
    if (assumeOwnershipOfCamera)
    {
        _cameraWithOwnership = _camera.get();
    }
    else
    {
        _cameraWithOwnership = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::setSceneData(osg::Node* node)
{
    // take a temporary reference to node to prevent the possibility
    // of it getting deleted when when we do the camera clear of children. 
    osg::ref_ptr<osg::Node> temporaryRefernce = node;
    
    // remove pre existing children
    _camera->removeChildren(0, _camera->getNumChildren());
    
    // add the new one in.
    _camera->addChild(node);

#ifdef USE_OCCLUSION_CULLING
    node->getOrCreateStateSet()->setRenderBinDetails( 0, _QUERY_FRONT_TO_BACK_BIN_NAME );
    osgwQuery::AddQueries aqs;
    aqs.setQueryStats( NULL );
    _camera->accept( aqs );
	_camera->setPreDrawCallback(new osgwQuery::InitCallback() );
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::init()
{
    _initCalled = true;

    if (_camera.valid() && _initVisitor.valid())
    {
        _initVisitor->reset();
        _initVisitor->setFrameStamp(_frameStamp.get());
        
        GLObjectsVisitor* dlv = dynamic_cast<GLObjectsVisitor*>(_initVisitor.get());
        if (dlv) dlv->setState(_renderInfo.getState());
        
        if (_frameStamp.valid())
        {
             _initVisitor->setTraversalNumber(_frameStamp->getFrameNumber());
        }
        
        _camera->accept(*_initVisitor.get());
        
    } 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::updateUniforms()
{
    if (!_localStateSet)
    {
        _localStateSet = new osg::StateSet;
    }

    if (!_localStateSet) return;
    
    if ((_activeUniforms & FRAME_NUMBER_UNIFORM) && _frameStamp.valid())
    {
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_FrameNumber",osg::Uniform::INT);
        uniform->set(static_cast<int>(_frameStamp->getFrameNumber()));        
    }
    
    if ((_activeUniforms & FRAME_TIME_UNIFORM) && _frameStamp.valid())
    {
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_FrameTime",osg::Uniform::FLOAT);
        uniform->set(static_cast<float>(_frameStamp->getReferenceTime()));
    }
    
    if ((_activeUniforms & DELTA_FRAME_TIME_UNIFORM) && _frameStamp.valid())
    {
        float delta_frame_time = (_previousFrameTime != 0.0) ? static_cast<float>(_frameStamp->getReferenceTime()-_previousFrameTime) : 0.0f;
        _previousFrameTime = _frameStamp->getReferenceTime();
        
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_DeltaFrameTime",osg::Uniform::FLOAT);
        uniform->set(delta_frame_time);
    }
    
    if ((_activeUniforms & SIMULATION_TIME_UNIFORM) && _frameStamp.valid())
    {
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_SimulationTime",osg::Uniform::FLOAT);
        uniform->set(static_cast<float>(_frameStamp->getSimulationTime()));
    }
    
    if ((_activeUniforms & DELTA_SIMULATION_TIME_UNIFORM) && _frameStamp.valid())
    {
        float delta_simulation_time = (_previousSimulationTime != 0.0) ? static_cast<float>(_frameStamp->getSimulationTime()-_previousSimulationTime) : 0.0f;
        _previousSimulationTime = _frameStamp->getSimulationTime();
        
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_DeltaSimulationTime",osg::Uniform::FLOAT);
        uniform->set(delta_simulation_time);
    }
    
    if (_activeUniforms & VIEW_MATRIX_UNIFORM)
    {
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_ViewMatrix",osg::Uniform::FLOAT_MAT4);
        uniform->set(getViewMatrix());
    }

    if (_activeUniforms & VIEW_MATRIX_INVERSE_UNIFORM)
    {
        osg::Uniform* uniform = _localStateSet->getOrCreateUniform("osg_ViewMatrixInverse",osg::Uniform::FLOAT_MAT4);
        uniform->set(osg::Matrix::inverse(getViewMatrix()));
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::setLightingMode(LightingMode mode)
{
    if (mode==_lightingMode) return;
    
    if (_lightingMode!=NO_SCENEVIEW_LIGHT)
    {
        // remove GL_LIGHTING mode
        _globalStateSet->removeMode(GL_LIGHTING);

        if (_light.valid())
        {
            _globalStateSet->removeAssociatedModes(_light.get());
        }

    }

    _lightingMode = mode;

    //if (_lightingMode!=NO_SCENEVIEW_LIGHT)
    //{
    //    #if defined(OSG_GL_FIXED_FUNCTION_AVAILABLE)
    //        // add GL_LIGHTING mode
    //        _globalStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    //        if (_light.valid()) 
    //        {
    //            _globalStateSet->setAssociatedModes(_light.get(), osg::StateAttribute::ON);
    //        }
    //    #endif
    //}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::inheritCullSettings(const osg::CullSettings& settings, unsigned int inheritanceMask)
{
    if (_camera.valid() && _camera->getView()) 
    {
        if (inheritanceMask & osg::CullSettings::LIGHTING_MODE)
        {
            LightingMode newLightingMode = _lightingMode;
        
            switch(_camera->getView()->getLightingMode())
            {
                case(osg::View::NO_LIGHT): newLightingMode = NO_SCENEVIEW_LIGHT; break;
                case(osg::View::HEADLIGHT): newLightingMode = HEADLIGHT; break;
                case(osg::View::SKY_LIGHT): newLightingMode = SKY_LIGHT; break;
            }
            
            if (newLightingMode != _lightingMode)
            {
                setLightingMode(newLightingMode);
            }
        }
                
        if (inheritanceMask & osg::CullSettings::LIGHT)
        {
            setLight(_camera->getView()->getLight());
        }
    }
    
    osg::CullSettings::inheritCullSettings(settings, inheritanceMask);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::cull()
{
    _dynamicObjectCount = 0;

    if (_camera->getNodeMask()==0) return;

    _renderInfo.setView(_camera->getView());
	_renderInfo.pushCamera(_camera.get());

    // update the active uniforms
    updateUniforms();

    if (!_renderInfo.getState())
    {
        osg::notify(osg::INFO) << "Warning: no valid osgUtil::SceneView::_state attached, creating a default state automatically."<< std::endl;

        // note the constructor for osg::State will set ContextID to 0 which will be fine to single context graphics
        // applications which is ok for most apps, but not multiple context/pipe applications.
        _renderInfo.setState(new osg::State);
    }
    
    osg::State* state = _renderInfo.getState();

    if (!_localStateSet)
    {
        _localStateSet = new osg::StateSet;
    }
    
    // we in theory should be able to be able to bypass reset, but we'll call it just incase.
    //_state->reset();
   
    state->setFrameStamp(_frameStamp.get());
    state->setDisplaySettings(_displaySettings.get());

#ifdef USE_OCCLUSION_CULLING
	if (_camera->getPreDrawCallback())
    {
        (*(_camera->getPreDrawCallback()))(*_camera);
    }
#endif

    if (!_cullVisitor)
    {
        osg::notify(osg::INFO) << "Warning: no valid osgUtil::SceneView:: attached, creating a default CullVisitor automatically."<< std::endl;
        _cullVisitor = CullVisitor::create();
    }
    if (!_stateGraph)
    {
        osg::notify(osg::INFO) << "Warning: no valid osgUtil::SceneView:: attached, creating a global default StateGraph automatically."<< std::endl;
        _stateGraph = new StateGraph;
    }
    if (!_renderStage)
    {
        osg::notify(osg::INFO) << "Warning: no valid osgUtil::SceneView::_renderStage attached, creating a default RenderStage automatically."<< std::endl;
        _renderStage = new RenderStage;
    }

    _cullVisitor->setTraversalMask(_cullMask);
    bool computeNearFar = cullStage(getProjectionMatrix(),getViewMatrix(),_cullVisitor.get(),_stateGraph.get(),_renderStage.get(),getViewport());

    if (computeNearFar)
    {
        CullVisitor::value_type zNear = _cullVisitor->getCalculatedNearPlane();
        CullVisitor::value_type zFar = _cullVisitor->getCalculatedFarPlane();
        _cullVisitor->clampProjectionMatrix(getProjectionMatrix(),zNear,zFar);
    }
	_renderInfo.popCamera();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneView::cullStage(const osg::Matrixd& projection,const osg::Matrixd& modelview,osgUtil::CullVisitor* cullVisitor, osgUtil::StateGraph* rendergraph, osgUtil::RenderStage* renderStage, osg::Viewport *viewport)
{

    if (!_camera || !viewport) return false;

    osg::ref_ptr<RefMatrix> proj = new osg::RefMatrix(projection);
    osg::ref_ptr<RefMatrix> mv = new osg::RefMatrix(modelview);

    // collect any occluder in the view frustum.
    if (_camera->containsOccluderNodes())
    {
        //std::cout << "Scene graph contains occluder nodes, searching for them"<<std::endl;
        
        
        if (!_collectOccludersVisitor) _collectOccludersVisitor = new osg::CollectOccludersVisitor;
        
        _collectOccludersVisitor->inheritCullSettings(*this);
        
        _collectOccludersVisitor->reset();
        
        _collectOccludersVisitor->setFrameStamp(_frameStamp.get());

        // use the frame number for the traversal number.
        if (_frameStamp.valid())
        {
             _collectOccludersVisitor->setTraversalNumber(_frameStamp->getFrameNumber());
        }

        _collectOccludersVisitor->pushViewport(viewport);
        _collectOccludersVisitor->pushProjectionMatrix(proj.get());
        _collectOccludersVisitor->pushModelViewMatrix(mv.get(),osg::Transform::ABSOLUTE_RF);

        // traverse the scene graph to search for occluder in there new positions.
        _collectOccludersVisitor->traverse(*_camera);

        _collectOccludersVisitor->popModelViewMatrix();
        _collectOccludersVisitor->popProjectionMatrix();
        _collectOccludersVisitor->popViewport();
        
        // sort the occluder from largest occluder volume to smallest.
        _collectOccludersVisitor->removeOccludedOccluders();
        
        
        osg::notify(osg::DEBUG_INFO) << "finished searching for occluder - found "<<_collectOccludersVisitor->getCollectedOccluderSet().size()<<std::endl;
           
        cullVisitor->getOccluderList().clear();
        std::copy(_collectOccludersVisitor->getCollectedOccluderSet().begin(),_collectOccludersVisitor->getCollectedOccluderSet().end(), std::back_insert_iterator<CullStack::OccluderList>(cullVisitor->getOccluderList()));
    }
    
    cullVisitor->reset();

    cullVisitor->setFrameStamp(_frameStamp.get());

    // use the frame number for the traversal number.
    if (_frameStamp.valid())
    {
         cullVisitor->setTraversalNumber(_frameStamp->getFrameNumber());
    }

    cullVisitor->inheritCullSettings(*this);

    cullVisitor->setStateGraph(rendergraph);
    cullVisitor->setRenderStage(renderStage);

    cullVisitor->setRenderInfo( _renderInfo );

    renderStage->reset();

    // comment out reset of rendergraph since clean is more efficient.
    //  rendergraph->reset();

    // use clean of the rendergraph rather than reset, as it is able to
    // reuse the structure on the rendergraph in the next frame. This
    // achieves a certain amount of frame cohereancy of memory allocation.
    rendergraph->clean();

    renderStage->setViewport(viewport);
    renderStage->setClearColor(_camera->getClearColor());
    renderStage->setClearDepth(_camera->getClearDepth());
    renderStage->setClearAccum(_camera->getClearAccum());
    renderStage->setClearStencil(_camera->getClearStencil());
    renderStage->setClearMask(_camera->getClearMask());
    
    renderStage->setCamera(_camera.get());

    if (_globalStateSet.valid()) cullVisitor->pushStateSet(_globalStateSet.get());
    if (_secondaryStateSet.valid()) cullVisitor->pushStateSet(_secondaryStateSet.get());
    if (_localStateSet.valid()) cullVisitor->pushStateSet(_localStateSet.get());

    cullVisitor->pushViewport(viewport);
    cullVisitor->pushProjectionMatrix(proj.get());
    cullVisitor->pushModelViewMatrix(mv.get(),osg::Transform::ABSOLUTE_RF);

    // traverse the scene graph to generate the rendergraph.    
    // If the camera has a cullCallback execute the callback which has the  
    // requirement that it must traverse the camera's children.
    {
       osg::NodeCallback* callback = _camera->getCullCallback();
       if (callback) (*callback)(_camera.get(), cullVisitor);
       else cullVisitor->traverse(*_camera);
    }


    cullVisitor->popModelViewMatrix();
    cullVisitor->popProjectionMatrix();
    cullVisitor->popViewport();

    if (_localStateSet.valid()) cullVisitor->popStateSet();
    if (_secondaryStateSet.valid()) cullVisitor->popStateSet();
    if (_globalStateSet.valid()) cullVisitor->popStateSet();
    

    renderStage->sort();

    // prune out any empty StateGraph children.
    // note, this would be not required if the rendergraph had been
    // reset at the start of each frame (see top of this method) but
    // a clean has been used instead to try to minimize the amount of
    // allocation and deleteing of the StateGraph nodes.
    rendergraph->prune();
    
    // set the number of dynamic objects in the scene.    
    _dynamicObjectCount += renderStage->computeNumberOfDynamicRenderLeaves();


    bool computeNearFar = (cullVisitor->getComputeNearFarMode()!=osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR) && getSceneData()!=0;
    return computeNearFar;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::releaseAllGLObjects()
{
    if (!_camera) return;
   
    _camera->releaseGLObjects(_renderInfo.getState());
    
    // we need to reset State as it keeps handles to Program objects.
    if (_renderInfo.getState()) _renderInfo.getState()->reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::flushAllDeletedGLObjects()
{
    _requiresFlush = false;
    
    osg::flushAllDeletedGLObjects(getState()->getContextID());
 }

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::flushDeletedGLObjects(double& availableTime)
{
    osg::State* state = _renderInfo.getState();

    _requiresFlush = false;

    double currentTime = state->getFrameStamp()?state->getFrameStamp()->getReferenceTime():0.0;

    osg::flushDeletedGLObjects(getState()->getContextID(), currentTime, availableTime);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::draw()
{
    if (_camera->getNodeMask()==0) return;

    osg::State* state = _renderInfo.getState();
    state->initializeExtensionProcs();

    osg::Texture::TextureObjectManager* tom = osg::Texture::getTextureObjectManager(state->getContextID()).get();
    tom->newFrame(state->getFrameStamp());

    osg::GLBufferObjectManager* bom = osg::GLBufferObjectManager::getGLBufferObjectManager(state->getContextID()).get();
    bom->newFrame(state->getFrameStamp());

    if (!_initCalled) init();

    // note, to support multi-pipe systems the deletion of OpenGL display list
    // and texture objects is deferred until the OpenGL context is the correct
    // context for when the object were originally created.  Here we know what
    // context we are in so can flush the appropriate caches.
    
    if (_requiresFlush)
    {
        double availableTime = 0.005;
        flushDeletedGLObjects(availableTime);
    }

    // assume the the draw which is about to happen could generate GL objects that need flushing in the next frame.
    _requiresFlush = true;

    state->setInitialViewMatrix(new osg::RefMatrix(getViewMatrix()));

    RenderLeaf* previous = NULL;

    _localStateSet->setAttribute(getViewport());

    // bog standard draw.
    _renderStage->drawPreRenderStages(_renderInfo,previous);
    _renderStage->draw(_renderInfo,previous);
    
    // re apply the defalt OGL state.
    state->popAllStateSets();
    state->apply();

    if (state->getCheckForGLErrors()!=osg::State::NEVER_CHECK_GL_ERRORS)
    {
        if (state->checkGLErrors("end of SceneView::draw()"))
        {
            // go into debug mode of OGL error in a fine grained way to help
            // track down OpenGL errors.
            state->setCheckForGLErrors(osg::State::ONCE_PER_ATTRIBUTE);
        }
    }

// #define REPORT_TEXTURE_MANAGER_STATS
#ifdef REPORT_TEXTURE_MANAGER_STATS
    tom->reportStats();
    bom->reportStats();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//void SceneView::clearArea(int x,int y,int width,int height,const osg::Vec4& color)
//{
//    osg::ref_ptr<osg::Viewport> viewport = new osg::Viewport;
//    viewport->setViewport(x,y,width,height);
//
//    _renderInfo.getState()->applyAttribute(viewport.get());
//    
//    glScissor( x, y, width, height );
//    glEnable( GL_SCISSOR_TEST );
//    glClearColor( color[0], color[1], color[2], color[3]);
//    glClear( GL_COLOR_BUFFER_BIT);
//    glDisable( GL_SCISSOR_TEST );
//}
//

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneView::getStats(osgUtil::Statistics& stats)
{
    return _renderStage->getStats(stats);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::collateReferencesToDependentCameras()
{
    if (_renderStage.valid()) _renderStage->collateReferencesToDependentCameras();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneView::clearReferencesToDependentCameras()
{
    if (_renderStage.valid()) _renderStage->clearReferencesToDependentCameras();
}
