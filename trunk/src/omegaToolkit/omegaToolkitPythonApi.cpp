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
 * Original code Copyright (c) Kitware, Inc.
 * All rights reserved.
 * See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
 *************************************************************************************************/
#include "omega/PythonInterpreter.h"
#include "omega/SystemManager.h"
#include "omega/Engine.h"
#include "omegaToolkit/SceneEditorModule.h"
#include "omegaToolkit/UiModule.h"
#include "omegaToolkit/ui/MenuManager.h"
#include "omegaToolkit/ToolkitUtils.h"
#ifdef OMEGA_USE_PORTHOLE
	#include "omegaToolkit/PortholeService.h"
#endif

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

#define BOOST_PYTHON_NO_LIB
#include <boost/python.hpp>
using namespace boost::python;

#ifdef OMEGA_USE_PORTHOLE
///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* getCameraById(int id)
{
	if(SystemManager::instance()->isMaster())
	{
		return PortholeGUI::CamerasMap[id]->camera;
	}
	return NULL;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omegaToolkit)
{
	// Container
	PYAPI_ENUM(Container::Layout, ContainerLayout)
		PYAPI_ENUM_VALUE(Container, LayoutFree)
		PYAPI_ENUM_VALUE(Container, LayoutHorizontal)
		PYAPI_ENUM_VALUE(Container, LayoutVertical)
		//PYAPI_ENUM_VALUE(Container, LayoutGrid)
		;

	// MenuItem
	PYAPI_ENUM(MenuItem::Type, MenuItemType)
		PYAPI_ENUM_VALUE(MenuItem, Button)
		PYAPI_ENUM_VALUE(MenuItem, Checkbox)
		PYAPI_ENUM_VALUE(MenuItem, Label)
		PYAPI_ENUM_VALUE(MenuItem, Slider)
		PYAPI_ENUM_VALUE(MenuItem, SubMenu)
		PYAPI_ENUM_VALUE(MenuItem, Container)
		;

	// Widget Layer
	PYAPI_ENUM(Widget::Layer, WidgetLayer)
		PYAPI_ENUM_VALUE(Widget, Front)
		PYAPI_ENUM_VALUE(Widget, Middle)
		PYAPI_ENUM_VALUE(Widget, Back)
		;

	// Widget Blend Mode
	PYAPI_ENUM(Widget::BlendMode, WidgetBlendMode)
		PYAPI_ENUM_VALUE(Widget, BlendInherit)
		PYAPI_ENUM_VALUE(Widget, BlendNormal)
		PYAPI_ENUM_VALUE(Widget, BlendAdditive)
		;

	PYAPI_BASE_CLASS(ToolkitUtils)
		PYAPI_STATIC_REF_GETTER(ToolkitUtils, createInteractor)
		PYAPI_STATIC_REF_GETTER(ToolkitUtils, setupInteractor)
		PYAPI_STATIC_REF_GETTER(ToolkitUtils, getKinectDepthCameraImage)
		;

	// MenuManager
	PYAPI_REF_BASE_CLASS(MenuManager)
		PYAPI_STATIC_REF_GETTER(MenuManager, createAndInitialize)
		PYAPI_REF_GETTER(MenuManager, createMenu)
		PYAPI_METHOD(MenuManager, setMainMenu)
		PYAPI_REF_GETTER(MenuManager, getMainMenu)
		;

	// Menu
	PYAPI_REF_BASE_CLASS(Menu)
		PYAPI_REF_GETTER(Menu, addItem)
		PYAPI_REF_GETTER(Menu, addButton)
		PYAPI_REF_GETTER(Menu, addSubMenu)
		PYAPI_REF_GETTER(Menu, addLabel)
		PYAPI_REF_GETTER(Menu, addSlider)
		PYAPI_REF_GETTER(Menu, addImage)
		PYAPI_REF_GETTER(Menu, addContainer)
		PYAPI_REF_GETTER(Menu, getContainer)
		PYAPI_METHOD(Menu, show)
		PYAPI_METHOD(Menu, hide)
		PYAPI_METHOD(Menu, isVisible)
		//PYAPI_METHOD(Menu, setLabel)
		//PYAPI_METHOD(Menu, getLabel)
		PYAPI_METHOD(Menu, placeOnWand)
		;

	PYAPI_REF_BASE_CLASS(MenuItem)
		//PYAPI_REF_GETTER(MenuItem, addItem)
		PYAPI_REF_GETTER(MenuItem, getSubMenu)
		PYAPI_REF_GETTER(MenuItem, getWidget)
		PYAPI_REF_GETTER(MenuItem, getButton)
		PYAPI_REF_GETTER(MenuItem, getLabel)
		PYAPI_REF_GETTER(MenuItem, getSlider)
		PYAPI_REF_GETTER(MenuItem, getImage)
		PYAPI_REF_GETTER(MenuItem, getContainer)
		PYAPI_GETTER(MenuItem, getText)
		PYAPI_METHOD(MenuItem, setText)
		PYAPI_GETTER(MenuItem, getDescription)
		PYAPI_METHOD(MenuItem, setDescription)
		PYAPI_METHOD(MenuItem, setCommand)
		PYAPI_GETTER(MenuItem, getCommand)
		PYAPI_METHOD(MenuItem, setChecked)
		PYAPI_METHOD(MenuItem, isChecked)
		PYAPI_METHOD(MenuItem, setUserTag)
		PYAPI_GETTER(MenuItem, getUserTag)
		PYAPI_GETTER(MenuItem, setImage)
		;

	// Container3dSettings
	PYAPI_BASE_CLASS(Container3dSettings)
		PYAPI_PROPERTY(Container3dSettings, enable3d)
		PYAPI_PROPERTY(Container3dSettings, position)
		PYAPI_PROPERTY(Container3dSettings, normal)
		PYAPI_PROPERTY(Container3dSettings, scale)
		PYAPI_PROPERTY(Container3dSettings, up)
		PYAPI_PROPERTY(Container3dSettings, alpha)
		PYAPI_PROPERTY(Container3dSettings, node)
		;

	// UiModule
	PYAPI_REF_BASE_CLASS(UiModule)
		PYAPI_STATIC_REF_GETTER(UiModule, instance)
		PYAPI_STATIC_REF_GETTER(UiModule, createAndInitialize)
		PYAPI_REF_GETTER(UiModule, getWidgetFactory)
		PYAPI_REF_GETTER(UiModule, getUi)
		;

	// WidgetFactory
	PYAPI_REF_BASE_CLASS(WidgetFactory)
		PYAPI_REF_GETTER(WidgetFactory, createButton)
		PYAPI_REF_GETTER(WidgetFactory, createSlider)
		PYAPI_REF_GETTER(WidgetFactory, createCheckButton)
		PYAPI_REF_GETTER(WidgetFactory, createImage)
		PYAPI_REF_GETTER(WidgetFactory, createLabel)
		PYAPI_REF_GETTER(WidgetFactory, createContainer)
		;

	// Widget
	void (Widget::*setPosition1)(const Vector2f&) = &Widget::setPosition;
	void (Widget::*setSize1)(const Vector2f&) = &Widget::setSize;
	PYAPI_REF_BASE_CLASS(Widget)
		PYAPI_METHOD(Widget, setVisible)
		PYAPI_METHOD(Widget, isVisible)
		.def("setPosition", setPosition1)
		PYAPI_METHOD(Widget, setCenter)
		PYAPI_GETTER(Widget, getCenter)
		PYAPI_GETTER(Widget, getPosition)
		.def("setSize", setSize1)
		PYAPI_METHOD(Widget, setWidth)
		PYAPI_METHOD(Widget, getWidth)
		PYAPI_METHOD(Widget, setHeight)
		PYAPI_METHOD(Widget, getHeight)
		PYAPI_GETTER(Widget, getSize)
		PYAPI_METHOD(Widget, setName)
		PYAPI_GETTER(Widget, getName)
		PYAPI_METHOD(Widget, setStyle)
		PYAPI_METHOD(Widget, setStyleValue)
		PYAPI_METHOD(Widget, refresh)
		PYAPI_METHOD(Widget, hitTest)
		PYAPI_METHOD(Widget, setUIEventCommand)
		PYAPI_METHOD(Widget, setLayer)
		PYAPI_METHOD(Widget, getLayer)
		PYAPI_METHOD(Widget, getAutosize)
		PYAPI_METHOD(Widget, setAutosize)
		PYAPI_GETTER(Widget, transformPoint)
		PYAPI_METHOD(Widget, setAlpha)
		PYAPI_METHOD(Widget, getAlpha)
		PYAPI_METHOD(Widget, setBlendMode)
		PYAPI_METHOD(Widget, getBlendMode)
		PYAPI_METHOD(Widget, setScale)
		PYAPI_METHOD(Widget, getScale)
		PYAPI_METHOD(Widget, setUpdateCommand)
		PYAPI_GETTER(Widget, getUpdateCommand)
		PYAPI_METHOD(Widget, isStereo)
		PYAPI_METHOD(Widget, setStereo)
		;

	// Container
	PYAPI_REF_CLASS(Container, Widget)
		.def("get3dSettings", &Container::get3dSettings, PYAPI_RETURN_INTERNAL_REF)
		PYAPI_METHOD(Container, isPixelOutputEnabled)
		PYAPI_METHOD(Container, setPixelOutputEnabled)
		PYAPI_REF_GETTER(Container, getPixels)
		PYAPI_METHOD(Container, addChild)
		PYAPI_METHOD(Container, removeChild)
		PYAPI_REF_GETTER(Container, getChildByIndex)
		PYAPI_REF_GETTER(Container, getChildByName)
		;

	// Button
	PYAPI_REF_CLASS(Button, Widget)
		PYAPI_METHOD(Button, getText)
		PYAPI_METHOD(Button, setText)
		PYAPI_METHOD(Button, isCheckable)
		PYAPI_METHOD(Button, setCheckable)
		PYAPI_METHOD(Button, setChecked)
		PYAPI_METHOD(Button, isChecked)
		PYAPI_METHOD(Button, setRadio)
		PYAPI_METHOD(Button, isRadio)
		PYAPI_METHOD(Button, setIcon)
		PYAPI_REF_GETTER(Button, getIcon)
		PYAPI_REF_GETTER(Button, getImage)
		;

	// Image
	PYAPI_REF_CLASS(Image, Widget)
		PYAPI_STATIC_REF_GETTER(Image, create)
		PYAPI_REF_GETTER(Image, getData)
		PYAPI_METHOD(Image, setData)
		;

	// Slider
	PYAPI_REF_CLASS(Slider, Widget)
		PYAPI_METHOD(Slider, getTicks)
		PYAPI_METHOD(Slider, setTicks)
		PYAPI_METHOD(Slider, setValue)
		PYAPI_METHOD(Slider, getValue)
		;

	// Label
	PYAPI_REF_CLASS(Label, Widget)
		PYAPI_METHOD(Label, getText)
		PYAPI_METHOD(Label, setText)
		PYAPI_METHOD(Label, setColor)
		PYAPI_METHOD(Label, setFont)
		PYAPI_METHOD(Label, getFont)
		;

#ifdef OMEGA_USE_PORTHOLE
	// PortholeService
	PYAPI_REF_BASE_CLASS(PortholeService)
		PYAPI_STATIC_REF_GETTER(PortholeService, createAndInitialize)
		;

	def("getCameraById", getCameraById, PYAPI_RETURN_REF);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OTK_API omegaToolkitPythonApiInit()
{
	static bool sApiInitialized = false;

	if(!sApiInitialized)
	{
		sApiInitialized = true;
		omsg("omegaToolkitPythonApiInit()");
		initomegaToolkit();

		// import the module by default
		omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->eval("from omegaToolkit import *");
	}
}

#endif