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

///////////////////////////////////////////////////////////////////////////////////////////////////
class SharedData: public co::Object
{
public:
	void registerObject(SharedObject* object, const String& id);
	virtual ChangeType getChangeType() const { return INSTANCE; }

protected:
	virtual void getInstanceData( co::DataOStream& os );
	virtual void applyInstanceData( co::DataIStream& is );

private:
	Dictionary<String, SharedObject*> myObjects;
	typedef Dictionary<String, SharedObject*>::Item SharedObjectItem;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
//!  Frame-specific data.
//!   The frame-specific data is used as a per-config distributed object and contains mutable, rendering-relevant data. 
//!   Each rendering thread (pipe) keeps its own instance synchronized with the frame currently being rendered. The 
//!   data is managed by the Config, which modifies it directly.
class FrameData : public eq::fabric::Serializable
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    FrameData()	{ }

    virtual ~FrameData() {};

    int getNumEvents() { return myNumEvents; }
    void setNumEvents(int value) { myNumEvents = value; }
    Event& getEvent(int index) { return myEventBuffer[index]; }

    void setDirtyEvents() { setDirty( DIRTY_EVENTS ); }


protected:
    //! Serialize an instance of this class.
    virtual void serialize( co::DataOStream& os, const uint64_t dirtyBits )
    {
        eq::fabric::Serializable::serialize( os, dirtyBits );
        if( dirtyBits & DIRTY_EVENTS )
        {
            os << myNumEvents;
            for(int i = 0; i < myNumEvents; i++)
            {
                EventUtils::serializeEvent(myEventBuffer[i], os);
            }
        }
    }

    //! Deserialize an instance of this class.
    virtual void deserialize( co::DataIStream& is, const uint64_t dirtyBits )
    {
        eq::fabric::Serializable::deserialize( is, dirtyBits );
        if( dirtyBits & DIRTY_EVENTS )
        {
            is >> myNumEvents;
            for(int i = 0; i < myNumEvents; i++)
            {
                EventUtils::deserializeEvent(myEventBuffer[i], is);
            }
        }
    }

    enum DirtyBits
    {
        DIRTY_EVENTS   = eq::fabric::Serializable::DIRTY_CUSTOM << 1,
    };

private:
    int myNumEvents;
    Event myEventBuffer[ OMICRON_MAX_EVENTS ];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class ViewImpl;
class ViewProxy : public eq::fabric::Serializable
{
public:
    ViewProxy(ViewImpl* view);

protected:
    /** The changed parts of the view. */
    enum DirtyBits
    {
        DIRTY_DRAW_STATS       = eq::fabric::Serializable::DIRTY_CUSTOM << 1,
        DIRTY_DRAW_FPS       = eq::fabric::Serializable::DIRTY_CUSTOM << 2,
    };

    virtual void serialize( co::DataOStream& os, const uint64_t dirtyBits );
    virtual void deserialize( co::DataIStream& is, const uint64_t dirtyBits );
    virtual void notifyNewVersion();

private:
    ViewImpl* myView;
    friend class ViewImpl;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ViewImpl: public eq::View
{
friend class ViewProxy;
public:
    ViewImpl(eq::Layout* parent);
    ~ViewImpl();

    void drawStatistics(bool enable);
    bool isDrawStatisticsEnabled();
    void drawFps(bool enable);
    bool isDrawFpsEnabled();

private:
    bool myDrawStatistics;
    bool myDrawFps;
    ViewProxy myProxy;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class ConfigImpl: public eq::Config
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    static const int MaxCanvasChannels = 128;
public:
    ConfigImpl( co::base::RefPtr< eq::Server > parent);
    virtual bool init();
    virtual bool exit();
	void mapSharedData(const uint128_t& initID);
	void updateSharedData();
    virtual bool handleEvent(const eq::ConfigEvent* event);
    virtual uint32_t startFrame( const uint128_t& version );
    virtual uint32_t finishFrame();
    ViewImpl* findView(const String& viewName);
    //const FrameData& getFrameData();

private:
    void processMousePosition(eq::Window* source, int x, int y, Vector2i& outPosition, Ray& ray);
    uint processMouseButtons(uint btns); 

private:
    //FrameData myFrameData;
	SharedData mySharedData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class NodeImpl: public eq::Node
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    NodeImpl( eq::Config* parent );

    //! Returns the application server instance running on this node.
    ServerBase* getApplicationServer();

protected:
    virtual bool configInit( const eq::uint128_t& initID );
    virtual bool configExit();
    virtual void frameStart( const eq::uint128_t& frameID, const uint32_t frameNumber );

private:
    //bool myInitialized;
    ServerBase* myServer;
    //FrameData myFrameData;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
inline ServerBase* NodeImpl::getApplicationServer()
{ return myServer; }

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @internal
class PipeImpl: public eq::Pipe
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    PipeImpl(eq::Node* parent);
    RendererBase* getClient();
    GpuContext* getGpuContext() { return myGpuContext; }

protected:
    virtual ~PipeImpl();
    virtual bool configInit(const uint128_t& initID);
    virtual void frameStart( const uint128_t& frameID, const uint32_t frameNumber );
    virtual void frameFinish( const uint128_t& frameID, const uint32_t frameNumber );

private:
	NodeImpl* myNode;
    RendererBase* myClient;
    GpuManager* myGpu;
    GpuContext* myGpuContext;
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

protected:
    virtual bool configInit(const uint128_t& initID);
	bool processEvent(const eq::Event& event);

private:
    Vector2i myIndex;
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

protected:
    void setupDrawContext(DrawContext* context, const uint128_t& spin);

    virtual bool configInit(const uint128_t& initID);
    virtual void frameDraw( const uint128_t& spin );
    virtual void frameViewFinish(const uint128_t& spin);

    omega::RendererBase* getClient();
    bool isDrawStatisticsEnabled();
    bool isDrawFpsEnabled();

private:
    eq::Window* myWindow;
    omicron::Lock myLock;
    ViewImpl* myView;
    DrawContext myDC;
    uint128_t myLastFrame;
    RenderTarget* myDrawBuffer;
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
    virtual eq::View* createView(eq::Layout* parent)
        { return new ViewImpl(parent); }
    virtual eq::Window* createWindow(eq::Pipe* parent)
        { return new WindowImpl(parent); }
    virtual eq::Pipe* createPipe(eq::Node* parent)
        { return new PipeImpl(parent); }
    virtual eq::Node* createNode( eq::Config* parent )
       { return new NodeImpl( parent ); }
};

}; // namespace omega

#endif