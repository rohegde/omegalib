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
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "omegaToolkit/omegaToolkitConfig.h"
#include "omega/DrawInterface.h"
#include "omega/Renderable.h"

namespace omegaToolkit { namespace ui {
    class Container;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    class OTK_API Widget: public RenderableFactory, IEventListener
    {
    friend class UiManager;
    friend class UiRenderPass;
    friend class Container;
    friend class WidgetRenderable;
    public:
        Widget(Engine* server);
        virtual ~Widget();

        virtual Renderable* createRenderable();

        virtual void handleEvent(const Event& evt);
        virtual void update(const omega::UpdateContext& context);

        //! Sets the event listener used to handle events generated by this widget.
        void setUIEventHandler(IEventListener* value);
        //! @see setWidgetEventHandler
        IEventListener* getUIEventHandler();

        //! Gets the color used when widget debug mode is enabled.
        Color getDebugColor() { return myDebugModeColor; }

        //! Sets the color used when widget debug mode is enabled.
        void setDebugColor( omega::Color value ) { myDebugModeColor = value; }

        //! Returns true if debug mode is enabled for this widget.
        bool isDebugModeEnabled() { return myDebugModeEnabled; }

        //! Enabled or disabled debug mode for this widget.
        //! When debug mode is enabled, the widget bounding box will be displayed.
        void setDebugModeEnabled(bool value) { myDebugModeEnabled = value; }

        //! Returns the widget name.
        String getName();
        //! Sets the widget name.
        void setName(const String& name);

        //! Position and rotation
        //@{
        //! Gets the widget position.
        Vector2f& getPosition() { return myPosition; }
        //! Sets the widget position
        void setPosition(const omega::Vector2f& value) { myPosition = value; }
        void setPosition(int value, int dimension) { myPosition[dimension] = value; }
        //! Sets the widget rotation
        //! @param value - the widget rotation in degrees
        void setRotation(float value) { myRotation = value; }
        //! Gets the widget position.
        float getRotation() { return myRotation; }
        //@}

        //! sets the widget scale.
        //void setScale(float value) { myScale = value; }
        //! Gets the widget scale.
        //float getScale() { return myScale; }

        //! Size constraints
        //@{
        //! Gets the widget size
        Vector2f& getSize();
        float getSize(Orientation orientation);
        float getWidth();
        float getHeight();
        //! Sets the widget size.
        void setSize(const Vector2f& value);
        void setWidth(float value);
        void setHeight(float value);
        Vector2f& getMinimumSize();
        Vector2f& getMaximumSize();
        void setMinimumSize(const Vector2f& value);
        int getMinimumWidth();
        int getMinimumHeight();
        void setMaximumSize(const Vector2f& value);
        int getMaximumWidth();
        int getMaximumHeight();
        void setMinimumWidth(float value);
        void setMinimumHeight(float value);
        void setMaximumWidth(float value);
        void setMaximumHeight(float value);
        void clearSizeConstaints();
        void setAutosize(bool value);
        bool getAutosize();
        //@}

        bool isVisible();
        void setVisible(bool value);

		bool isActive() { return myActive; }
		void setActive(bool value) 
		{
			myActive = value; 
			//if(myActive != value)
			{
				if(value) activate();
				else deactivate();
			}
			//ofmsg("Widget %1% active: %2%", %myId %value);
		} 

        bool isUserMoveEnabled() { return myUserMoveEnabled; }
        void setUserMoveEnabled(bool value) { myUserMoveEnabled = value; }
        
        bool hitTest(const omega::Vector2f& point);
        static bool hitTest(const omega::Vector2f& point, const omega::Vector2f& pos, const omega::Vector2f& size);

        //! Returns the unique Widget id.
        int getId();
        virtual void layout();

        void setStereo(bool value) { myStereo = value; }
        bool isStereo() { return myStereo; }

		bool isGamepadInteractionEnabled();
		bool isPointerInteractionEnabled();

		void setHorizontalNextWidget(Widget* value) { myHorizontalNextWidget = value; }
		Widget* getHorizontalNextWidget() { return myHorizontalNextWidget; }

		void setHorizontalPrevWidget(Widget* value) { myHorizontalPrevWidget = value; }
		Widget* getHorizontalPrevWidget() { return myHorizontalPrevWidget; }

		void setVerticalNextWidget(Widget* value) { myVerticalNextWidget = value; }
		Widget* getVerticalNextWidget() { return myVerticalNextWidget; }

		void setVerticalPrevWidget(Widget* value) { myVerticalPrevWidget = value; }
		Widget* getVerticalPrevWidget() { return myVerticalPrevWidget; }

