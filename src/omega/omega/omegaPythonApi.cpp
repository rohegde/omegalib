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
#include "omega/DisplaySystem.h"
#include "omega/EqualizerDisplaySystem.h"
#include "omega/Engine.h"
#include "omega/Actor.h"
#include "omega/PortholeService.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

#include <boost/mpl/if.hpp>

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

using namespace omega;

//! Static instance of ScriptRendererCommand, used by rendererQueueCommand
//ScriptRendererCommand* sScriptRendererCommand = NULL;

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
//static PyObject* rendererQueueCommand(PyObject* self, PyObject* args)
//{
//	const char* statement;
//	if(!PyArg_ParseTuple(args, "s", &statement)) return NULL;
//
//	if(sScriptRendererCommand == NULL) 
//	{
//		sScriptRendererCommand = new ScriptRendererCommand();
//	}
//	sScriptRendererCommand->setStatement(statement);
//	Engine* engine = Engine::instance();
//	foreach(Renderer* r, engine->getClients())
//	{
//		r->queueCommand(sScriptRendererCommand);
//	}
//	Py_INCREF(Py_None);
//	return Py_None;
//}

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

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* omegaEventCallback(PyObject *dummy, PyObject *args)
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
		interp->registerCallback(temp, PythonInterpreter::CallbackEvent);

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

	//// Renderer API
 //   {"rendererQueueCommand", rendererQueueCommand, METH_VARARGS, 
	//	"rendererQueueCommand(funcRef)\n"
	//	"Queues a script fuction to be executed once on all running rendering threads"},

	// Base omegalib API
	{"oexit", omegaExit, METH_VARARGS, 
		"oexit()\n"
		"Terminates the current omegalib program"},

    {"ofindFile", omegaFindFile, METH_VARARGS, 
		"ofindFile(name)\n"
		"Searches for a file in the application data filesystems and returns a full path if found"},

    {"orun", omegaRun, METH_VARARGS, 
		"orun(fileName)\n"
		"Runs an external script"},

    {"setUpdateFunction", omegaUpdateCallback, METH_VARARGS, 
		"setUpdateFunction(funcRef)\n"
		"Registers a script function to be called before each frame is rendered"},

    {"setEventFunction", omegaEventCallback, METH_VARARGS, 
		"setEventFunction(funcRef)\n"
		"Registers a script function to be called when events are received"},

    {NULL, NULL, 0, NULL}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
