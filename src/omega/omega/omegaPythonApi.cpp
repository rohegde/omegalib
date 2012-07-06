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
class ScriptNodeListener: public SceneNodeListener
{
public:
	enum Type { VisibleListener, SelectedListener };

	Type type;
	String command;

	virtual void onVisibleChanged(SceneNode* source, bool value) 
	{
		if(type == VisibleListener)
		{
			PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
			if(interp != NULL)
			{
				interp->eval(command);
			}
		}
	}

	virtual void onSelectedChanged(SceneNode* source, bool value) 
	{
		if(type == SelectedListener)
		{
			PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
			if(interp != NULL)
			{
				interp->eval(command);
			}
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* addVisibilityListener(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	const char* cmd;
	PyArg_ParseTuple(args, "Os", &pyNode, &cmd);

	if(pyNode != NULL && cmd != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");

		ScriptNodeListener* listener = new ScriptNodeListener();
		listener->type = ScriptNodeListener::VisibleListener;
		listener->command = cmd;
		node->addListener(listener);

		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* addSelectionListener(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	const char* cmd;
	PyArg_ParseTuple(args, "Os", &pyNode, &cmd);

	if(pyNode != NULL && cmd != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");

		ScriptNodeListener* listener = new ScriptNodeListener();
		listener->type = ScriptNodeListener::SelectedListener;
		listener->command = cmd;
		node->addListener(listener);

		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
static PyMethodDef omegaMethods[] = 
{
    {"addSelectionListener", addSelectionListener, METH_VARARGS, 
		"addSelectionListener(node, cmd)\n"
		"Attaches a command to be executed whenever the node selection state changes."},

    {"addVisibilityListener", addVisibilityListener, METH_VARARGS, 
		"addVisibilityListener(node, cmd)\n"
		"Attaches a command to be executed whenever the node visibility changes."},

  //  {"printChildren", printChildren, METH_VARARGS, 
		//"printChildren(node, maxDepth)\n"
		//"prints the node children tree up to maxDepth."},

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

    {"setUpdateFunction", omegaUpdateCallback, METH_VARARGS, 
		"setUpdateFunction(funcRef)\n"
		"Registers a script function to be called before each frame is rendered"},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
ServerEngine* getServer() { return ServerEngine::instance(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
void printChildrenHelper(Node* n, int depth, const String& prefix, const String& indentString)
{
	if(depth != 0)
	{
		foreach(Node* child, n->getChildren())
		{
			omsg(prefix + child->getName());
			if(child->numChildren() != 0)
			{
				printChildrenHelper(child, depth - 1, prefix + indentString, indentString);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void printChildren(Node* node, int depth)
{
	if(node != NULL)
	{
		printChildrenHelper(node, depth, " ", " ");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//struct Node_to_tuple
//{
//    static PyObject* convert(Node* n)
//      {
//		 return incref(boost::python::object(n).ptr());
//      }
//
//};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector3f_to_tuple
{
    static PyObject* convert(Vector3f const& value)
      {
		 boost::python::tuple vec = boost::python::make_tuple(value[0], value[1], value[2]);
		 return incref(vec.ptr());
      }

};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector3f_from_tuple
{
	Vector3f_from_tuple()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Vector3f>());
	}

	static void* convertible(PyObject* obj)
	{
		if(!PyTuple_Check(obj)) return 0;
		return obj;
	}

	static void construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data)
	{
		float x = extract<float>(PyTuple_GetItem(obj, 0));
		float y = extract<float>(PyTuple_GetItem(obj, 1));
		float z = extract<float>(PyTuple_GetItem(obj, 2));

		void* storage = (
			(converter::rvalue_from_python_storage<Vector3f>*)data)->storage.bytes;
		new (storage) Vector3f(x, y, z);
		data->convertible = storage;
	}
};


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeYawOverloads, yaw, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodePitchOverloads, pitch, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeRollOverloads, roll, 1, 2) 
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omega)
{
	// ServerEngine 
	class_<ServerEngine, boost::noncopyable>("ServerEngine", no_init)
		.def("isConsoleEnabled", &ServerEngine::isConsoleEnabled)
		.def("setConsoleEnabled", &ServerEngine::setConsoleEnabled)
		.def("getScene", &ServerEngine::getScene, PYAPI_RETURN_POINTER)
		.def("getDefaultCamera", &ServerEngine::getDefaultCamera, PYAPI_RETURN_POINTER)
		;

	// Node
	void (Node::*setPosition1)(const Vector3f&) = &Node::setPosition;
	void (Node::*setScale1)(const Vector3f&) = &Node::setScale;
	Node* (Node::*getChildByIndex)(unsigned short) const = &Node::getChild;
	Node* (Node::*getChildByName)(const String&) const = &Node::getChild;
	class_<Node, boost::noncopyable>("Node", no_init)
		.def("getPosition", &Node::getPosition, PYAPI_RETURN_VALUE)
		.def("setPosition", setPosition1)
		.def("getScale", &Node::getScale, PYAPI_RETURN_VALUE)
		.def("setScale", setScale1)
		.def("yaw", &Node::yaw, NodeYawOverloads())
		.def("pitch", &Node::pitch, NodePitchOverloads())
		.def("roll", &Node::roll, NodeRollOverloads())

		.def("numChildren", &Node::numChildren)
		.def("getChildByName", getChildByName, PYAPI_RETURN_POINTER)
		.def("getChildByIndex", getChildByIndex, PYAPI_RETURN_POINTER)


		.def("resetOrientation", &Node::resetOrientation)
		.def("getName", &Node::getName, PYAPI_RETURN_VALUE)
		.def("getParent", &Node::getParent, PYAPI_RETURN_POINTER)
		.def("getChildren", &Node::getChildren, PYAPI_RETURN_POINTER)
		;

	// NodeList
	PYAPI_POINTER_LIST(Node, "NodeList")

	// SceneNode
	class_<SceneNode, bases<Node>, boost::noncopyable >("SceneNode", no_init)
		.def("isVisible", &SceneNode::isVisible)
		.def("setVisible", &SceneNode::setVisible)
		.def("isSelected", &SceneNode::isSelected)
		.def("setSelected", &SceneNode::setSelected)
		.def("isSelectable", &SceneNode::isSelectable)
		.def("setSelectable", &SceneNode::setSelectable)
	;

	// SceneNode
	class_<Camera, boost::noncopyable >("Camera", no_init)
		.def("getPosition", &Camera::getPosition, PYAPI_RETURN_VALUE)
		.def("setPosition", &Camera::setPosition)
		.def("setYawPitchRoll", &Camera::setYawPitchRoll)
		;

	// Color
	class_<Color>("Color", init<String>());

	// Free Functions
	def("getServer", getServer, PYAPI_RETURN_POINTER);
	def("printChildren", &printChildren);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void omegaPythonApiInit()
{
	boost::python::to_python_converter<Vector3f, Vector3f_to_tuple>();
	//boost::python::to_python_converter<Node*, Node_to_tuple>();
	Vector3f_from_tuple();
	
	initomega();

	omsg("omegaPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->addModule("omega", omegaMethods);
}

#endif