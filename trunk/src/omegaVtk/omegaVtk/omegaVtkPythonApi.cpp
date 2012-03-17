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
#include <vtkPython.h>

#include "omegaVtk/PyVtk.h"
#include "omega/PythonInterpreter.h"
#include "omegaVtk/VtkModule.h"

using namespace omegaVtk;

///////////////////////////////////////////////////////////////////////////////////////////////
class InitializeViewCommand: public IRendererCommand
{
public:
	void execute(Renderer* r)
	{
		VtkModule* vtk = VtkModule::instance();
		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		vtk->beginClientInitialize(r);
		interp->eval("initializeView()");
		vtk->endClientInitialize();
	}
};

InitializeViewCommand* sInitializeViewCommand = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* moduleEnableVtk(PyObject* self, PyObject* args)
{
	// Create and register the omegalib vtk module.
	VtkModule* mod = new VtkModule();
	ModuleServices::addModule(mod);

	// Force module initialization.
	mod->doInitialize(ServerEngine::instance());

	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* queueInitializeView(PyObject* self, PyObject* args)
{
	if(sInitializeViewCommand == NULL) 
	{
		sInitializeViewCommand = new InitializeViewCommand();
	}
	ServerEngine* engine = ServerEngine::instance();
	foreach(Renderer* r, engine->getClients())
	{
		r->queueCommand(sInitializeViewCommand);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* attachProp(PyObject* self, PyObject* args)
{
	PyObject* pyactor = NULL;
	PyObject* pynode = NULL;
	PyArg_ParseTuple(args, "OO", &pyactor, &pynode);

	if(pyactor != NULL && pynode != NULL)
	{
		PyVTKObject* pyvtkactor = (PyVTKObject *)pyactor;
		vtkProp3D* vtkactor = (vtkProp3D*)pyvtkactor->vtk_ptr;
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pynode, "SceneNode");
		Py_INCREF(pyactor);
		VtkModule::instance()->attachProp(vtkactor, node);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//static PyObject* ovtk_addButton(PyObject* self, PyObject* args)
//{
//	const char* buttonName;
//	const char* command;
//	if(!PyArg_ParseTuple(args, "s|s", &buttonName, &command)) return NULL;
//
//	//VtkModule::instance()->getActiveEntity()->addButton(buttonName, command);
//
//	return Py_BuildValue("s", "ok");
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//static PyObject* ovtk_addCheckButton(PyObject* self, PyObject* args)
//{
//	const char* buttonName;
//	const char* getValueCommand;
//	const char* changeValueCommand;
//	if(!PyArg_ParseTuple(args, "s|s|s", &buttonName, &getValueCommand, &changeValueCommand)) return NULL;
//
//	//VtkModule::instance()->getActiveEntity()->addCheckButton(buttonName, getValueCommand, changeValueCommand);
//
//	return Py_BuildValue("s", "ok");
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//static PyObject* ovtk_addSlider(PyObject* self, PyObject* args)
//{
//	const char* sliderName;
//	const char* getValueCommand;
//	const char* changeValueCommand;
//	float minValue;
//	float maxValue;
//	float step;
//	if(!PyArg_ParseTuple(args, "s|f|f|f|s|s", &sliderName, &minValue, &maxValue, &step, &getValueCommand, &changeValueCommand)) return NULL;
//
//	//VtkModule::instance()->getActiveEntity()->addSlider(sliderName, minValue, maxValue, step, getValueCommand, changeValueCommand);
//
//	return Py_BuildValue("s", "ok");
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef ovtkMethods[] = 
{
    {"moduleEnableVtk", moduleEnableVtk, METH_VARARGS, 
		"moduleEnableVtk()\n" 
		"Enables omegalib vtk support."},

    {"attachProp", attachProp, METH_VARARGS, 
		"attachProp(actor, sceneNode)\n"
			"Attaches a vtk 3d prop to an omegalib scene node."},

    {"queueInitializeView", queueInitializeView, METH_VARARGS, 
	"queueInitializeView()\n"
		"queues a call to initializeView(). Must be called after creation of vtk objects to finish vtk scene initialization."},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void OVTK_API omegaVtkPythonApiInit()
{
	omsg("omegaVtkPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->addModule("omegaVtk", ovtkMethods);
}

