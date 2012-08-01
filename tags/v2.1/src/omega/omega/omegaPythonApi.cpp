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

PyObject* sEuclidModule = NULL;

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
void printObjCounts()
{
	ReferenceType::printObjCounts();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector3f_to_python
{
	static PyObject* convert(Vector3f const& value)
	{
		// If we haven't looked up for the Vector3 class, let's do it now.
		static PyObject* sVector3Class = NULL;
		if(sVector3Class == NULL)
		{
			PyObject* moduleDict = PyModule_GetDict(sEuclidModule);
			sVector3Class = PyDict_GetItemString(moduleDict, "Vector3");
		}

		// Create a new euclid.Vector3 instance using the omega::Vector3f components
		// as arguments.
		boost::python::tuple vec = boost::python::make_tuple(value[0], value[1], value[2]);
		PyObject* vector3obj = PyObject_CallObject(sVector3Class, vec.ptr());
		return incref(vector3obj);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector3f_from_python
{
	Vector3f_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Vector3f>());
	}

	static void* convertible(PyObject* obj)
	{
		// We don't really care if the object is of type Vector3. We just require
		// it to have x, y, z attributes.
		if(!PyObject_HasAttrString(obj, "x") ||
			!PyObject_HasAttrString(obj, "y") ||
			!PyObject_HasAttrString(obj, "z")) return 0;
		return obj;
	}

	static void construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data)
	{
		float x = extract<float>(PyObject_GetAttrString(obj, "x"));
		float y = extract<float>(PyObject_GetAttrString(obj, "y"));
		float z = extract<float>(PyObject_GetAttrString(obj, "z"));

		void* storage = (
			(converter::rvalue_from_python_storage<Vector3f>*)data)->storage.bytes;
		new (storage) Vector3f(x, y, z);
		data->convertible = storage;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Quaternion_to_python
{
	static PyObject* convert(Quaternion const& value)
	{
		// If we haven't looked up for the Quaternion class, let's do it now.
		static PyObject* sQuaternionClass = NULL;
		if(sQuaternionClass == NULL)
		{
			PyObject* moduleDict = PyModule_GetDict(sEuclidModule);
			sQuaternionClass = PyDict_GetItemString(moduleDict, "Quaternion");
		}

		// Create a new euclid.Quaternion instance using the omega::Quaternion components
		// as arguments.
		boost::python::tuple vec = boost::python::make_tuple(value.x(), value.y(), value.z(), value.w());
		PyObject* quatobj = PyObject_CallObject(sQuaternionClass, vec.ptr());
		return incref(quatobj);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Quaternion_from_python
{
	Quaternion_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Quaternion>());
	}

	static void* convertible(PyObject* obj)
	{
		// We don't really care if the object is of type Quaternion. We just require
		// it to have x, y, z attributes.
		if(!PyObject_HasAttrString(obj, "x") ||
			!PyObject_HasAttrString(obj, "y") ||
			!PyObject_HasAttrString(obj, "z") ||
			!PyObject_HasAttrString(obj, "w")) return 0;
		return obj;
	}

	static void construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data)
	{
		float x = extract<float>(PyObject_GetAttrString(obj, "x"));
		float y = extract<float>(PyObject_GetAttrString(obj, "y"));
		float z = extract<float>(PyObject_GetAttrString(obj, "z"));
		float w = extract<float>(PyObject_GetAttrString(obj, "w"));

		void* storage = (
			(converter::rvalue_from_python_storage<Quaternion>*)data)->storage.bytes;
		new (storage) Quaternion(x, y, z, w);
		data->convertible = storage;
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeYawOverloads, yaw, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodePitchOverloads, pitch, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeRollOverloads, roll, 1, 2) 
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omega)
{
	// Event type
	enum_<EventBase::Type>("EventType")
			PYAPI_ENUM_VALUE(EventBase, Select)
			PYAPI_ENUM_VALUE(EventBase,Toggle)
			PYAPI_ENUM_VALUE(EventBase,ChangeValue)
			PYAPI_ENUM_VALUE(EventBase,Update)
			PYAPI_ENUM_VALUE(EventBase,Move) 
			PYAPI_ENUM_VALUE(EventBase,Down)
			PYAPI_ENUM_VALUE(EventBase,Up)
			PYAPI_ENUM_VALUE(EventBase,Trace)
			PYAPI_ENUM_VALUE(EventBase,Connect)
			PYAPI_ENUM_VALUE(EventBase,Untrace)
			PYAPI_ENUM_VALUE(EventBase,Disconnect)
			PYAPI_ENUM_VALUE(EventBase,Click)
			PYAPI_ENUM_VALUE(EventBase,DoubleClick)
			PYAPI_ENUM_VALUE(EventBase,MoveLeft)
			PYAPI_ENUM_VALUE(EventBase,MoveRight)
			PYAPI_ENUM_VALUE(EventBase,MoveUp)
			PYAPI_ENUM_VALUE(EventBase,MoveDown)
			PYAPI_ENUM_VALUE(EventBase,Zoom)
			PYAPI_ENUM_VALUE(EventBase,SplitStart)
			PYAPI_ENUM_VALUE(EventBase,SplitEnd)
			PYAPI_ENUM_VALUE(EventBase,Split)
			PYAPI_ENUM_VALUE(EventBase,RotateStart)
			PYAPI_ENUM_VALUE(EventBase,RotateEnd)
			PYAPI_ENUM_VALUE(EventBase,Rotate)
			PYAPI_ENUM_VALUE(EventBase,Null)
		;

	// Event Flags
	enum_<EventBase::Flags>("EventFlags")
			PYAPI_ENUM_VALUE(EventBase, Left)
			PYAPI_ENUM_VALUE(EventBase,Button1)
			PYAPI_ENUM_VALUE(EventBase,Right)
			PYAPI_ENUM_VALUE(EventBase,Button2)
			PYAPI_ENUM_VALUE(EventBase,Middle)
			PYAPI_ENUM_VALUE(EventBase,Button3)
			PYAPI_ENUM_VALUE(EventBase,Ctrl)
			PYAPI_ENUM_VALUE(EventBase,SpecialButton1)
			PYAPI_ENUM_VALUE(EventBase,Alt)
			PYAPI_ENUM_VALUE(EventBase,SpecialButton2)
			PYAPI_ENUM_VALUE(EventBase,Shift)
			PYAPI_ENUM_VALUE(EventBase,SpecialButton3)
			PYAPI_ENUM_VALUE(EventBase,Button4)
			PYAPI_ENUM_VALUE(EventBase,Button5)
			PYAPI_ENUM_VALUE(EventBase,Button6)
			PYAPI_ENUM_VALUE(EventBase,Button7)
			PYAPI_ENUM_VALUE(EventBase,ButtonUp)
			PYAPI_ENUM_VALUE(EventBase,ButtonDown)
			PYAPI_ENUM_VALUE(EventBase,ButtonLeft)
			PYAPI_ENUM_VALUE(EventBase,ButtonRight)
			PYAPI_ENUM_VALUE(EventBase,Processed)
			PYAPI_ENUM_VALUE(EventBase,User)
		;
	
	// Event Extra Data Type
	enum_<EventBase::ExtraDataType>("EventExtraDataType")
			PYAPI_ENUM_VALUE(EventBase,ExtraDataNull)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataFloatArray)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataIntArray)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataVector3Array)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataString)
		;

	// Event Extra Data Type
	enum_<EventBase::ServiceType>("ServiceType")
			PYAPI_ENUM_VALUE(EventBase,ServiceTypePointer)
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeMocap)
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeKeyboard) 
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeController)
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeUi) 
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeGeneric)
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeBrain)
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeWand) 
			PYAPI_ENUM_VALUE(EventBase,ServiceTypeAudio)
			;

	// Event
	const Vector3f& (Event::*getPosition1)() const = &Event::getPosition;
	class_<Event, boost::noncopyable>("Event", no_init)
		.def("getSourceId", &Event::getSourceId)
		.def("getType", &Event::getType)
		.def("getPosition", getPosition1, PYAPI_RETURN_VALUE)
		.def("getOrientation", &Event::getOrientation, PYAPI_RETURN_VALUE)
		.def("getServiceType", &Event::getServiceType)
		;

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
	void (Node::*setOrientation1)(const Quaternion&) = &Node::setOrientation;
	Node* (Node::*getChildByIndex)(unsigned short) const = &Node::getChild;
	Node* (Node::*getChildByName)(const String&) const = &Node::getChild;
	class_<Node, boost::noncopyable>("Node", no_init)
		.def("getPosition", &Node::getPosition, PYAPI_RETURN_VALUE)
		.def("setPosition", setPosition1)
		.def("getScale", &Node::getScale, PYAPI_RETURN_VALUE)
		.def("setScale", setScale1)
		.def("setOrientation", setOrientation1)
		.def("getOrientation", &Node::getOrientation, PYAPI_RETURN_VALUE)
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
	def("printObjCounts", &printObjCounts);
};

// Black magic. Include the pyeuclid source code (saved as hex file using xdd -i)
char euclid_source[] = { 
	#include "euclid.xdd" 
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void omegaPythonApiInit()
{
	omsg("omegaPythonApiInit()");
	omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();

	// Compile, load and import the euclid module.
	PyObject* euclidModuleCode = Py_CompileString(euclid_source, "euclid", Py_file_input);
	if(euclidModuleCode != NULL)
	{
		sEuclidModule = PyImport_ExecCodeModule("euclid", euclidModuleCode);
		interp->eval("from euclid import *");
	}

	// Register omega::Vector3f <-> euclid.Vector3 converters
	boost::python::to_python_converter<Vector3f, Vector3f_to_python>();
	Vector3f_from_python();

	// Register omega::Quaternion <-> euclid.Quaternion converters
	boost::python::to_python_converter<Quaternion, Quaternion_to_python>();
	Quaternion_from_python();

	// Initialize the omega wrapper module
	initomega();

	interp->addModule("omega", omegaMethods);
}

#endif
