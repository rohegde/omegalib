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
 *-------------------------------------------------------------------------------------------------
 * Original code adapted from Equalizer/osg
 * -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *                           2010 Stefan Eilemann <eile@eyescale.ch>
 *************************************************************************************************/
#ifndef OOSG_SCENEVIEW
#define OOSG_SCENEVIEW

#include <osg/Node>
#include <osg/StateSet>
#include <osg/Light>
#include <osg/FrameStamp>
#include <osg/DisplaySettings>
#include <osgDB/DatabasePager>
#include <osg/CollectOccludersVisitor>
#include <osg/CullSettings>
#include <osg/Camera>

#include <osgUtil/CullVisitor>

///////////////////////////////////////////////////////////////////////////////////////////////////
class SceneView : public osg::Object, public osg::CullSettings
{
    public:

        /** Construct a default scene view.*/
        SceneView(osgDB::DatabasePager* db = NULL);

        SceneView(const SceneView& sceneview, const osg::CopyOp& copyop = osg::CopyOp());

        META_Object(osgUtil, SceneView);

        /** Set the camera used to represent the camera view of this SceneView.*/
        //void setCamera(osg::Camera* camera, bool assumeOwnershipOfCamera = true);

        /** Get the camera used to represent the camera view of this SceneView.*/
        osg::Camera* getCamera() { return _camera.get(); }

        /** Get the const camera used to represent the camera view of this SceneView.*/
        //const osg::Camera* getCamera() const { return _camera.get(); }

        /** Set the data to view. The data will typically be
         *  an osg::Scene but can be any osg::Node type.
         */
        void setSceneData(osg::Node* node);
        
        /** Get the scene data to view. The data will typically be
         *  an osg::Scene but can be any osg::Node type.
         */
        osg::Node* getSceneData(unsigned int childNo=0) { return (_camera->getNumChildren()>childNo) ? _camera->getChild(childNo) : 0; }

        /** Get the const scene data which to view. The data will typically be
         *  an osg::Scene but can be any osg::Node type.
         */
        //const osg::Node* getSceneData(unsigned int childNo=0) const { return (_camera->getNumChildren()>childNo) ? _camera->getChild(childNo) : 0; }
        
        /** Get the number of scene data subgraphs added to the SceneView's camera.*/
        //unsigned int getNumSceneData() const { return _camera->getNumChildren(); }

        /** Set the viewport of the scene view to use specified osg::Viewport. */
        //void setViewport(osg::Viewport* viewport) { _camera->setViewport(viewport); }

        /** Set the viewport of the scene view to specified dimensions. */
        //void setViewport(int x,int y,int width,int height) { _camera->setViewport(x,y,width,height); }

        /** Get the viewport. */
        //osg::Viewport* getViewport() { return (_camera->getViewport()!=0) ? _camera->getViewport() : 0; }

        /** Get the const viewport. */
        //const osg::Viewport* getViewport() const { return (_camera->getViewport()!=0) ? _camera->getViewport() : 0; }
        
        /** Set the DisplaySettings. */
        //inline void setDisplaySettings(osg::DisplaySettings* vs) { _displaySettings = vs; }
        
        /** Get the const DisplaySettings */
        //inline const osg::DisplaySettings* getDisplaySettings() const { return _displaySettings.get(); }

        /** Get the DisplaySettings */
        //inline osg::DisplaySettings* getDisplaySettings() { return _displaySettings.get(); }


        void setGlobalStateSet(osg::StateSet* state) { _globalStateSet = state; }
        osg::StateSet* getGlobalStateSet() { return _globalStateSet.get(); }
        //const osg::StateSet* getGlobalStateSet() const { return _globalStateSet.get(); }

        //void setSecondaryStateSet(osg::StateSet* state) { _secondaryStateSet = state; }
        //osg::StateSet* getSecondaryStateSet() { return _secondaryStateSet.get(); }
        //const osg::StateSet* getSecondaryStateSet() const { return _secondaryStateSet.get(); }

        void setLocalStateSet(osg::StateSet* state) { _localStateSet = state; }
        osg::StateSet* getLocalStateSet() { return _localStateSet.get(); }
        //const osg::StateSet* getLocalStateSet() const { return _localStateSet.get(); }

		void setAutoNearFar(bool value);
        
