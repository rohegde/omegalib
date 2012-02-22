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
#include "omega/EngineServer.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeGetRoot(PyObject* self, PyObject* args)
{
	EngineServer* engine = EngineServer::instance();
	SceneNode* root = engine->getScene(0);
	PyObject* pyRoot = PyCapsule_New(root, "SceneNode", NULL);

	return Py_BuildValue("O", pyRoot);
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
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
		const Vector3f& pos = node->getPosition();
		return Py_BuildValue("(fff)", pos[0], pos[1], pos[2]);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeSetPosition(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	Vector3f pos;
	PyArg_ParseTuple(args, "O(fff)", &pyNode, &pos[0], &pos[1], &pos[2]);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "SceneNode");
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
		return Py_BuildValue("(fff)", pos[0], pos[1], pos[2]);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* nodeSetScale(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	Vector3f pos;
	PyArg_ParseTuple(args, "O(fff)", &pyNode, &pos[0], &pos[1], &pos[2]);

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
    {"nodeGetRoot", nodeGetRoot, METH_VARARGS, "NO INFO"},
    {"nodeGetName", nodeGetName, METH_VARARGS, "NO INFO"},
    {"nodeNumChildren", nodeGetNumChildren, METH_VARARGS, "NO INFO"},
    {"nodeGetParent", nodeGetParent, METH_VARARGS, "NO INFO"},
    {"nodeGetChildByName", nodeGetChildByName, METH_VARARGS, "NO INFO"},
    {"nodeGetChildByIndex", nodeGetChildByIndex, METH_VARARGS, "NO INFO"},
    {"nodeGetPosition", nodeGetPosition, METH_VARARGS, "NO INFO"},
    {"nodeSetPosition", nodeSetPosition, METH_VARARGS, "NO INFO"},
    {"nodeGetScale", nodeGetScale, METH_VARARGS, "NO INFO"},
    {"nodeSetScale", nodeSetScale, METH_VARARGS, "NO INFO"},
    {"nodeResetOrientation", nodeResetOrientation, METH_VARARGS, "NO INFO"},
    {"nodeYaw", nodeYaw, METH_VARARGS, "NO INFO"},
    {"nodePitch", nodePitch, METH_VARARGS, "NO INFO"},
    {"nodeRoll", nodeRoll, METH_VARARGS, "NO INFO"},
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