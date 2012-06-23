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
#include "cyclops/Entity.h"
#include "cyclops/SceneLoader.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
/*static PyObject* moduleEnableCyclops(PyObject* self, PyObject* args)
{
	SceneManager::createAndInitialize();

	Py_INCREF(Py_None);
	return Py_None;
}*/

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
PyObject* setLightEnabled(PyObject* self, PyObject* args)
{
	bool enabled = false;
	PyObject* pyLight = NULL;
	PyArg_ParseTuple(args, "Ob", &pyLight, &enabled);

	if(pyLight != NULL)
	{
		Light* l = dynamic_cast<Light*>((ReferenceType*)PyCapsule_GetPointer(pyLight, "node"));
		if(l != NULL)
		{
			l->setEnabled(enabled);
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetColor(PyObject* self, PyObject* args)
{
	const char* color = NULL;
	PyObject* pyLight = NULL;
	PyArg_ParseTuple(args, "Os", &pyLight, &color);

	if(pyLight != NULL && color != NULL)
	{
		Light* l = dynamic_cast<Light*>((ReferenceType*)PyCapsule_GetPointer(pyLight, "node"));
		if(l != NULL)
		{
			l->setColor(Color(color));
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* lightSetAmbient(PyObject* self, PyObject* args)
{
	const char* color = NULL;
	PyObject* pyLight = NULL;
	PyArg_ParseTuple(args, "Os", &pyLight, &color);

	if(pyLight != NULL && color != NULL)
	{
		Light* l = dynamic_cast<Light*>((ReferenceType*)PyCapsule_GetPointer(pyLight, "node"));
		if(l != NULL)
		{
			l->setAmbient(Color(color));
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setMainLight(PyObject* self, PyObject* args)
{
	PyObject* pyLight = NULL;
	PyArg_ParseTuple(args, "O", &pyLight);

	if(pyLight != NULL)
	{
		Light* l = dynamic_cast<Light*>((ReferenceType*)PyCapsule_GetPointer(pyLight, "node"));
		if(l != NULL)
		{
			SceneManager::instance()->setMainLight(l);
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* lightSetShadow(PyObject* self, PyObject* args)
//{
//	int id;
//	float soft;
//	int jitter;
//	PyArg_ParseTuple(args, "ifi", &id, &soft, &jitter);
//
//	Light* l = SceneManager::instance()->getLight(id);
//	if(l != NULL)
//	{
//		l->setSoftShadowJitter(jitter);
//		l->setSoftShadowWidth(soft);
//	}
//	Py_INCREF(Py_None);
//	return Py_None;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* animationCount(PyObject* self, PyObject* args)
//{
//	int id;
//	PyArg_ParseTuple(args, "i", &id);
//
//	Entity* e = SceneManager::instance()->findEntity(id);
//	if(e != NULL)
//	{
//		return Py_BuildValue("i", e->getNumAnimations());
//	}
//	return NULL;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* animationPlay(PyObject* self, PyObject* args)
//{
//	int id, animationId;
//	PyArg_ParseTuple(args, "ii", &id, &animationId);
//
//	Entity* e = SceneManager::instance()->findEntity(id);
//	if(e != NULL)
//	{
//		e->playAnimation(animationId);
//		Py_INCREF(Py_None);
//		return Py_None;
//	}
//	return NULL;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* animationLoop(PyObject* self, PyObject* args)
//{
//	int id, animationId;
//	PyArg_ParseTuple(args, "i", &id, &animationId);
//
//	Entity* e = SceneManager::instance()->findEntity(id);
//	if(e != NULL)
//	{
//		e->loopAnimation(animationId);
//		Py_INCREF(Py_None);
//		return Py_None;
//	}
//	return NULL;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* animationStop(PyObject* self, PyObject* args)
//{
//	int id, animationId;
//	PyArg_ParseTuple(args, "ii", &id, &animationId);
//
//	Entity* e = SceneManager::instance()->findEntity(id);
//	if(e != NULL)
//	{
//		e->pauseAnimation(animationId);
//		Py_INCREF(Py_None);
//		return Py_None;
//	}
//	return NULL;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//PyObject* animationStopAll(PyObject* self, PyObject* args)
//{
//	int id;
//	PyArg_ParseTuple(args, "i", &id);
//
//	Entity* e = SceneManager::instance()->findEntity(id);
//	if(e != NULL)
//	{
//		e->stopAllAnimations();
//		Py_INCREF(Py_None);
//		return Py_None;
//	}
//	return NULL;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* loadModel(PyObject* self, PyObject* args)
{
	const char* name;
	const char* path;
	float size = 0;
	PyArg_ParseTuple(args, "ssf", &name, &path, &size);

	if(name != NULL && path != NULL)
	{
		SceneManager::instance()->loadModel(ModelInfo(name, path, size));
	}
	Py_INCREF(Py_None);
	return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* newStaticObject(PyObject* self, PyObject* args)
{
	const char* modelName;
	PyArg_ParseTuple(args, "s", &modelName);

	if(modelName != NULL)
	{
		StaticObject* obj = new StaticObject(SceneManager::instance(), modelName);
		if(obj != NULL)
		{
			return PyCapsule_New(obj, "node", NULL);
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* newPlaneShape(PyObject* self, PyObject* args)
{
	float width = 1;
	float height = 1;
	float tilingX = 1;
	float tilingY = 1;
	PyArg_ParseTuple(args, "ffff", &width, &height, &tilingX, &tilingY);

	PlaneShape* obj = new PlaneShape(SceneManager::instance(), width, height, Vector2f(tilingX, tilingY));
	if(obj != NULL)
	{
		return PyCapsule_New(obj, "node", NULL);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* newLight(PyObject* self, PyObject* args)
{
	Light* l = new Light(SceneManager::instance());
	return PyCapsule_New(l, "node", NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setEffect(PyObject* self, PyObject* args)
{
	const char* fxDef = NULL;
	PyObject* pyDrawable = NULL;
	PyArg_ParseTuple(args, "Os", &pyDrawable, &fxDef);

	if(pyDrawable != NULL && fxDef != NULL)
	{
		DrawableObject* obj = dynamic_cast<DrawableObject*>((ReferenceType*)PyCapsule_GetPointer(pyDrawable, "node"));
		if(obj != NULL)
		{
			obj->setEffect(fxDef);
		}
    Py_INCREF(Py_None);
    return Py_None;
	}
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef cyMethods[] = 
{
  //  {"moduleEnableCyclops", moduleEnableCyclops, METH_VARARGS, 
		//"moduleEnableCyclops()\n" 
		//"Enables cyclops scene manager support."},

    {"loadModel", loadModel, METH_VARARGS, 
		"loadModel(name, path, size)\n" 
		"Loads mesh model."},

    {"newStaticObject", newStaticObject, METH_VARARGS, 
		"newStaticObject(modelName)\n" 
		"Creates a new static object using the specified mesh as a model."},

    {"newPlaneShape", newPlaneShape, METH_VARARGS, 
		"newPlaneShape(width, height, tilingX, tilingY)\n" 
		"Creates a new plane with the specified width and height, and with texturing coordinates repeating (tilingX, tilingY) times."},

    {"newLight", newLight, METH_VARARGS, 
		"newLight()\n" 
		"Creates a new light."},

    {"setEffect", setEffect, METH_VARARGS, 
		"setEffect(obj, effectDefinition)\n" 
		"Applies the specified effect to the passed drawable object."},

    {"sceneLoad", sceneLoad, METH_VARARGS, 
		"sceneLoad(path)\n" 
		"Loads a cyclops xml scene file."},

    {"setLightColor", lightSetColor, METH_VARARGS, 
		"setLightColor(light, colorString)\n" 
		"Sets the color for the specified light."},

    {"setLightAmbient", lightSetAmbient, METH_VARARGS, 
		"setLightAmbient(light, colorString)\n" 
		"Sets the color for the specified light."},

  //  {"lightSetShadow", lightSetShadow, METH_VARARGS, 
		//"lightSetAmbient(id, softnessWidth, softnessJitter)\n" 
		//"Sets the shadow parameters for the specified light."},

    {"setMainLight", setMainLight, METH_VARARGS, 
		"setMainLight(light)\n" 
		"Sets the specified light as the main scene light."},

    {"setLightEnabled", setLightEnabled, METH_VARARGS, 
		"setLightEnabled(light, enabled)\n" 
		"Enables or disables the specified light."},

	//{"animationCount", animationCount, METH_VARARGS, 
	//	"animationCount(entityId)\n" 
	//	"Gets the number of animations supported by the specified entity."},

 //   {"animationPlay", animationPlay, METH_VARARGS, 
	//	"animationPlay(entityId, animationId)\n" 
	//	"Plays the specified animation for the specified entity."},

 //   {"animationLoop", animationLoop, METH_VARARGS, 
	//	"animationLoop(entityId, animationId)\n" 
	//	"Loops the specified animation for the specified entity."},

 //   {"animationStop", animationStop, METH_VARARGS, 
	//	"animationStop(entityId, animationId)\n" 
	//	"Stops the specified animation for the specified entity."},

 //   {"animationStopAll", animationStopAll, METH_VARARGS, 
	//	"animationStopAll(entityId)\n" 
	//	"Stops all animations for the specified entity."},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
CY_API void cyclopsPythonApiInit()
{
	static bool sApiInitialized = false;

	if(!sApiInitialized)
	{
		sApiInitialized = true;
		omsg("cyclopsPythonApiInit()");
		omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->addModule("cyclops", cyMethods);
	}
}

#endif