        virtual void autosize(Renderer* r) {}
        virtual void updateSize(Renderer* r);

    protected:
		virtual void activate() { }
		virtual void deactivate() {}

        //! internal layout management
        //@{
        void requestLayoutRefresh();
        bool needLayoutRefresh();
        void setActualSize(int value, Orientation orientation, bool force = false);
        //@}

        void setContainer(Container* value);
		Container* getContainer() { return myContainer; }
        Vector2f transformPoint(const omega::Vector2f& point);
        void dispatchUIEvent(const Event& evt);

    protected:
        omega::Vector2f myPosition;
        omega::Vector2f mySize;
        float myRotation;
        //float myScale;

    private:
        bool myStereo;
        bool myInitialized;

        Engine* myServer;

        static NameGenerator mysNameGenerator;

        unsigned int myId;
		// Widget links for button-based navigation
		Widget* myHorizontalNextWidget;
		Widget* myHorizontalPrevWidget;
		Widget* myVerticalPrevWidget;
		Widget* myVerticalNextWidget;

        omega::String myName;
        Container* myContainer;

        IEventListener* myEventHandler;

        bool myNeedLayoutRefresh;

        // Debug mode.
        bool myDebugModeEnabled;
        omega::Color myDebugModeColor;

        bool myUserMoveEnabled;
        bool myMoving;
        omega::Vector2f myUserMovePosition;

        bool myVisible;
        bool myActive;

        // Size constraints.
        omega::Vector2f myMinimumSize;
        omega::Vector2f myMaximumSize;
        bool myAutosize;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    class OTK_API WidgetRenderable: public Renderable
    {
    public:
        WidgetRenderable(Widget* owner): myOwner(owner) {}
        virtual void draw(RenderState* state);
        virtual void drawContent();

    protected:
        virtual void preDraw();
        virtual void postDraw();

        //! Gets the current renderstate (accessible inside drawContent)
        RenderState* getRenderState() { return myRenderState; }

    private:
        Widget* myOwner;
        RenderState* myRenderState;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline String Widget::getName() 
    { return myName; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setName(const String& name)
    { myName = name; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline IEventListener* Widget::getUIEventHandler() 
    { return myEventHandler; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setUIEventHandler(IEventListener* value)
    { myEventHandler = value; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setAutosize(bool value)
    { 
        myAutosize = value; 
        requestLayoutRefresh();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline bool Widget::getAutosize()
    { return myAutosize; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline Vector2f& Widget::getSize() 
    { return mySize; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline float Widget::getSize(Orientation orientation)
    {
        return mySize[orientation];
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline float Widget::getWidth()
    { return mySize[0]; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline float Widget::getHeight() 
    { return mySize[1]; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setSize(const omega::Vector2f& value) 
    { 
        requestLayoutRefresh(); 
        mySize = value; 
        myMinimumSize = value; 
        myMaximumSize = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setWidth(float value) 
    { 
        requestLayoutRefresh(); 
        mySize[0] = value; 
        myMinimumSize[0] = value; 
        myMaximumSize[0] = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setHeight(float value) 
    { 
        requestLayoutRefresh(); 
        mySize[1] = value; 
        myMinimumSize[1] = value; 
        myMaximumSize[1] = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline Vector2f& Widget::getMinimumSize() 
    { return myMinimumSize; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline Vector2f& Widget::getMaximumSize() 
    { return myMaximumSize; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setMinimumSize(const Vector2f& value) 
    {
        requestLayoutRefresh(); 
        myMinimumSize = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline int Widget::getMinimumWidth() 
    { return myMinimumSize[0]; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline int Widget::getMinimumHeight() 
    { return myMinimumSize[1]; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setMaximumSize(const Vector2f& value) 
    {
        requestLayoutRefresh(); 
        myMaximumSize = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline int Widget::getMaximumWidth() 
    { return myMaximumSize[0]; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline int Widget::getMaximumHeight() 
    { return myMaximumSize[1]; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setMinimumWidth(float value) 
    { 
        requestLayoutRefresh(); 
        myMinimumSize[0] = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setMinimumHeight(float value) 
    {
        requestLayoutRefresh(); 
        myMinimumSize[1] = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setMaximumWidth(float value) 
    { 
        requestLayoutRefresh(); 
        myMaximumSize[0] = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setMaximumHeight(float value) 
    { 
        requestLayoutRefresh(); 
        myMaximumSize[1] = value; 
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline bool Widget::isVisible() 
    { return myVisible; }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    inline void Widget::setVisible(bool value) 
    { myVisible = value; }
};
}; // namespace omegaToolkit

#endif