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
#ifndef EQ_INTERNAL
#define EQ_INTERNAL

#include "omega/osystem.h"
#include "omega/Application.h"
#include "omega/RenderTarget.h"
#include "omega/EqualizerDisplaySystem.h"

#define EQ_IGNORE_GLEW

// Equalizer includes
#include "eq/eq.h"
#include "co/co.h"

#ifdef OMEGA_OS_WIN
// This include is needed to use Layout::findView since equalizer code doesn't use this method, and its 
// template definition isn't compiled in the original equalizer static library.
#include "eq/../../../equalizer/libs/fabric/layout.ipp"
#endif

// Define to enable debugging of equalizer flow.
//#define OMEGA_DEBUG_EQ_FLOW
#ifdef OMEGA_DEBUG_EQ_FLOW
    #define DEBUG_EQ_FLOW(msg, id) ofmsg("EQ_FLOW " msg, id);
#else
    #define DEBUG_EQ_FLOW(msg, id)
#endif 


using namespace omega;
using namespace co::base;
using namespace std;

namespace omicron {
    class EventUtils
    {
    public:
        static void serializeEvent(Event& evt, co::DataOStream& os);
        static void deserializeEvent(Event& evt, co::DataIStream& is);
    private:
        EventUtils() {}
    };
};

namespace omega {
    class RenderTarget;
	class Camera;

///////////////////////////////////////////////////////////////////////////////////////////////////
class SharedData: public co::Object
{
public:
	void registerObject(SharedObject* object, const String& id);
	void unregisterObject(const String& id);
	virtual ChangeType getChangeType() const { return INSTANCE; }
	void setUpdateContext(const UpdateContext& ctx) { myUpdateContext = ctx; }
	const UpdateContext& getUpdateContext() { return myUpdateContext; }


protected:
	virtual void getInstanceData( co::DataOStream& os );
	virtual void applyInstanceData( co::DataIStream& is );

private:
	Dictionary<String, SharedObject*> myObjects;
	typedef Dictionary<String, SharedObject*>::Item SharedObjectItem;
	UpdateContext myUpdateContext;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal Keeps observer per-tile data
struct ObserverTileData
{
	ObserverTileData(): observer(NULL)
	{}

	eq::Observer* observer;
	int x;
	int y;

	// CAVE2 SIMPLIFICATION: We are just interested in adjusting the observer yaw
	float yaw;
	
	Vector3f tileCenter;

	omega::Camera* camera;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ConfigImpl: public eq::Config
{
public:
    //EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    static const int MaxCanvasChannels = 128;
	static const int MaxTiles = 4096;
public:
    ConfigImpl( co::base::RefPtr< eq::Server > parent);
	virtual ~ConfigImpl();
    virtual bool init();
    virtual bool exit();
	void mapSharedData(const uint128_t& initID);
	void updateSharedData();
    virtual bool handleEvent(const eq::ConfigEvent* event);
    virtual uint32_t startFrame( const uint128_t& version );
    virtual uint32_t finishFrame();
	const UpdateContext& getUpdateContext();
	// Refresh information for all observers. Used when camera attachments change.
	void updateObserverCameras();

private:
    void processMousePosition(eq::Window* source, int x, int y, Vector2i& outPosition, Ray& ray);
    uint processMouseButtons(uint btns); 

private:
	SharedData mySharedData;
	Timer myGlobalTimer;
	//! Global fps counter.
	Stat* myFpsStat;

	ObserverTileData myObserverTileData[MaxTiles];

    omicron::Ref<Engine> myServer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class NodeImpl: public eq::Node
{
public:
    //EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    NodeImpl( eq::Config* parent );

protected:
    virtual bool configInit( const eq::uint128_t& initID );
    virtual bool configExit();
    virtual void frameStart( const eq::uint128_t& frameID, const uint32_t frameNumber );

private:
    //bool myInitialized;
    omicron::Ref<Engine> myServer;
    //FrameData myFrameData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class PipeImpl: public eq::Pipe
{
public:
    //EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    PipeImpl(eq::Node* parent);
    GpuContext* getGpuContext() { return myGpuContext.get(); }

protected:
    virtual ~PipeImpl();
    virtual bool configInit(const uint128_t& initID);
private:
	NodeImpl* myNode;
    omicron::Ref<GpuContext> myGpuContext;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
//! A Window represents an on-screen or off-screen drawable. A drawable is a 2D rendering surface, 
//! typically attached to an OpenGL context. A Window is a child of a Pipe. The task methods for all windows 
//! of a pipe are executed in the same pipe thread. The default window initialization methods initialize all windows 
//! of the same pipe with a shared context, so that OpenGL objects can be reused between them for optimal GPU memory usage.
class WindowImpl: public eq::Window
{
public:
    WindowImpl(eq::Pipe* parent);
    virtual ~WindowImpl();

	EqualizerDisplaySystem* getDisplaySystem() { return (EqualizerDisplaySystem*)SystemManager::instance()->getDisplaySystem(); }

	DisplayTileConfig* getTileConfig() { return myTile; }
    Renderer* getRenderer();

protected:
    virtual bool configInit(const uint128_t& initID);
    virtual void frameStart( const uint128_t& frameID, const uint32_t frameNumber );
    virtual void frameFinish( const uint128_t& frameID, const uint32_t frameNumber );
	bool processEvent(const eq::Event& event);

private:
	PipeImpl* myPipe;
    omicron::Ref<Renderer> myRenderer;
    DisplayTileConfig* myTile;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ChannelImpl: public eq::Channel
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    ChannelImpl( eq::Window* parent );
    virtual ~ChannelImpl();

    ViewImpl* getViewImpl();
	
	DrawContext& getDrawContext() { return myDC; }

protected:
    void setupDrawContext(DrawContext* context, const uint128_t& spin);

	void drawStats();
    virtual bool configInit(const uint128_t& initID);
    virtual void frameDraw( const uint128_t& spin );
    virtual void frameViewFinish(const uint128_t& spin);
	//virtual void frameViewStart( const co::base::uint128_t& frameID );

    omega::Renderer* getRenderer();

	void setupStencil(int gliWindowWidth, int gliWindowHeight);

private:
    eq::Window* myWindow;
    omicron::Lock myLock;
    DrawContext myDC;
    uint128_t myLastFrame;
    omicron::Ref<RenderTarget> myDrawBuffer;

	bool myStencilInitialized;

	omicron::Ref<Texture> myStatsTexture;
    omicron::Ref<RenderTarget> myStatsBuffer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class EqualizerNodeFactory: public eq::NodeFactory
{
public:
    EqualizerNodeFactory() {}
public:
    virtual eq::Config*  createConfig( eq::ServerPtr parent )
        { return new ConfigImpl( parent ); }
    virtual eq::Channel* createChannel(eq::Window* parent)
        { return new ChannelImpl( parent ); }
    //virtual eq::View* createView(eq::Layout* parent)
    //    { return new ViewImpl(parent); }
    virtual eq::Window* createWindow(eq::Pipe* parent)
        { return new WindowImpl(parent); }
    virtual eq::Pipe* createPipe(eq::Node* parent)
        { return new PipeImpl(parent); }
    virtual eq::Node* createNode( eq::Config* parent )
       { return new NodeImpl( parent ); }
};

}; // namespace omega

#endif