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
#include "omega/PythonInterpreterWrapper.h"

using namespace omegaVtk;
using namespace omega;

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
static PyObject* queueInitializeView(PyObject* self, PyObject* args)
{
	if(sInitializeViewCommand == NULL) 
	{
		sInitializeViewCommand = new InitializeViewCommand();
	}
	Engine* engine = Engine::instance();
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef ovtkMethods[] = 
{
    {"attachProp", attachProp, METH_VARARGS, 
		"attachProp(actor, sceneNode)\n"
			"Attaches a vtk 3d prop to an omegalib scene node."},

    {"queueInitializeView", queueInitializeView, METH_VARARGS, 
	"queueInitializeView()\n"
		"queues a call to initializeView(). Must be called after creation of vtk objects to finish vtk scene initialization."},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omegaVtk)
{
	// SceneLoader
	PYAPI_REF_BASE_CLASS(VtkModule)
		PYAPI_STATIC_REF_GETTER(VtkModule, createAndInitialize)
		;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OVTK_API omegaVtkPythonApiInit()
{
	static bool sApiInitialized = false;

	if(!sApiInitialized)
	{
		omsg("omegaVtkPythonApiInit()");
		omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->addModule("omegaVtk", ovtkMethods);
		initomegaVtk();
	}
}