Engine* getEngine() { return Engine::instance(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
const Event* getEvent() { return PythonInterpreter::getLastEvent(); }

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
bool settingExists(const String& settingName)
{
	return SystemManager::settingExists(settingName);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const Setting& settingLookup(const String& settingName)
{
	return SystemManager::settingLookup(settingName);
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
struct Vector2f_to_python
{
	static PyObject* convert(Vector2f const& value)
	{
		// If we haven't looked up for the Vector3 class, let's do it now.
		static PyObject* sVector2Class = NULL;
		if(sVector2Class == NULL)
		{
			PyObject* moduleDict = PyModule_GetDict(sEuclidModule);
			sVector2Class = PyDict_GetItemString(moduleDict, "Vector2");
		}

		// Create a new euclid.Vector3 instance using the omega::Vector3f components
		// as arguments.
		boost::python::tuple vec = boost::python::make_tuple(value[0], value[1]);
		PyObject* vector2obj = PyObject_CallObject(sVector2Class, vec.ptr());
		return incref(vector2obj);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector2f_from_python
{
	Vector2f_from_python()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Vector2f>());
	}

	static void* convertible(PyObject* obj)
	{
		// We don't really care if the object is of type Vector3. We just require
		// it to have x, y attributes.
		if(!PyObject_HasAttrString(obj, "x") ||
			!PyObject_HasAttrString(obj, "y")) return 0;
		return obj;
	}

	static void construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data)
	{
		float x = extract<float>(PyObject_GetAttrString(obj, "x"));
		float y = extract<float>(PyObject_GetAttrString(obj, "y"));

		void* storage = (
			(converter::rvalue_from_python_storage<Vector2f>*)data)->storage.bytes;
		new (storage) Vector2f(x, y);
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void querySceneRay(const Vector3f& origin, const Vector3f& dir, boost::python::object callback)
{
	const SceneQueryResultList& sqrl = Engine::instance()->querySceneRay(Ray(origin, dir));
    boost::python::list l;
	foreach(SceneQueryResult sqr, sqrl)
	{
		callback(boost::python::ptr(sqr.node), sqr.distance);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
boost::python::tuple getRayFromEvent(const Event* evt)
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	Ray r;
	bool res = ds->getViewRayFromEvent(*evt, r);
	return boost::python::make_tuple(res, r.getOrigin(), r.getDirection());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* getDefaultCamera()
{
	return Engine::instance()->getDefaultCamera();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* getCameraById(int id)
{
	return PortholeGUI::CamerasMap[id]->camera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* getScene()
{
	return Engine::instance()->getScene();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void toggleStats(const String& stats)
{
	EqualizerDisplaySystem* eqds = dynamic_cast<EqualizerDisplaySystem*>(SystemManager::instance()->getDisplaySystem());
	eqds->toggleStats(stats);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeYawOverloads, yaw, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodePitchOverloads, pitch, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeRollOverloads, roll, 1, 2) 
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omega)
{
	// Event type
	PYAPI_ENUM(Event::Type, EventType)
			PYAPI_ENUM_VALUE(Event, Select)
			PYAPI_ENUM_VALUE(Event,Toggle)
			PYAPI_ENUM_VALUE(Event,ChangeValue)
			PYAPI_ENUM_VALUE(Event,Update)
			PYAPI_ENUM_VALUE(Event,Move) 
			PYAPI_ENUM_VALUE(Event,Down)
			PYAPI_ENUM_VALUE(Event,Up)
			PYAPI_ENUM_VALUE(Event,Trace)
			PYAPI_ENUM_VALUE(Event,Connect)
			PYAPI_ENUM_VALUE(Event,Untrace)
			PYAPI_ENUM_VALUE(Event,Disconnect)
			PYAPI_ENUM_VALUE(Event,Click)
			PYAPI_ENUM_VALUE(Event,DoubleClick)
			PYAPI_ENUM_VALUE(Event,MoveLeft)
			PYAPI_ENUM_VALUE(Event,MoveRight)
			PYAPI_ENUM_VALUE(Event,MoveUp)
			PYAPI_ENUM_VALUE(Event,MoveDown)
			PYAPI_ENUM_VALUE(Event,Zoom)
			PYAPI_ENUM_VALUE(Event,SplitStart)
			PYAPI_ENUM_VALUE(Event,SplitEnd)
			PYAPI_ENUM_VALUE(Event,Split)
			PYAPI_ENUM_VALUE(Event,RotateStart)
			PYAPI_ENUM_VALUE(Event,RotateEnd)
			PYAPI_ENUM_VALUE(Event,Rotate)
			PYAPI_ENUM_VALUE(Event,Null)
		;

	// Event Flags
	PYAPI_ENUM(EventBase::Flags, EventFlags)
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
	PYAPI_ENUM(EventBase::ExtraDataType, EventExtraDataType)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataNull)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataFloatArray)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataIntArray)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataVector3Array)
			PYAPI_ENUM_VALUE(EventBase,ExtraDataString)
		;

	// Event Extra Data Type
	PYAPI_ENUM(Service::ServiceType, ServiceType)
			PYAPI_ENUM_VALUE(Service,Pointer)
			PYAPI_ENUM_VALUE(Service,Mocap)
			PYAPI_ENUM_VALUE(Service,Keyboard) 
			PYAPI_ENUM_VALUE(Service,Controller)
			PYAPI_ENUM_VALUE(Service,Ui) 
			PYAPI_ENUM_VALUE(Service,Generic)
			PYAPI_ENUM_VALUE(Service,Brain)
			PYAPI_ENUM_VALUE(Service,Wand) 
			PYAPI_ENUM_VALUE(Service,Audio)
			;

	// Event
	const Vector3f& (Event::*getPosition1)() const = &Event::getPosition;
	PYAPI_REF_BASE_CLASS(Event)
		PYAPI_METHOD(Event, isKeyDown)
		PYAPI_METHOD(Event, isKeyUp)
		PYAPI_METHOD(Event, isButtonDown)
		PYAPI_METHOD(Event, getSourceId)
		PYAPI_METHOD(Event, getType)
		PYAPI_METHOD(Event, getServiceType)
		PYAPI_GETTER(Event, getPosition)
		PYAPI_GETTER(Event, getOrientation)
		;

	// Engine 
	PYAPI_REF_BASE_CLASS(Engine)
		PYAPI_METHOD(Engine, isConsoleEnabled)
		PYAPI_METHOD(Engine, setConsoleEnabled)
		PYAPI_REF_GETTER(Engine, getScene)
		PYAPI_REF_GETTER(Engine, getDefaultCamera)
		;

	// Node
	void (Node::*setPosition1)(const Vector3f&) = &Node::setPosition;
	void (Node::*setScale1)(const Vector3f&) = &Node::setScale;
	void (Node::*setOrientation1)(const Quaternion&) = &Node::setOrientation;
	Node* (Node::*getChildByIndex)(unsigned short) const = &Node::getChild;
	Node* (Node::*getChildByName)(const String&) const = &Node::getChild;
	
	void (Node::*removeChildByRef)(Node*) = &Node::removeChild;
	void (Node::*removeChildByName)(const String&) = &Node::removeChild;
	void (Node::*removeChildByIndex)(unsigned short) = &Node::removeChild;

	class_<Node, Ref<Node>, boost::noncopyable >("Node", no_init)
		.def("getPosition", &Node::getPosition, PYAPI_RETURN_VALUE)
		.def("setPosition", setPosition1)
		.def("getScale", &Node::getScale, PYAPI_RETURN_VALUE)
		.def("setScale", setScale1)
		.def("setOrientation", setOrientation1)
		.def("getOrientation", &Node::getOrientation, PYAPI_RETURN_VALUE)
		.def("yaw", &Node::yaw, NodeYawOverloads())
		.def("pitch", &Node::pitch, NodePitchOverloads())
		.def("roll", &Node::roll, NodeRollOverloads())

		PYAPI_METHOD(Node, numChildren)
		PYAPI_METHOD(Node, addChild)
		.def("getChildByName", getChildByName, PYAPI_RETURN_REF)
		.def("getChildByIndex", getChildByIndex, PYAPI_RETURN_REF)
		.def("removeChildByRef", removeChildByRef)
		.def("removeChildByName", removeChildByName)
		.def("removeChildByIndex", removeChildByIndex)

		PYAPI_METHOD(Node, resetOrientation)
		PYAPI_GETTER(Node, getName)
		PYAPI_REF_GETTER(Node, getParent)
		//.def("getChildren", &Node::getChildren, PYAPI_RETURN_REF)
		;

	// NodeList
	//PYAPI_POINTER_LIST(Node, "NodeList")

	// SceneNode
	PYAPI_REF_CLASS(SceneNode, Node)
		PYAPI_STATIC_REF_GETTER(SceneNode, create)
		PYAPI_METHOD(SceneNode, isVisible)
		PYAPI_METHOD(SceneNode, setVisible)
		PYAPI_METHOD(SceneNode, isSelected)
		PYAPI_METHOD(SceneNode, setSelected)
		PYAPI_METHOD(SceneNode, isSelectable)
		PYAPI_METHOD(SceneNode, setSelectable)
		PYAPI_METHOD(SceneNode, isBoundingBoxVisible)
		PYAPI_METHOD(SceneNode, setBoundingBoxVisible)
	;

	// Camera
	PYAPI_REF_CLASS(Camera, Node)
		PYAPI_METHOD(Camera, setYawPitchRoll)
		PYAPI_GETTER(Camera, getHeadOffset)
		PYAPI_METHOD(Camera, setHeadOffset)
		PYAPI_METHOD(Camera, setHeadOrientation)
		PYAPI_GETTER(Camera, getHeadOrientation)
		PYAPI_METHOD(Camera, isTrackingEnabled)
		PYAPI_METHOD(Camera, setTrackingEnabled)
		PYAPI_METHOD(Camera, getTrackerSourceId)
		PYAPI_METHOD(Camera, setTrackerSourceId)
		PYAPI_METHOD(Camera, setControllerEnabled)
		PYAPI_METHOD(Camera, isControllerEnabled)
		PYAPI_METHOD(Camera, localToWorldPosition)
		PYAPI_METHOD(Camera, localToWorldOrientation)
		;

	// Color
	class_<Color>("Color", init<String>());

	// Setting
	PYAPI_BASE_CLASS(Setting);

	// Actor
	PYAPI_REF_BASE_CLASS(Actor)
		PYAPI_METHOD(Actor, setSceneNode)
		PYAPI_REF_GETTER(Actor, getSceneNode)
		;

	// PortholeService
	PYAPI_REF_BASE_CLASS(PortholeService)
		PYAPI_STATIC_REF_GETTER(PortholeService, createAndInitialize)
		;

	// Free Functions
	def("getEvent", getEvent, return_value_policy<reference_existing_object>());
	def("getEngine", getEngine, PYAPI_RETURN_REF);
	def("getDefaultCamera", getDefaultCamera, PYAPI_RETURN_REF);
	def("getCameraById", getCameraById, PYAPI_RETURN_REF);
	def("getScene", getScene, PYAPI_RETURN_REF);
	def("querySceneRay", querySceneRay);
	def("getRayFromEvent", getRayFromEvent);
	def("printChildren", &printChildren);
	def("printObjCounts", &printObjCounts);
	def("settingLookup", &settingLookup, PYAPI_RETURN_VALUE);
	def("settingExists", &settingExists);
	def("toggleStats", &toggleStats);
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

	// Register omega::Vector2f <-> euclid.Vector2 converters
	boost::python::to_python_converter<Vector2f, Vector2f_to_python>();
	Vector2f_from_python();

	// Register omega::Quaternion <-> euclid.Quaternion converters
	boost::python::to_python_converter<Quaternion, Quaternion_to_python>();
	Quaternion_from_python();

	// Initialize the omega wrapper module
	initomega();

	interp->addModule("omega", omegaMethods);
}

#endif
