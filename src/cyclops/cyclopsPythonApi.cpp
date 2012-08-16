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
#include "cyclops/AnimatedObject.h"
#include "cyclops/SceneLoader.h"
#include "cyclops/SkyboxSwitcher.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager* getSceneManager() { return SceneManager::instance(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(cyclops)
{
	// SceneManager
	class_<SceneManager, boost::noncopyable>("SceneManager", no_init)
		.def("setMainLight", &SceneManager::setMainLight)
		.def("getMainLight", &SceneManager::getMainLight, PYAPI_RETURN_POINTER)
		.def("loadModel", &SceneManager::loadModel)
		.def("setBackgroundColor", &SceneManager::setBackgroundColor)
		;

	// Entity
	class_<Entity, bases<SceneNode>, boost::noncopyable >("Entity", no_init)
		.def("hasEffect", &Entity::hasEffect)
		.def("setEffect", &Entity::setEffect)
		;

	// SphereShape
	class_<SphereShape, bases<Entity>, boost::noncopyable >("SphereShape", no_init)
		.def("create", &SphereShape::create, PYAPI_RETURN_NEW_INSTANCE).staticmethod("create")
		;

	// PlaneShape
	class_<PlaneShape, bases<Entity>, boost::noncopyable >("PlaneShape", no_init)
		.def("create", &PlaneShape::create, PYAPI_RETURN_NEW_INSTANCE).staticmethod("create")
		;

	// StaticObject
	class_<StaticObject, bases<Entity>, boost::noncopyable >("StaticObject", no_init)
		.def("create", &StaticObject::create, PYAPI_RETURN_NEW_INSTANCE).staticmethod("create")
		;

	// Light
	class_<Light, bases<Node>, boost::noncopyable >("Light", no_init)
		.def("create", &Light::create, PYAPI_RETURN_NEW_INSTANCE).staticmethod("create")
		.def("setColor", &Light::setColor)
		.def("setAmbient", &Light::setAmbient)
		.def("setEnabled", &Light::setEnabled)
		.def("isEnabled", &Light::isEnabled)
		.def("setSoftShadowJitter", &Light::setSoftShadowJitter)
		.def("getSoftShadowJitter", &Light::getSoftShadowJitter)
		.def("setSoftShadowWidth", &Light::setSoftShadowWidth)
		.def("getSoftShadowWidth", &Light::getSoftShadowWidth)
		;

	class_<ModelInfo, boost::noncopyable>("ModelInfo")
		.def_readwrite("name", &ModelInfo::name)
		.def_readwrite("description", &ModelInfo::description)
		.def_readwrite("generateNormals", &ModelInfo::generateNormals)
		.def_readwrite("numFiles", &ModelInfo::numFiles)
		.def_readwrite("path", &ModelInfo::path)
		.def_readwrite("size", &ModelInfo::size)
		;

	class_<SkyboxSwitcher, boost::noncopyable>("SkyboxSwitcher", no_init)
		.def("createAndInitialize", &SkyboxSwitcher::createAndInitialize, PYAPI_RETURN_NEW_INSTANCE).staticmethod("createAndInitialize")
		.def("getNumSkyboxes", &SkyboxSwitcher::getNumSkyboxes)
		.def("getActiveSkybox", &SkyboxSwitcher::getActiveSkybox)
		.def("setActiveSkybox", &SkyboxSwitcher::setActiveSkybox)
		;

	// Free Functions
	def("getSceneManager", getSceneManager, PYAPI_RETURN_POINTER);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CY_API void cyclopsPythonApiInit()
{
	static bool sApiInitialized = false;

	if(!sApiInitialized)
	{
		sApiInitialized = true;
		omsg("cyclopsPythonApiInit()");
		initcyclops();
		//omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		//interp->addModule("cyclops", cyMethods);
	}
}

#endif