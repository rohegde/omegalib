/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omega/SystemManager.h"
#include "omega/DisplaySystem.h"
#include "omega/EqualizerDisplaySystem.h"
#include "omega/Engine.h"
#include "omega/Actor.h"
#include "omega/ImageUtils.h"
#include "omega/CameraController.h"
#include "omega/MissionControl.h"
#ifdef OMEGA_USE_PORTHOLE
	#include "omega/PortholeService.h"
#endif

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

#include <boost/mpl/if.hpp>

#define PYCAP_GET(pyobj, className) pyobj != NULL ? (className*)PyCapsule_GetPointer(pyobj, #className) : NULL

using namespace omega;

//! Static instance of ScriptRendererCommand, used by rendererQueueCommand
//ScriptRendererCommand* sScriptRendererCommand = NULL;

PyObject* sEuclidModule = NULL;

bool sRefPtrForwardingEnabled = false;
void enableRefPtrForwarding() { sRefPtrForwardingEnabled = true; }
void disableRefPtrForwarding() { sRefPtrForwardingEnabled = false; }
bool isRefPtrForwardingEnabled() { return sRefPtrForwardingEnabled; }


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

/////////////////////////////////////////////////////////////////////////////////////////////////////
static PyObject* omegaDrawCallback(PyObject *dummy, PyObject *args)
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
		interp->registerCallback(temp, PythonInterpreter::CallbackDraw);

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

	// Base omegalib API
	{"oexit", omegaExit, METH_VARARGS, 
		"oexit()\n"
		"Terminates the current omegalib program"},

    {"ofindFile", omegaFindFile, METH_VARARGS, 
		"ofindFile(name)\n"
		"Searches for a file in the application data filesystems and returns a full path if found"},

    {"setUpdateFunction", omegaUpdateCallback, METH_VARARGS, 
		"setUpdateFunction(funcRef)\n"
		"Registers a script function to be called before each frame is rendered"},

    {"setEventFunction", omegaEventCallback, METH_VARARGS, 
		"setEventFunction(funcRef)\n"
		"Registers a script function to be called when events are received"},

    {"setDrawFunction", omegaDrawCallback, METH_VARARGS, 
		"setDrawFunction(funcRef)\n"
		"Registers a script function to be called when drawing"},

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
const bool getBoolSetting(const String& section, const String& name, bool defaultValue)
{
	if(SystemManager::settingExists(section))
	{
		const Setting& s = SystemManager::settingLookup(section);
		return Config::getBoolValue(name, s, defaultValue);
	}
	return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void queueCommand(const String& command)
{
	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	// Mark the command as queued locally, since we expect queueCommand to be executed on all nodes
	// when running in a distributed environment
	interp->queueCommand(command, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @internal
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
// @internal
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
// @internal
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
// @internal
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
// @internal
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
// @internal
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
	if(sqrl.size() == 0)
	{
		callback(boost::python::ptr<SceneNode*>(NULL), 0);
	}
	else
	{
		foreach(SceneQueryResult sqr, sqrl)
		{
			callback(boost::python::ptr(sqr.node), sqr.distance);
		}
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
Camera* getCamera(const String& name)
{
	return Engine::instance()->getCamera(name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* getOrCreateCamera(const String& name)
{
	Camera* cam = Engine::instance()->getCamera(name);
	if(cam == NULL)
	{
		cam = Engine::instance()->createCamera(name);
	}
	return cam;
}

#ifdef OMEGA_USE_PORTHOLE
///////////////////////////////////////////////////////////////////////////////////////////////////
Camera* getCameraById(int id)
{
	if(SystemManager::instance()->isMaster())
	{
		return PortholeGUI::CamerasMap[id]->camera;
	}
	return NULL;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
bool isMaster()
{
	return SystemManager::instance()->isMaster();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
String getHostname()
{
	return SystemManager::instance()->getHostname();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneNode* getScene()
{
	return Engine::instance()->getScene();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SoundEnvironment* getSoundEnvironment()
{
	return Engine::instance()->getSoundEnvironment();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool isSoundEnabled()
{
	return Engine::instance()->getSoundManager()->isSoundServerRunning();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void toggleStats(const String& stats)
{
	EqualizerDisplaySystem* eqds = dynamic_cast<EqualizerDisplaySystem*>(SystemManager::instance()->getDisplaySystem());
	if(eqds != NULL)
	{
		eqds->toggleStats(stats);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void overridePanopticStereo(bool value)
{
	EqualizerDisplaySystem* eqds = dynamic_cast<EqualizerDisplaySystem*>(SystemManager::instance()->getDisplaySystem());
	if(eqds != NULL)
	{
		eqds->getDisplayConfig().panopticStereoEnabled = value;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void toggleStereo()
{
	EqualizerDisplaySystem* eqds = dynamic_cast<EqualizerDisplaySystem*>(SystemManager::instance()->getDisplaySystem());
	if(eqds != NULL)
	{
		eqds->getDisplayConfig().forceMono = !eqds->getDisplayConfig().forceMono;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void setNearFarZ(float nearZ, float farZ)
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	ds->setNearFarZ(nearZ, farZ);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float getNearZ(float near)
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	return ds->getNearZ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float getFarZ(float near)
{
	DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
	return ds->getFarZ();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void mcstart(int port = MissionControlServer::DefaultPort)
{
	if(isMaster())
	{
		omsg("Initializing mission control server...");
		MissionControlServer* srv = new MissionControlServer();
		srv->setMessageHandler(new MissionControlMessageHandler());
		srv->setPort(port);

		// Register the mission control server. The service manager will take care of polling the server
		// periodically to check for new connections.
		SystemManager::instance()->getServiceManager()->addService(srv);
		srv->start();
	}
}

// Oh god the humanity!!
static Ref<MissionControlClient> sMissionControlClient = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
void mcconnect(const String& host = "127.0.0.1", int port = MissionControlServer::DefaultPort)
{
	if(isMaster())
	{
		sMissionControlClient = new MissionControlClient();
		ModuleServices::addModule(sMissionControlClient);
		sMissionControlClient->doInitialize(Engine::instance());
		sMissionControlClient->connect(host, port);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void mcclose()
{
	if(isMaster())
	{
		if(sMissionControlClient != NULL)
		{
			ModuleServices::removeModule(sMissionControlClient);
			sMissionControlClient = NULL;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void orun(const String& script)
{
	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->runFile(script);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void oclean()
{
	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->clean();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ocleanrun(const String& script)
{
	PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
	interp->cleanRun(script);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! Class wrapping a PixelData object and a filename, used by the python wrapper
//class ImageFile
//{
//public:
//	ImageFile(const ImageFile& rhs):
//	  myFilename(rhs.myFilename), myPixels(rhs.myPixels)
//	  {}
//
//	ImageFile(const String filename, PixelData* pixels):
//	  myFilename(filename), myPixels(pixels)
//	  {}
//
//	const String& getFilename() { return myFilename; }
//	PixelData* getPixels() { return myPixels; }
//	bool isLoaded() { return myPixels != NULL; }
//
//private:
//	String myFilename;
//	Ref<PixelData> myPixels;
//};

///////////////////////////////////////////////////////////////////////////////////////////////////
PixelData* loadImage(const String& filename)
{
	Ref<PixelData> data = ImageUtils::loadImage(filename);
	if(data != NULL)
	{
		enableRefPtrForwarding();
		data->ref();
		return data;
	}
	return NULL;
	//return ImageFile(filename, data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void resetDataPaths()
{
	DataManager* dm = DataManager::getInstance();
	dm->removeAllSources();

	// Always re-add search paths for relative and absolute paths.
	dm->addSource(new FilesystemDataSource("./"));
	dm->addSource(new FilesystemDataSource(""));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addDataPath(const String& path)
{
	DataManager* dm = DataManager::getInstance();
	dm->addSource(new FilesystemDataSource(path));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void printDataPaths()
{
	DataManager* dm = DataManager::getInstance();
	omsg(dm->getDataSourceNames());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void setImageLoaderThreads(int threads)
{
	ImageUtils::setImageLoaderThreads(threads);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int getImageLoaderThreads()
{
	return ImageUtils::getImageLoaderThreads();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void printModules()
{
	Vector<EngineModule*> mods = ModuleServices::getModules();
	foreach(EngineModule* m, mods)
	{
		ofmsg("%1%", %m->getName());
	}
}

BOOST_PYTHON_FUNCTION_OVERLOADS(mcstartOverloads , mcstart, 0, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(mcconnectOverloads , mcconnect, 0, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeYawOverloads, yaw, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodePitchOverloads, pitch, 1, 2) 
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NodeRollOverloads, roll, 1, 2) 
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(omega)
{
	// Font alignment
	PYAPI_ENUM(Font::Align, TextAlign)
			PYAPI_ENUM_VALUE(Font, HALeft)
			PYAPI_ENUM_VALUE(Font, HARight)
			PYAPI_ENUM_VALUE(Font, HACenter)
			PYAPI_ENUM_VALUE(Font, VATop)
			PYAPI_ENUM_VALUE(Font, VABottom)
			PYAPI_ENUM_VALUE(Font, VAMiddle)
			;

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
			PYAPI_ENUM_VALUE(Event,Zoom)
			PYAPI_ENUM_VALUE(Event,Split)
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
			;

	// Event
	const Vector3f& (Event::*getPosition1)() const = &Event::getPosition;
	PYAPI_REF_BASE_CLASS(Event)
		PYAPI_METHOD(Event, isKeyDown)
		PYAPI_METHOD(Event, isKeyUp)
		PYAPI_METHOD(Event, isButtonDown)
		PYAPI_METHOD(Event, getAxis)
		PYAPI_METHOD(Event, getSourceId)
		PYAPI_METHOD(Event, getType)
		PYAPI_METHOD(Event, getServiceType)
		PYAPI_METHOD(Event, isProcessed)
		PYAPI_METHOD(Event, setProcessed)
		PYAPI_GETTER(Event, getPosition)
		PYAPI_GETTER(Event, getOrientation)
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

		PYAPI_METHOD(Node, lookAt)
		PYAPI_METHOD(Node, numChildren)
		PYAPI_METHOD(Node, addChild)
		.def("getChildByName", getChildByName, PYAPI_RETURN_REF)
		.def("getChildByIndex", getChildByIndex, PYAPI_RETURN_REF)
		.def("removeChildByRef", removeChildByRef)
		.def("removeChildByName", removeChildByName)
		.def("removeChildByIndex", removeChildByIndex)

		PYAPI_METHOD(Node, resetOrientation)
		PYAPI_GETTER(Node, getName)
		PYAPI_METHOD(Node, setName)
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
		PYAPI_METHOD(SceneNode, setTag)
		PYAPI_GETTER(SceneNode, getTag)
		PYAPI_GETTER(SceneNode, setFacingCamera)
		PYAPI_GETTER(SceneNode, getFacingCamera)
		PYAPI_GETTER(SceneNode, getBoundMinimum)
		PYAPI_GETTER(SceneNode, getBoundMaximum)
		PYAPI_GETTER(SceneNode, getBoundCenter)
		PYAPI_GETTER(SceneNode, getBoundRadius)
	;

	// CameraController
	PYAPI_REF_BASE_CLASS(CameraController)
		PYAPI_METHOD(CameraController, getSpeed)
		PYAPI_METHOD(CameraController, setSpeed);

	// Camera
	PYAPI_REF_CLASS(Camera, Node)
		PYAPI_REF_GETTER(Camera, getController)
		PYAPI_METHOD(Camera, setController)
		PYAPI_METHOD(Camera, setPitchYawRoll)
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
		PYAPI_METHOD(Camera, focusOn)
		;

	// Color
	class_<Color>("Color", init<String>())
		.def(init<float, float, float, float>())
		.add_property("red", &Color::getRed, &Color::setRed)
		.add_property("green", &Color::getGreen, &Color::setGreen)
		.add_property("blue", &Color::getBlue, &Color::setBlue)
		.add_property("alpha", &Color::getAlpha, &Color::setAlpha);

	// Actor
	PYAPI_REF_BASE_CLASS(Actor)
		PYAPI_METHOD(Actor, setSceneNode)
		PYAPI_REF_GETTER(Actor, getSceneNode)
		;

#ifdef OMEGA_USE_PORTHOLE
	// PortholeService
	PYAPI_REF_BASE_CLASS(PortholeService)
		PYAPI_STATIC_REF_GETTER(PortholeService, createAndInitialize)
		;
#endif

	// DrawInterface
	PYAPI_REF_BASE_CLASS(DrawInterface)
		PYAPI_METHOD(DrawInterface, drawRectGradient)
		PYAPI_METHOD(DrawInterface, drawRect)
		PYAPI_METHOD(DrawInterface, drawRectOutline)
		PYAPI_METHOD(DrawInterface, drawText)
		PYAPI_METHOD(DrawInterface, drawRectTexture)
		PYAPI_METHOD(DrawInterface, drawCircleOutline)
		PYAPI_REF_GETTER(DrawInterface, createFont)
		PYAPI_REF_GETTER(DrawInterface, getFont)
		PYAPI_REF_GETTER(DrawInterface, getDefaultFont)
		;

	// Font
	PYAPI_REF_BASE_CLASS(Font)
		PYAPI_METHOD(Font, computeSize)
		;

	// PixelData
	PYAPI_REF_BASE_CLASS(PixelData)
		PYAPI_METHOD(PixelData, getWidth)
		PYAPI_METHOD(PixelData, getHeight)
		;

	// ImageFile
	//PYAPI_BASE_CLASS(ImageFile)
	//	PYAPI_METHOD(ImageFile, isLoaded)
	//	PYAPI_GETTER(ImageFile, getFilename)
	//	PYAPI_REF_GETTER(ImageFile, getPixels)
	//	;

	// SoundEnvironment
	PYAPI_REF_BASE_CLASS(SoundEnvironment)
		PYAPI_REF_GETTER(SoundEnvironment, loadSoundFromFile)
		PYAPI_REF_GETTER(SoundEnvironment, setAssetDirectory)
		;

	// Sound
	PYAPI_REF_BASE_CLASS(Sound)
		PYAPI_METHOD(Sound, getDuration)
		PYAPI_METHOD(Sound, getVolumeScale)
		PYAPI_METHOD(Sound, setVolumeScale)
		;

	// SoundInstance
	void (SoundInstance::*playSimple)() = &SoundInstance::play;
	void (SoundInstance::*playSimpleStereo)() = &SoundInstance::playStereo;
	class_<SoundInstance, boost::noncopyable, Ref<SoundInstance> >("SoundInstance", init<Sound*>())
		.def("play", playSimple)
		.def("playStereo", playSimpleStereo)
		PYAPI_METHOD(SoundInstance, pause)
		PYAPI_METHOD(SoundInstance, stop)
		PYAPI_METHOD(SoundInstance, isPlaying)
		PYAPI_METHOD(SoundInstance, setLoop)
		PYAPI_METHOD(SoundInstance, getLoop)
		PYAPI_METHOD(SoundInstance, setPosition)
		PYAPI_GETTER(SoundInstance, getPosition)
		PYAPI_METHOD(SoundInstance, isEnvironmentSound)
		PYAPI_METHOD(SoundInstance, setEnvironmentSound)
		PYAPI_METHOD(SoundInstance, setVolume)
		PYAPI_METHOD(SoundInstance, getVolume)
		PYAPI_METHOD(SoundInstance, setWidth)
		PYAPI_METHOD(SoundInstance, getWidth)
		PYAPI_METHOD(SoundInstance, setMix)
		PYAPI_METHOD(SoundInstance, getMix)
		PYAPI_METHOD(SoundInstance, setReverb)
		PYAPI_METHOD(SoundInstance, getReverb)
		PYAPI_METHOD(SoundInstance, fade)
		;

	// Free Functions
	def("getEvent", getEvent, return_value_policy<reference_existing_object>());
	def("getEngine", getEngine, PYAPI_RETURN_REF);
	def("getDefaultCamera", getDefaultCamera, PYAPI_RETURN_REF);
	def("getCamera", getCamera, PYAPI_RETURN_REF);
	def("getOrCreateCamera", getOrCreateCamera, PYAPI_RETURN_REF);
#ifdef OMEGA_USE_PORTHOLE
	def("getCameraById", getCameraById, PYAPI_RETURN_REF);
#endif
	def("getScene", getScene, PYAPI_RETURN_REF);
	def("getSoundEnvironment", getSoundEnvironment, PYAPI_RETURN_REF);
	def("isSoundEnabled", isSoundEnabled);
	def("querySceneRay", querySceneRay);
	def("getRayFromEvent", getRayFromEvent);
	def("printChildren", &printChildren);
	def("printObjCounts", &printObjCounts);
	def("getBoolSetting", &getBoolSetting);
	def("toggleStats", &toggleStats);
	def("overridePanopticStereo", overridePanopticStereo);
	def("toggleStereo", toggleStereo);
	def("queueCommand", queueCommand);
	def("ogetdataprefix", ogetdataprefix);
	def("osetdataprefix", osetdataprefix);
	def("isMaster", isMaster);
	def("loadImage", loadImage, PYAPI_RETURN_REF);

	// NEW IN 3.2.1
	def("addDataPath", addDataPath);
	def("resetDataPaths", addDataPath);
	def("printDataPaths", printDataPaths);

	// NEW IN 3.3
	def("setImageLoaderThreads", setImageLoaderThreads);
	def("getImageLoaderThreads", getImageLoaderThreads);
	def("getHostname", getHostname, PYAPI_RETURN_VALUE);
	def("printModules", printModules);

	// NEW IN 3.4
	def("mcstart", mcstart, mcstartOverloads());
	def("mcconnect", mcconnect, mcconnectOverloads());
	def("mcclose", mcclose);

	def("orun", orun);
	def("oclean", oclean);
	def("ocleanrun", ocleanrun);

	def("setNearFarZ", setNearFarZ);
	def("getNearZ", getNearZ);
	def("getFarZ", getFarZ);
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
