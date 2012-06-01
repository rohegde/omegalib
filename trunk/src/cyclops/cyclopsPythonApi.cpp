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
PyObject* entitySetPosition(PyObject* self, PyObject* args)
{
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

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightEnable(PyObject* self, PyObject* args)
{
	int id;
	PyArg_ParseTuple(args, "i", &id);

	Light* l = SceneManager::instance()->getLight(id);
	if(l != NULL) l->enabled = true;
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightDisable(PyObject* self, PyObject* args)
{
	int id;
	PyArg_ParseTuple(args, "i", &id);

	Light* l = SceneManager::instance()->getLight(id);
	if(l != NULL) l->enabled = false;
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetColor(PyObject* self, PyObject* args)
{
	int id;
	const char* colorStr;
	PyArg_ParseTuple(args, "is", &id, &colorStr);

	if(colorStr != NULL)
	{
		Color& col = Color::Color(colorStr);
		Light* l = SceneManager::instance()->getLight(id);
		if(l != NULL)
		{
			l->color = col;
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetAmbient(PyObject* self, PyObject* args)
{
	int id;
	const char* colorStr;
	PyArg_ParseTuple(args, "is", &id, &colorStr);

	if(colorStr != NULL)
	{
		Color& col = Color::Color(colorStr);
		Light* l = SceneManager::instance()->getLight(id);
		if(l != NULL)
		{
			l->ambient = col;
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetShadow(PyObject* self, PyObject* args)
{
	int id;
	float soft;
	int jitter;
	PyArg_ParseTuple(args, "ifi", &id, &soft, &jitter);

	Light* l = SceneManager::instance()->getLight(id);
	if(l != NULL)
	{
		l->softShadowJitter = jitter;
		l->softShadowWidth = soft;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetPosition(PyObject* self, PyObject* args)
{
	int id;
	float x, y, z;
	PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z);

	Light* l = SceneManager::instance()->getLight(id);
	if(l != NULL)
	{
		l->position[0] = x;
		l->position[1] = y;
		l->position[2] = z;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightGetPosition(PyObject* self, PyObject* args)
{
	int id;
	PyArg_ParseTuple(args, "i", &id);

	Light* l = SceneManager::instance()->getLight(id);
	if(l != NULL)
	{
		return Py_BuildValue("fff", l->position[0], l->position[1], l->position[2]);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* animationCount(PyObject* self, PyObject* args)
{
	int id;
	PyArg_ParseTuple(args, "i", &id);

	Entity* e = SceneManager::instance()->findEntity(id);
	if(e != NULL)
	{
		return Py_BuildValue("i", e->getNumAnimations());
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* animationPlay(PyObject* self, PyObject* args)
{
	int id, animationId;
	PyArg_ParseTuple(args, "ii", &id, &animationId);

	Entity* e = SceneManager::instance()->findEntity(id);
	if(e != NULL)
	{
		e->playAnimation(animationId);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* animationLoop(PyObject* self, PyObject* args)
{
	int id, animationId;
	PyArg_ParseTuple(args, "i", &id, &animationId);

	Entity* e = SceneManager::instance()->findEntity(id);
	if(e != NULL)
	{
		e->loopAnimation(animationId);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* animationStop(PyObject* self, PyObject* args)
{
	int id, animationId;
	PyArg_ParseTuple(args, "ii", &id, &animationId);

	Entity* e = SceneManager::instance()->findEntity(id);
	if(e != NULL)
	{
		e->pauseAnimation(animationId);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* animationStopAll(PyObject* self, PyObject* args)
{
	int id;
	PyArg_ParseTuple(args, "i", &id);

	Entity* e = SceneManager::instance()->findEntity(id);
	if(e != NULL)
	{
		e->stopAllAnimations();
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef cyMethods[] = 
{
    {"moduleEnableCyclops", moduleEnableCyclops, METH_VARARGS, 
		"moduleEnableCyclops()\n" 
		"Enables cyclops scene manager support."},

    {"sceneLoad", sceneLoad, METH_VARARGS, 
		"sceneLoad(path)\n" 
		"Loads a cyclops xml scene file."},

    {"lightGetPosition", lightGetPosition, METH_VARARGS, 
		"lightGetPosition(id)\n" 
		"Gets the position for the specified light."},

    {"lightSetPosition", lightSetPosition, METH_VARARGS, 
		"lightSetPosition(id, x, y, z)\n" 
		"Sets the position for the specified light."},

    {"lightSetColor", lightSetColor, METH_VARARGS, 
		"lightSetColor(id, colorString)\n" 
		"Sets the color for the specified light."},

    {"lightSetAmbient", lightSetAmbient, METH_VARARGS, 
		"lightSetAmbient(id, colorString)\n" 
		"Sets the color for the specified light."},

    {"lightSetShadow", lightSetShadow, METH_VARARGS, 
		"lightSetAmbient(id, softnessWidth, softnessJitter)\n" 
		"Sets the shadow parameters for the specified light."},

    {"lightEnable", lightEnable, METH_VARARGS, 
		"lightEnable(id)\n" 
		"Enables the specified light."},

    {"lightDisable", lightDisable, METH_VARARGS, 
		"lightDisable(id)\n" 
		"Disable the specified light."},

	{"animationCount", animationCount, METH_VARARGS, 
		"animationCount(entityId)\n" 
		"Gets the number of animations supported by the specified entity."},

    {"animationPlay", animationPlay, METH_VARARGS, 
		"animationPlay(entityId, animationId)\n" 
		"Plays the specified animation for the specified entity."},

    {"animationLoop", animationLoop, METH_VARARGS, 
		"animationLoop(entityId, animationId)\n" 
		"Loops the specified animation for the specified entity."},

    {"animationStop", animationStop, METH_VARARGS, 
		"animationStop(entityId, animationId)\n" 
		"Stops the specified animation for the specified entity."},

    {"animationStopAll", animationStopAll, METH_VARARGS, 
		"animationStopAll(entityId)\n" 
		"Stops all animations for the specified entity."},

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