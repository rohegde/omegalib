/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
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

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* uiImageLoad(PyObject* self, PyObject* args)
//{
//	// Create a mouse interactor and associate it with our scene node.
//	Actor* actor = new DefaultMouseInteractor();
//	ModuleServices::addModule(actor);
//
//	ui::Image* img = myWidgetFactory->createImage("img", UiModule::instance()->getUi());
//
//	bool stereo = Config::getBoolValue("stereo", imageSetting, false);
//
//	img->setStereo(stereo);
//	ImageData* imgData = ImageUtils::loadImage(fileName);
//	img->setData(imgData->getPixels());
//
//	PyObject* pyImage = PyCapsule_New(actor, "Widget", NULL);
//	return Py_BuildValue("O", pyImage);
//}

SceneEditorModule* sEditor = NULL;

#define BOOST_PYTHON_NO_LIB
#include <boost/python.hpp>
using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omegaToolkit)
{
	PYAPI_ENUM(MenuItem::Type, MenuItemType)
		PYAPI_ENUM_VALUE(MenuItem, Button)
		PYAPI_ENUM_VALUE(MenuItem, Checkbox)
		PYAPI_ENUM_VALUE(MenuItem, Slider)
		;

	PYAPI_REF_BASE_CLASS(MenuManager)
		PYAPI_STATIC_REF_GETTER(MenuManager, createAndInitialize)
		PYAPI_REF_GETTER(MenuManager, createMenu)
		PYAPI_METHOD(MenuManager, setMainMenu)
		;

	PYAPI_REF_BASE_CLASS(Menu)
		PYAPI_REF_GETTER(Menu, addItem)
		;

	PYAPI_REF_BASE_CLASS(MenuItem)
		//PYAPI_REF_GETTER(MenuItem, addItem)
		PYAPI_REF_GETTER(MenuItem, getSubMenu)
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
		;
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