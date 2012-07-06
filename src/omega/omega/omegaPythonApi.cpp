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
PyObject* getRoot(PyObject* self, PyObject* args)
{
	ServerEngine* engine = ServerEngine::instance();
	SceneNode* root = engine->getScene();
	PyObject* pyRoot = PyCapsule_New(root, "node", NULL);

	return Py_BuildValue("O", pyRoot);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* newNode(PyObject* self, PyObject* args)
{
	PyObject* pyParent = NULL;
	PyArg_ParseTuple(args, "O", &pyParent);

	SceneNode* parent = PYCAP_GET(pyParent, SceneNode);
	if(parent != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyParent, "node");
		parent->addChild(node);

		PyObject* pyNode = PyCapsule_New(node, "node", NULL);
		return Py_BuildValue("O", pyNode);
	}
    Py_INCREF(Py_None);
    return Py_None;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* getName(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		return Py_BuildValue("s", node->getName().c_str());
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* getChildCount(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		return Py_BuildValue("i", node->numChildren());
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* getParent(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		SceneNode* parent = (SceneNode*)node->getParent();
		PyObject* pyParent = PyCapsule_New(parent, "node", NULL);
		return Py_BuildValue("O", pyParent);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* getChildByName(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	char* nodeName = NULL;
	PyArg_ParseTuple(args, "Os", &pyNode, &nodeName);

	if(pyNode != NULL && nodeName != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		SceneNode* child = (SceneNode*)node->getChild(String(nodeName));
		PyObject* pyChild = PyCapsule_New(child, "node", NULL);
		return Py_BuildValue("O", pyChild);
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* getChildByIndex(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	int index = 0;
	PyArg_ParseTuple(args, "Oi", &pyNode, &index);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		SceneNode* child = (SceneNode*)node->getChild(index);
		PyObject* pyChild = PyCapsule_New(child, "SceneNode", NULL);
		return Py_BuildValue("O", pyChild);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* getPosition(PyObject* self, PyObject* args)
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
PyObject* setPosition(PyObject* self, PyObject* args)
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
PyObject* getScale(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		const Vector3f& pos = node->getScale();
		return Py_BuildValue("fff", pos[0], pos[1], pos[2]);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* setScale(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	Vector3f pos;
	PyArg_ParseTuple(args, "Offf", &pyNode, &pos[0], &pos[1], &pos[2]);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		node->setScale(pos);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* resetOrientation(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		node->setOrientation(Quaternion::Identity());
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* yaw(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float val;
	PyArg_ParseTuple(args, "Of", &pyNode, &val);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		node->yaw(val);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* pitch(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float val;
	PyArg_ParseTuple(args, "Of", &pyNode, &val);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		node->pitch(val);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* roll(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	float val;
	PyArg_ParseTuple(args, "Of", &pyNode, &val);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		node->roll(val);
		Py_INCREF(Py_None);
		return Py_None;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* isVisible(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		return Py_BuildValue("b", node->isVisible());
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PyObject* isSelected(PyObject* self, PyObject* args)
{
	PyObject* pyNode = NULL;
	PyArg_ParseTuple(args, "O", &pyNode);

	if(pyNode != NULL)
	{
		SceneNode* node = (SceneNode*)PyCapsule_GetPointer(pyNode, "node");
		return Py_BuildValue("b", node->isSelected());
	}
	return NULL;
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
    {"newNode", newNode, METH_VARARGS, 
		"newNode(parent)\n"
		"creates a new scene node and sets its parent."},
    
	{"getRoot", getRoot, METH_VARARGS, 
		"getRoot()\n"
		"Return the scene root node"},

    {"getName", getName, METH_VARARGS, 
		"getName(nodeRef)\n"
		"Gets the node name"},

    {"getChildCount", getChildCount, METH_VARARGS, 
		"getChildCount(nodeRef)\n"
		"Gets the node number of children"},

    {"getParent", getParent, METH_VARARGS, 
		"getParent(nodeRef)\n"
		"Gets the node parent"},

    {"getChildByName", getChildByName, METH_VARARGS, 
		"getChildByName(nodeRef, name)\n"
		"Finds a child of the specified node by name"},

    {"getChildByIndex", getChildByIndex, METH_VARARGS, 
		"getChildByIndex(nodeRef, index)\n"
		"Finds a child of the specified node by index"},

    {"getPosition", getPosition, METH_VARARGS, 
		"getPosition(nodeRef)\n"
		"Gets the node position"},

    {"setPosition", setPosition, METH_VARARGS, 
		"setPosition(nodeRef, x, y, z)\n"
		"Sets the node position"},

//#
    {"getScale", getScale, METH_VARARGS, 
		"getScale(nodeRef)\n"
		"Gets the node scale"},

//#
    {"setScale", setScale, METH_VARARGS, 
		"setScale(nodeRef, x, y, z)\n"
		"Sets the node scale"},

//#
    {"resetOrientation", resetOrientation, METH_VARARGS,
		"resetOrientation(nodeRef)\n"
		"Resets the node orientation"},

//#
    {"yaw", yaw, METH_VARARGS, 
		"yaw(nodeRef, yaw)\n"
		"Sets the node yaw"},

//#
    {"pitch", pitch, METH_VARARGS,
		"pitch(nodeRef, pitch)\n"
		"Sets the node position"},

//#
    {"roll", roll, METH_VARARGS, 
		"roll(nodeRef, roll)\n"
		"Sets the node roll"},

    {"isVisible", isVisible, METH_VARARGS, 
		"isVisible(node)\n"
		"Returns wether the node is visible or not"},

    {"isSelected", isSelected, METH_VARARGS, 
		"isSelected(node)\n"
		"Returns wether the node is selected or not"},

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

BOOST_PYTHON_MODULE(omega)
{
	// ServerEngine 
	class_<ServerEngine, boost::noncopyable>("ServerEngine", no_init)
		.def("isConsoleEnabled", &ServerEngine::isConsoleEnabled)
		.def("setConsoleEnabled", &ServerEngine::setConsoleEnabled)
		.def("getScene", &ServerEngine::getScene, PYAPI_RETURN_POINTER)
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
		.def("setScale", setScale)
		.def("yaw", &Node::yaw)
		.def("pitch", &Node::yaw)
		.def("roll", &Node::yaw)

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