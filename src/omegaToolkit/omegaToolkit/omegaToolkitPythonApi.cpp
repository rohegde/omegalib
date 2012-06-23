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
#include "omega/ServerEngine.h"
#include "omegaToolkit/SceneEditorModule.h"
#include "omegaToolkit/UiModule.h"
#include "omegaToolkit/ui/MenuManager.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace omega;
using namespace omegaToolkit;

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

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* addToEditor(PyObject* self, PyObject* args)
{
	if(sEditor == NULL) sEditor = SceneEditorModule::createAndInitialize();
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = dynamic_cast<SceneNode*>((ReferenceType*)PyCapsule_GetPointer(pyNode, "node"));
		if(node != NULL)
		{
			sEditor->addNode(node);
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* removeFromEditor(PyObject* self, PyObject* args)
{
	if(sEditor == NULL) sEditor = SceneEditorModule::createAndInitialize();
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = dynamic_cast<SceneNode*>((ReferenceType*)PyCapsule_GetPointer(pyNode, "node"));
		if(node != NULL)
		{
			sEditor->removeNode(node);
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setEditorEnabled(PyObject* self, PyObject* args)
{
	if(sEditor == NULL) sEditor = SceneEditorModule::createAndInitialize();
	bool enabled = false;
	PyArg_ParseTuple(args, "b", &enabled);

	sEditor->setEnabled(enabled);

	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* newMenu(PyObject* self, PyObject* args)
{
	const char* name = NULL;
	PyArg_ParseTuple(args, "s", &name);

	if(name != NULL)
	{
		ui::MenuManager* mm = ui::MenuManager::instance();

		ui::Menu* menu = mm->createMenu(name);
		return PyCapsule_New(menu, "menu", NULL);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setMainMenu(PyObject* self, PyObject* args)
{
	PyObject* pyMenu = NULL;
	PyArg_ParseTuple(args, "O", &pyMenu);

	if(pyMenu != NULL)
	{
		ui::Menu* menu = dynamic_cast<ui::Menu*>((ReferenceType*)PyCapsule_GetPointer(pyMenu, "menu"));
		menu->hide();
		if(menu != NULL)
		{
			ui::MenuManager::instance()->setMainMenu(menu);
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* addMenuItem(PyObject* self, PyObject* args)
{
	PyObject* pyMenu = NULL;
	ui::MenuItem::Type type;
	PyArg_ParseTuple(args, "Oi", &pyMenu, &type);

	if(pyMenu != NULL)
	{
		ui::Menu* menu = dynamic_cast<ui::Menu*>((ReferenceType*)PyCapsule_GetPointer(pyMenu, "menu"));
		if(menu != NULL)
		{
			ui::MenuItem* mi = menu->addItem(type);
			return PyCapsule_New(mi, "menuItem", NULL);
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setMenuItemText(PyObject* self, PyObject* args)
{
	PyObject* pyMenuItem = NULL;
	const char* text = NULL;
	PyArg_ParseTuple(args, "Os", &pyMenuItem, &text);

	if(pyMenuItem != NULL && text != NULL)
	{
		ui::MenuItem* mi = dynamic_cast<ui::MenuItem*>((ReferenceType*)PyCapsule_GetPointer(pyMenuItem, "menuItem"));
		if(mi != NULL)
		{
			mi->setText(text);
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setMenuItemCommand(PyObject* self, PyObject* args)
{
	PyObject* pyMenuItem = NULL;
	const char* text = NULL;
	PyArg_ParseTuple(args, "Os", &pyMenuItem, &text);

	if(pyMenuItem != NULL && text != NULL)
	{
		ui::MenuItem* mi = dynamic_cast<ui::MenuItem*>((ReferenceType*)PyCapsule_GetPointer(pyMenuItem, "menuItem"));
		if(mi != NULL)
		{
			mi->setCommand(text);
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setMenuItemChecked(PyObject* self, PyObject* args)
{
	PyObject* pyMenuItem = NULL;
	bool checked;
	PyArg_ParseTuple(args, "Ob", &pyMenuItem, &checked);

	if(pyMenuItem != NULL)
	{
		ui::MenuItem* mi = dynamic_cast<ui::MenuItem*>((ReferenceType*)PyCapsule_GetPointer(pyMenuItem, "menuItem"));
		if(mi != NULL)
		{
			mi->setChecked(checked);
			Py_INCREF(Py_None);
			return Py_None;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* isMenuItemChecked(PyObject* self, PyObject* args)
{
	PyObject* pyMenuItem = NULL;
	PyArg_ParseTuple(args, "O", &pyMenuItem);

	if(pyMenuItem != NULL)
	{
		ui::MenuItem* mi = dynamic_cast<ui::MenuItem*>((ReferenceType*)PyCapsule_GetPointer(pyMenuItem, "menuItem"));
		if(mi != NULL)
		{
			return Py_BuildValue("b", mi->isChecked());
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* interactorCreateMouse(PyObject* self, PyObject* args)
//{
//	// Create a mouse interactor and associate it with our scene node.
//	Actor* actor = new DefaultMouseInteractor();
//	ModuleServices::addModule(actor);
//	PyObject* pyActor = PyCapsule_New(actor, "Actor", NULL);
//	return Py_BuildValue("O", pyActor);
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* interactorAttach(PyObject* self, PyObject* args)
//{
//	PyObject* pyInteractor = NULL;
//	PyObject* pyNode = NULL;
//	PyArg_ParseTuple(args, "OO", &pyInteractor, &pyNode);
//
//	Actor* actor = PYCAP_GET(pyInteractor, Actor);
//	SceneNode* node = PYCAP_GET(pyNode, SceneNode);
//
//	if(actor != NULL && node != NULL)
//	{
//		actor->setSceneNode(node);
//	}
//
//    /* Boilerplate to return "None" */
//    Py_INCREF(Py_None);
//    return Py_None;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef omegaToolkitMethods[] = 
{
  //  {"interactorCreateMouse", interactorCreateMouse, METH_VARARGS, 
		//"interactorCreateMouse()\n" 
		//"Creates a mouse interactor."},
	
  //  {"interactorAttach", interactorAttach, METH_VARARGS, 
		//"interactorAttach(interactor, sceneNode)\n" 
		//"no info."},

    {"addToEditor", addToEditor, METH_VARARGS, 
		"addToEditor(node)\n" 
		"Adds the specified node to the scene editor."},

    {"removeToEditor", removeFromEditor, METH_VARARGS, 
		"removeToEditor(node)\n" 
		"Removes the specified node from the scene editor."},

    {"setEditorEnabled", setEditorEnabled, METH_VARARGS, 
		"setEditorEnabled(enabled)\n" 
		"Enables or disables the scene node editor. When enabled the scene node editor allows manipulation of selected nodes through mouse, wand or other input devices"},

    {"newMenu", newMenu, METH_VARARGS, 
		"newMenu(name)\n" 
		"Creates a new menu."},

    {"setMainMenu", setMainMenu, METH_VARARGS, 
		"setMainMenu(menu)\n" 
		"Sets the specified menu as the application main menu."},

    {"addMenuItem", addMenuItem, METH_VARARGS, 
		"addMenuItem(menu, type)\n" 
		"Adds a new menu item to the specified menu. type can be one of the following constants: Button, Checkbox."},

    {"setMenuItemText", setMenuItemText, METH_VARARGS, 
		"setMenuItemText(item, text)\n" 
		"Sets the label for the specified menu item."},

    {"setMenuItemCommand", setMenuItemCommand, METH_VARARGS, 
		"setMenuItemCommand(item, command)\n" 
		"Sets the command for the specified menu item. The command will be executed when the menu item is clicked (Button) or changes state (Checkbox)"},

    {"setMenuItemChecked", setMenuItemChecked, METH_VARARGS, 
		"setMenuItemChecked(item, checked)\n" 
		"Sets the menu item checked state. Valid only for checkbox menu items"},

    {"isMenuItemChecked", isMenuItemChecked, METH_VARARGS, 
		"isMenuItemChecked(item)\n" 
		"Gets the menu item checked state. Valid only for checkbox menu items"},

	{NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void OTK_API omegaToolkitPythonApiInit()
{
	omsg("omegaToolkitPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();

	Dictionary<String, int> intConstants;
	intConstants["CheckboxMenuItem"] = ui::MenuItem::Checkbox;
	intConstants["ButtonMenuItem"] = ui::MenuItem::Button;

	Dictionary<String, String> stringConstants;

	interp->addModule("omegaToolkit", omegaToolkitMethods, intConstants, stringConstants);
}

#endif