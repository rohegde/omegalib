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
#include "omegaToolkit/DefaultMouseInteractor.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace omega;
using namespace omegaToolkit;

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* interactorCreateMouse(PyObject* self, PyObject* args)
{
	// Create a mouse interactor and associate it with our scene node.
	Actor* actor = new DefaultMouseInteractor();
	ModuleServices::addModule(actor);
	PyObject* pyActor = PyCapsule_New(actor, "Actor", NULL);
	return Py_BuildValue("O", pyActor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* interactorAttach(PyObject* self, PyObject* args)
{
	PyObject* pyInteractor = NULL;
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "OO", &pyInteractor, &pyNode);

	Actor* actor = PYCAP_GET(pyInteractor, Actor);
	SceneNode* node = PYCAP_GET(pyNode, SceneNode);

	if(actor != NULL && node != NULL)
	{
		actor->setSceneNode(node);
	}

    /* Boilerplate to return "None" */
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef omegaToolkitMethods[] = 
{
    {"interactorCreateMouse", interactorCreateMouse, METH_VARARGS, 
		"interactorCreateMouse()\n" 
		"Creates a mouse interactor."},
	
    {"interactorAttach", interactorAttach, METH_VARARGS, 
		"interactorAttach(interactor, sceneNode)\n" 
		"no info."},

	{NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void OTK_API omegaToolkitPythonApiInit()
{
	omsg("omegaToolkitPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->addModule("omegaToolkit", omegaToolkitMethods);
}

#endif