        enum ActiveUniforms
        {
            FRAME_NUMBER_UNIFORM            = 1,
            FRAME_TIME_UNIFORM              = 2,
            DELTA_FRAME_TIME_UNIFORM        = 4,
            SIMULATION_TIME_UNIFORM         = 8,
            DELTA_SIMULATION_TIME_UNIFORM   = 16,
            VIEW_MATRIX_UNIFORM             = 32,
            VIEW_MATRIX_INVERSE_UNIFORM     = 64,
            DEFAULT_UNIFORMS                = FRAME_NUMBER_UNIFORM |
                                              FRAME_TIME_UNIFORM |
                                              DELTA_FRAME_TIME_UNIFORM |
                                              SIMULATION_TIME_UNIFORM |
                                              DELTA_SIMULATION_TIME_UNIFORM |
                                              VIEW_MATRIX_UNIFORM |
                                              VIEW_MATRIX_INVERSE_UNIFORM,
            ALL_UNIFORMS                    = 0x7FFFFFFF
        };

        /** Set the uniforms that SceneView should set set up on each frame.*/        
        void setActiveUniforms(int activeUniforms) { _activeUniforms = activeUniforms; }

        /** Get the uniforms that SceneView should set set up on each frame.*/
        int getActiveUniforms() const { return _activeUniforms; }

        void updateUniforms(int eye);
        
        void setState(osg::State* state) { _renderInfo.setState(state); }
        osg::State* getState() { return _renderInfo.getState(); }
        const osg::State* getState() const { return _renderInfo.getState(); }
        
        void setView(osg::View* view) { _camera->setView(view); }
        osg::View* getView() { return _camera->getView(); }
        const osg::View* getView() const { return _camera->getView(); }

        void setRenderInfo(osg::RenderInfo& renderInfo) { _renderInfo = renderInfo; }
        osg::RenderInfo& getRenderInfo() { return _renderInfo; }
        const osg::RenderInfo& getRenderInfo() const { return _renderInfo; }
        


        /** Set the projection matrix. Can be thought of as setting the lens of a camera. */
        //inline void setProjectionMatrix(const osg::Matrixf& matrix) { _camera->setProjectionMatrix(matrix); }

        /** Get the projection matrix.*/
        //osg::Matrixd& getProjectionMatrix() { return _camera->getProjectionMatrix(); }

        /** Get the const projection matrix.*/
        //const osg::Matrixd& getProjectionMatrix() const { return _camera->getProjectionMatrix(); }

        /** Set the view matrix. Can be thought of as setting the position of the world relative to the camera in camera coordinates. */
        //inline void setViewMatrix(const osg::Matrixf& matrix) { _camera->setViewMatrix(matrix); }
        
        /** Get the view matrix. */
        //osg::Matrixd& getViewMatrix() { return _camera->getViewMatrix(); }

        /** Get the const view matrix. */
        //const osg::Matrixd& getViewMatrix() const { return _camera->getViewMatrix(); }

        /** Get to the position and orientation of a modelview matrix, using the same convention as gluLookAt. */
        //void getViewMatrixAsLookAt(osg::Vec3& eye,osg::Vec3& center,osg::Vec3& up,float lookDistance=1.0f) const;

        void setInitVisitor(osg::NodeVisitor* av) { _initVisitor = av; }
        osg::NodeVisitor* getInitVisitor() { return _initVisitor.get(); }
        const osg::NodeVisitor* getInitVisitor() const { return _initVisitor.get(); }

        //void setUpdateVisitor(osg::NodeVisitor* av) { _updateVisitor = av; }
        //osg::NodeVisitor* getUpdateVisitor() { return _updateVisitor.get(); }
        //const osg::NodeVisitor* getUpdateVisitor() const { return _updateVisitor.get(); }

        void setCullVisitor(osgUtil::CullVisitor* cv) { _cullVisitor = cv; }
        osgUtil::CullVisitor* getCullVisitor() { return _cullVisitor.get(); }
        const osgUtil::CullVisitor* getCullVisitor() const { return _cullVisitor.get(); }

        void setCollectOccludersVisitor(osg::CollectOccludersVisitor* cov) { _collectOccludersVisitor = cov; }
        osg::CollectOccludersVisitor* getCollectOccludersVisitor() { return _collectOccludersVisitor.get(); }
        const osg::CollectOccludersVisitor* getCollectOccludersVisitor() const { return _collectOccludersVisitor.get(); }


        void setStateGraph(osgUtil::StateGraph* rg) { _stateGraph = rg; }
        osgUtil::StateGraph* getStateGraph() { return _stateGraph.get(); }
        const osgUtil::StateGraph* getStateGraph() const { return _stateGraph.get(); }

        void setRenderStage(osgUtil::RenderStage* rs) { _renderStage = rs; }
        osgUtil::RenderStage* getRenderStage() { return _renderStage.get(); }
        const osgUtil::RenderStage* getRenderStage() const { return _renderStage.get(); }

        /** search through any pre and post RenderStage that reference a Camera, and take a reference to each of these cameras to prevent them being deleted while they are still be used by the drawing thread.*/
        void collateReferencesToDependentCameras();

