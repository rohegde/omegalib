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

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

using namespace omega;

//! Static instance of ScriptRendererCommand, used by rendererQueueCommand
ScriptRendererCommand* sScriptRendererCommand = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* omegaExit(PyObject* self, PyObject* args)
{
	SystemManager::instance()->postExitRequest();
	Py_INCREF(Py_None);
	return Py_None;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* omegaFindFile(PyObject* self, PyObject* args)
{
	const char* name;
	if(!PyArg_ParseTuple(args, "s", &name)) return NULL;

	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo info = dm->getInfo(name);

	if(info.isNull())
	{
		return Py_BuildValue("s", "");
	}

	return Py_BuildValue("s", info.path.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* omegaRun(PyObject* self, PyObject* args)
{
	const char* name;
	if(!PyArg_ParseTuple(args, "s", &name)) return NULL;

	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->runFile(name);

	Py_INCREF(Py_None);
	return Py_None;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* rendererQueueCommand(PyObject* self, PyObject* args)
{
	const char* statement;
	if(!PyArg_ParseTuple(args, "s", &statement)) return NULL;

	if(sScriptRendererCommand == NULL) 
	{
		sScriptRendererCommand = new ScriptRendererCommand();
	}
	sScriptRendererCommand->setStatement(statement);
	ServerEngine* engine = ServerEngine::instance();
	foreach(Renderer* r, engine->getClients())
	{
		r->queueCommand(sScriptRendererCommand);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* omegaUpdateCallback(PyObject *dummy, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O", &temp)) 
	{
        if (!PyCallable_Check(temp)) 
		{
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }

		PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->registerCallback(temp, PythonInterpreter::CallbackUpdate);

        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* cameraGetDefault(PyObject* self, PyObject* args)
{
	ServerEngine* engine = ServerEngine::instance();
	Camera* cam = engine->getDefaultCamera();
	PyObject* pyCam = PyCapsule_New(cam, "Camera", NULL);

	return Py_BuildValue("O", pyCam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* cameraEnableNavigation(PyObject* self, PyObject* args)
{
	PyObject* pyCam = NULL;
	PyArg_ParseTuple(args, "O", &pyCam);

	Camera* cam = PYCAP_GET(pyCam, Camera);
	if(cam != NULL)
	{
		cam->setControllerEnabled(true);
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* cameraDisableNavigation(PyObject* self, PyObject* args)
{
	PyObject* pyCam = NULL;
	PyArg_ParseTuple(args, "O", &pyCam);

	Camera* cam= PYCAP_GET(pyCam, Camera);
	if(cam != NULL)
	{
		cam->setControllerEnabled(false);
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* cameraSetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyCam = NULL;
	float x, y, z;
	PyArg_ParseTuple(args, "Offf", &pyCam, &x, &y, &z);

	Camera* cam= PYCAP_GET(pyCam, Camera);
	if(cam != NULL)
	{
		cam->setPosition(Vector3f(x, y, z));
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* cameraGetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyCam = NULL;
	PyArg_ParseTuple(args, "O");

	Camera* cam= PYCAP_GET(pyCam, Camera);
	if(cam != NULL)
	{
		const Vector3f& pos = cam->getPosition();
		return Py_BuildValue("fff", pos[0], pos[1], pos[2]);
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* cameraSetOrientation(PyObject* self, PyObject* args)
{
	PyObject* pyCam = NULL;
	float yaw, pitch, roll;
	PyArg_ParseTuple(args, "Offf", &pyCam, &yaw, &pitch, &roll);

	Camera* cam= PYCAP_GET(pyCam, Camera);
	if(cam != NULL)
	{
		cam->setOrientation(Vector3f(yaw, pitch, roll));
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetRoot(PyObject* self, PyObject* args)
{
	ServerEngine* engine = ServerEngine::instance();
	SceneNode* root = engine->getScene();
	PyObject* pyRoot = PyCapsule_New(root, "SceneNode", NULL);

	return Py_BuildValue("O", pyRoot);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeCreate(PyObject* self, PyObject* args)
{
	PyObject* pyParent = NULL;
	PyArg_ParseTuple(args, "O", &pyParent);

	SceneNode* parent = PYCAP_GET(pyParent, SceneNode);
	if(parent != NULL)
	{
		SceneNode* node = new SceneNode(ServerEngine::instance());
		parent->addChild(node);

		PyObject* pyNode = PyCapsule_New(node, "SceneNode", NULL);
		return Py_BuildValue("O", pyNode);
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetName(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		return Py_BuildValue("s", node->getName().c_str());
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetNumChildren(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		return Py_BuildValue("i", node->numChildren());
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetParent(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		SceneNode* parent = (SceneNode*)node->getParent();
		PyObject* pyParent = PyCapsule_New(parent, "SceneNode", NULL);
		return Py_BuildValue("O", pyParent);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetChildByName(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	char* nodeName = NULL;
	PyArg_ParseTuple(args, "Os", &pyNode, &nodeName);

	if(pyNode != NULL && nodeName != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		SceneNode* child = (SceneNode*)node->getChild(String(nodeName));
		PyObject* pyChild = PyCapsule_New(child, "SceneNode", NULL);
		return Py_BuildValue("O", pyChild);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetChildByIndex(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	int index = 0;
	PyArg_ParseTuple(args, "Oi", &pyNode, &index);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		SceneNode* child = (SceneNode*)node->getChild(index);
		PyObject* pyChild = PyCapsule_New(child, "SceneNode", NULL);
		return Py_BuildValue("O", pyChild);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		const Vector3f& pos = node->getPosition();
		return Py_BuildValue("fff", pos[0], pos[1], pos[2]);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeSetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	Vector3f pos;
	PyArg_ParseTuple(args, "Offf", &pyNode, &pos[0], &pos[1], &pos[2]);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		node->setPosition(pos);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetScale(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		const Vector3f& pos = node->getScale();
		return Py_BuildValue("fff", pos[0], pos[1], pos[2]);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeSetScale(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	Vector3f pos;
	PyArg_ParseTuple(args, "Offf", &pyNode, &pos[0], &pos[1], &pos[2]);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		node->setScale(pos);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeResetOrientation(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		node->setOrientation(Quaternion::Identity());
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeYaw(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float val;
	PyArg_ParseTuple(args, "Of", &pyNode, &val);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		node->yaw(val);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodePitch(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float val;
	PyArg_ParseTuple(args, "Of", &pyNode, &val);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		node->pitch(val);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeRoll(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float val;
	PyArg_ParseTuple(args, "Of", &pyNode, &val);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		node->roll(val);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef omegaMethods[] = 
{
	// Camera API
    {"cameraGetDefault", cameraGetDefault, METH_VARARGS,
		"cameraGetDefault()\n"
		"Returns a reference to the main camera."},

    {"cameraSetPosition", cameraSetPosition, METH_VARARGS,
		"cameraSetPosition(camerRef, x, y, z)\n"
		"Sets the camera position."},

    {"cameraGetPosition", cameraGetPosition, METH_VARARGS,
		"cameraGetPosition(cameraRef)\n"
		"Gets the camera position."},

    {"cameraSetOrientation", cameraSetOrientation, METH_VARARGS, 
		"cameraSetOrientation(cameraRef, yaw, pitch, roll)\n"
		"Sets the camera orientation."},

    {"cameraEnableNavigation", cameraEnableNavigation, METH_VARARGS,
		"cameraEnableNavigation(cameraRef)\n"
		"Enables camera navigation for the specified camera."},

    {"cameraDisableNavigation", cameraDisableNavigation, METH_VARARGS,
		"cameraDisableNavigation(cameraRef)\n"
		"Disables camera navigation for the specified camera."},

	// Node API
    {"nodeCreate", nodeCreate, METH_VARARGS, 
		"nodeCreate(parent)\n"
		"creates a new scene node and sets its parent."},
    
	{"nodeGetRoot", nodeGetRoot, METH_VARARGS, 
		"nodeGetRoot()\n"
		"Return the scene root node"},

    {"nodeGetName", nodeGetName, METH_VARARGS, 
		"nodeGetName(nodeRef)\n"
		"Gets the node name"},

    {"nodeNumChildren", nodeGetNumChildren, METH_VARARGS, 
		"nodeNumChildren(nodeRef)\n"
		"Gets the node number of children"},

    {"nodeGetParent", nodeGetParent, METH_VARARGS, 
		"nodeGetParent(nodeRef)\n"
		"Gets the node parent"},

    {"nodeGetChildByName", nodeGetChildByName, METH_VARARGS, 
		"nodeGetChildByName(nodeRef, name)\n"
		"Finds a child of the specified node by name"},

    {"nodeGetChildByIndex", nodeGetChildByIndex, METH_VARARGS, 
		"nodeGetChildByIndex(nodeRef, index)\n"
		"Finds a child of the specified node by index"},

    {"nodeGetPosition", nodeGetPosition, METH_VARARGS, 
		"nodeGetPosition(nodeRef)\n"
		"Gets the node position"},

    {"nodeSetPosition", nodeSetPosition, METH_VARARGS, 
		"nodeSetPosition(nodeRef, x, y, z)\n"
		"Sets the node position"},

    {"nodeGetScale", nodeGetScale, METH_VARARGS, 
		"nodeGetScale(nodeRef)\n"
		"Gets the node scale"},

    {"nodeSetScale", nodeSetScale, METH_VARARGS, 
		"nodeSetScale(nodeRef, x, y, z)\n"
		"Sets the node scale"},

    {"nodeResetOrientation", nodeResetOrientation, METH_VARARGS,
		"nodeResetOrientation(nodeRef)\n"
		"Resets the node orientation"},

    {"nodeYaw", nodeYaw, METH_VARARGS, 
		"nodeYaw(nodeRef, yaw)\n"
		"Sets the node yaw"},

    {"nodePitch", nodePitch, METH_VARARGS,
		"nodePitch(nodeRef, pitch)\n"
		"Sets the node position"},

    {"nodeRoll", nodeRoll, METH_VARARGS, 
		"nodeRoll(nodeRef, roll)\n"
		"Sets the node roll"},

	// Renderer API
    {"rendererQueueCommand", rendererQueueCommand, METH_VARARGS, 
		"rendererQueueCommand(funcRef)\n"
		"Queues a script fuction to be executed once on all running rendering threads"},

	// Base omegalib API
	{"oexit", omegaExit, METH_VARARGS, 
		"oexit()\n"
		"Terminates the current omegalib program"},

    {"ofindFile", omegaFindFile, METH_VARARGS, 
		"ofindFile(name)\n"
		"Searches for a file in the application data filesystems and returns a full path if found"},

    {"orun", omegaRun, METH_VARARGS, 
		"ofindFile(fileName)\n"
		"Runs an external script"},

    {"ofuncUpdate", omegaUpdateCallback, METH_VARARGS, 
		"ofuncUpdate(funcRef)\n"
		"Registers a script function to be called before each frame is rendered"},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void omegaPythonApiInit()
{
	omsg("omegaPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->addModule("omega", omegaMethods);
}

#endif