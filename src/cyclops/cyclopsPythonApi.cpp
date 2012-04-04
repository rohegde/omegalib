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
#include "omega/PythonInterpreter.h"
#include "cyclops/SceneManager.h"
#include "cyclops/SceneLoader.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* moduleEnableCyclops(PyObject* self, PyObject* args)
{
	// Create and register the omegalib vtk module.
	SceneManager* mod = new SceneManager();
	ModuleServices::addModule(mod);

	// Force module initialization.
	mod->doInitialize(ServerEngine::instance());

	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* sceneLoad(PyObject* self, PyObject* args)
{
	const char* mapName;
	if(!PyArg_ParseTuple(args, "s", &mapName)) return NULL;

	SceneManager::instance()->load(mapName);

	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float x, y, z, w;
	PyArg_ParseTuple(args, "ffff", &x, &y, &z, &w);

	SceneManager::instance()->setLightPosition(x, y, z, w);
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* entitySetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	int id;
	float x, y, z;
	PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z);

	Entity* e = SceneManager::instance()->findEntity(id);
	if(e != NULL)
	{
		e->getSceneNode()->setPosition(x, y, z);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef cyMethods[] = 
{
    {"moduleEnableCyclops", moduleEnableCyclops, METH_VARARGS, 
		"moduleEnableVtk()\n" 
		"Enables cyclops scene manager support."},

    {"sceneLoad", sceneLoad, METH_VARARGS, 
		"sceneLoad(path)\n" 
		"Loads a cyclops xml scene file."},

    {"lightSetPosition", lightSetPosition, METH_VARARGS, 
		"sceneLoad(path)\n" 
		"Loads a cyclops xml scene file."},

    {"entitySetPosition", entitySetPosition, METH_VARARGS, 
		"sceneLoad(path)\n" 
		"Loads a cyclops xml scene file."},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
CY_API void cyclopsPythonApiInit()
{
	omsg("cyclopsPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->addModule("cyclops", cyMethods);
}

#endif