        /** clear the refence to any any dependent cameras.*/
        void clearReferencesToDependentCameras();


        /** Set the draw buffer value used at the start of each frame draw. */
        void setDrawBufferValue( GLenum drawBufferValue ) { _camera->setDrawBuffer(drawBufferValue); }

        /** Get the draw buffer value used at the start of each frame draw. */
        GLenum getDrawBufferValue() const { return _camera->getDrawBuffer(); }


        /** Set whether the draw method should call renderer->prioritizeTexture.*/
        //void setPrioritizeTextures(bool pt) { _prioritizeTextures = pt; }
        
        /** Get whether the draw method should call renderer->prioritizeTexture.*/
        //bool getPrioritizeTextures() const { return _prioritizeTextures; }

        /** Set the frame stamp for the current frame.*/
        inline void setFrameStamp(osg::FrameStamp* fs) { _frameStamp = fs; }

        /** Get the frame stamp for the current frame.*/
        inline const osg::FrameStamp* getFrameStamp() const { return _frameStamp.get(); }

        /** Do init traversal of attached scene graph using Init NodeVisitor.
          * The init traversal is called once for each SceneView, and should
          * be used to compile display list, texture objects intialize data
          * not otherwise intialized during scene graph loading. Note, is
          * called automatically by update & cull if it hasn't already been called
          * elsewhere. Also init() should only ever be called within a valid
          * graphics context.*/
        virtual void initialize();

        /** Do cull traversal of attached scene graph using Cull NodeVisitor.*/
        virtual void cull(int eye);

        /** Do draw traversal of draw bins generated by cull traversal.*/
        virtual void draw();
        
        /** Compute the number of dynamic objects that will be held in the rendering backend */
       // unsigned int getReferenceTypeCount() const { return _dynamicObjectCount; }
        
        /** Release all OpenGL objects from the scene graph, such as texture objects, display lists etc.
          * These released scene graphs placed in the respective delete GLObjects cache, which
          * then need to be deleted in OpenGL by SceneView::flushAllDeleteGLObjects(). */
        virtual void releaseAllGLObjects();

        /** Flush all deleted OpenGL objects, such as texture objects, display lists etc.*/
        virtual void flushAllDeletedGLObjects();

        /** Flush deleted OpenGL objects, such as texture objects, display lists etc within specified available time.*/
        virtual void flushDeletedGLObjects(double& availableTime);
        
        /** Extract stats for current draw list. */
        bool getStats(osgUtil::Statistics& primStats); 

		unsigned int getTriangleCount();

    protected:

		virtual ~SceneView();

        /** Do cull traversal of attached scene graph using Cull NodeVisitor. Return true if computeNearFar has been done during the cull traversal.*/
        virtual bool cullStage(const osg::Matrixd& projection,const osg::Matrixd& modelview,osgUtil::CullVisitor* cullVisitor, osgUtil::StateGraph* rendergraph, osgUtil::RenderStage* renderStage, osg::Viewport *viewport);
        
        //void clearArea(int x,int y,int width,int height,const osg::Vec4& color);

        osg::ref_ptr<osg::StateSet>                 _localStateSet;
        osg::RenderInfo                             _renderInfo;
        
        bool                                        _initCalled;
        osg::ref_ptr<osg::NodeVisitor>              _initVisitor;
        //osg::ref_ptr<osg::NodeVisitor>              _updateVisitor;
        osg::ref_ptr<osgUtil::CullVisitor>          _cullVisitor;
        osg::ref_ptr<osgUtil::StateGraph>           _stateGraph;
        osg::ref_ptr<osgUtil::RenderStage>          _renderStage;

        osg::ref_ptr<osg::CollectOccludersVisitor>  _collectOccludersVisitor;
        
        osg::ref_ptr<osg::FrameStamp>               _frameStamp;
        
        osg::observer_ptr<osg::Camera>              _camera;
        //osg::ref_ptr<osg::Camera>                   _cameraWithOwnership;
        
        osg::ref_ptr<osg::StateSet>                 _globalStateSet;
        //osg::ref_ptr<osg::DisplaySettings>          _displaySettings;
        
        osg::ref_ptr<osg::StateSet>                 _secondaryStateSet;

		osg::ref_ptr<osgDB::DatabasePager>			_databasePager;

        //float                                       _fusionDistanceValue;

        bool                                        _prioritizeTextures;
        
        bool                                        _requiresFlush;
        
        int                                         _activeUniforms;        
        double                                      _previousFrameTime;
        double                                      _previousSimulationTime;
        
        unsigned int                                _dynamicObjectCount;        

		int _triangleCount;
};

#endif

