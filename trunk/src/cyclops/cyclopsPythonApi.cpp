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
#include "cyclops/SceneManager.h"
#include "cyclops/AnimatedObject.h"
#include "cyclops/SceneLoader.h"
#include "cyclops/SceneLoader.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
SceneManager* getSceneManager() { return SceneManager::instance(); }

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(cyclops)
{
	// SceneLoader
	class_<SceneLoader, boost::noncopyable >("SceneLoader", no_init)
		PYAPI_STATIC_REF_GETTER(SceneLoader, getLastLoadedEntity)
		;

	// ProgramAsset
	PYAPI_REF_BASE_CLASS(ProgramAsset);

	// SceneManager
	void (SceneManager::*loadModelAsync1)(ModelInfo*, const String&) = &SceneManager::loadModelAsync;
	PYAPI_REF_BASE_CLASS(SceneManager)
		PYAPI_METHOD(SceneManager, setMainLight)
		PYAPI_REF_GETTER(SceneManager, getMainLight)
		PYAPI_METHOD(SceneManager, loadModel)
		.def("loadModelAsync", loadModelAsync1)
		PYAPI_METHOD(SceneManager, setBackgroundColor)
		PYAPI_METHOD(SceneManager, loadScene)
		PYAPI_METHOD(SceneManager, unload)
		PYAPI_METHOD(SceneManager, setSkyBox)
		PYAPI_METHOD(SceneManager, displayWand)
		PYAPI_METHOD(SceneManager, hideWand)
		PYAPI_METHOD(SceneManager, setWandEffect)
		PYAPI_METHOD(SceneManager, setWandSize)
		PYAPI_GETTER(SceneManager, getCurrentShadowSettings)
		PYAPI_METHOD(SceneManager, resetShadowSettings)
		PYAPI_METHOD(SceneManager, setShaderMacroToFile)
		PYAPI_METHOD(SceneManager, setShaderMacroToString)
		PYAPI_REF_GETTER(SceneManager, createProgramFromString)
		PYAPI_REF_GETTER(SceneManager, createTexture)
		;

	// osg Texture2D
	class_<osg::Texture2D, boost::noncopyable, Ref<osg::Texture2D> >("Texture2D", no_init)
		;

	// Uniform
	PYAPI_REF_BASE_CLASS(Uniform)
		PYAPI_GETTER(Uniform, getType)
		PYAPI_METHOD(Uniform, setInt)
		PYAPI_METHOD(Uniform, getInt)
		PYAPI_METHOD(Uniform, setFloat)
		PYAPI_METHOD(Uniform, getFloat)
		PYAPI_METHOD(Uniform, setVector2f)
		PYAPI_GETTER(Uniform, getVector2f)
		PYAPI_METHOD(Uniform, setVector3f)
		PYAPI_GETTER(Uniform, getVector3f)
		PYAPI_METHOD(Uniform, setVector4f)
		PYAPI_GETTER(Uniform, getVector4f);

	// Uniforms
	PYAPI_REF_BASE_CLASS(Uniforms)
		PYAPI_REF_GETTER(Uniforms, addUniform)
		PYAPI_REF_GETTER(Uniforms, addUniformArray)
		PYAPI_REF_GETTER(Uniforms, getUniform)
		PYAPI_METHOD(Uniforms, removeAllUniforms);

	// Material
	PYAPI_REF_CLASS(Material, Uniforms)
		PYAPI_METHOD(Material, setDiffuseColor)
		PYAPI_METHOD(Material, setEmissiveColor)
		PYAPI_METHOD(Material, setTransparent)
		PYAPI_METHOD(Material, isTransparent);

	// Entity
	PYAPI_REF_CLASS(Entity, SceneNode)
		PYAPI_METHOD(Entity, followTrackable)
		PYAPI_METHOD(Entity, setFollowOffset)
		PYAPI_METHOD(Entity, unfollow)
		PYAPI_METHOD(Entity, castShadow)
		PYAPI_METHOD(Entity, doesCastShadow)
		PYAPI_METHOD(Entity, hasEffect)
		PYAPI_METHOD(Entity, setEffect)
		PYAPI_REF_GETTER(Entity, getMaterial)
		;

	// SphereShape
	PYAPI_REF_CLASS(SphereShape, Entity)
		PYAPI_STATIC_REF_GETTER(SphereShape, create)
		;

	// PlaneShape
	PYAPI_REF_CLASS(PlaneShape, Entity)
		PYAPI_STATIC_REF_GETTER(PlaneShape, create)
		;

	// BoxShape
	PYAPI_REF_CLASS(BoxShape, Entity)
		PYAPI_STATIC_REF_GETTER(BoxShape, create)
		;

	// StaticObject
	PYAPI_REF_CLASS(StaticObject, Entity)
		PYAPI_STATIC_REF_GETTER(StaticObject, create)
		;

	// AnimatedObject
	class_<AnimatedObject, bases<Entity>, boost::noncopyable, Ref<AnimatedObject> >("AnimatedObject", no_init)
		.def("create", &AnimatedObject::create, PYAPI_RETURN_REF).staticmethod("create")
		.def("hasAnimations", &AnimatedObject::hasAnimations)
		.def("getNumAnimations", &AnimatedObject::getNumAnimations)
		.def("playAnimation", &AnimatedObject::playAnimation)
		.def("loopAnimation", &AnimatedObject::loopAnimation)
		.def("pauseAnimation", &AnimatedObject::pauseAnimation)
		.def("stopAllAnimations", &AnimatedObject::stopAllAnimations)
		.def("getCurAnimation", &AnimatedObject::getCurAnimation)
		.def("getAnimationLength", &AnimatedObject::getAnimationLength)
		.def("getAnimationStart", &AnimatedObject::getAnimationStart)
		.def("setAnimationStart", &AnimatedObject::setAnimationStart)
		;

	// LightType
	PYAPI_ENUM(Light::LightType, LightType)
		PYAPI_ENUM_VALUE(Light, Point)
		PYAPI_ENUM_VALUE(Light, Directional)
		PYAPI_ENUM_VALUE(Light, Spot)
		PYAPI_ENUM_VALUE(Light, Custom);

	// Light
	class_<Light, bases<Node>, boost::noncopyable, Ref<Light> >("Light", no_init)
		.def("create", &Light::create, PYAPI_RETURN_REF).staticmethod("create")
		.def("setColor", &Light::setColor)
		.def("setAmbient", &Light::setAmbient)
		.def("setEnabled", &Light::setEnabled)
		.def("isEnabled", &Light::isEnabled)
		.def("setSoftShadowJitter", &Light::setSoftShadowJitter)
		.def("getSoftShadowJitter", &Light::getSoftShadowJitter)
		.def("setSoftShadowWidth", &Light::setSoftShadowWidth)
		.def("getSoftShadowWidth", &Light::getSoftShadowWidth)
		PYAPI_METHOD(Light, setAttenuation)
		PYAPI_GETTER(Light, getAttenuation)
		PYAPI_METHOD(Light, getLightType)
		PYAPI_METHOD(Light, setLightType)
		PYAPI_METHOD(Light, getLightFunction)
		PYAPI_METHOD(Light, setLightFunction)
		PYAPI_METHOD(Light, setLightDirection)
		PYAPI_METHOD(Light, getLightDirection)
		;

	// ModelInfo
	class_<ModelInfo, boost::noncopyable, Ref<ModelInfo> >("ModelInfo")
		.def_readwrite("name", &ModelInfo::name)
		.def_readwrite("description", &ModelInfo::description)
		.def_readwrite("generateNormals", &ModelInfo::generateNormals)
		.def_readwrite("generateTangents", &ModelInfo::generateNormals)
		.def_readwrite("numFiles", &ModelInfo::numFiles)
		.def_readwrite("path", &ModelInfo::path)
		.def_readwrite("size", &ModelInfo::size)
		.def_readwrite("optimize", &ModelInfo::optimize)
		.def_readwrite("usePowerOfTwoTextures", &ModelInfo::usePowerOfTwoTextures)
		;

	// ShadowSetings
	class_<ShadowSettings>("ShadowSettings")
		.def_readwrite("shadowsEnabled", &ShadowSettings::shadowsEnabled)
		.def_readwrite("shadowResolutionRatio", &ShadowSettings::shadowResolutionRatio)
		;

	PYAPI_REF_BASE_CLASS_WITH_CTOR(Skybox)
		PYAPI_METHOD(Skybox, loadCubeMap)
		;

	// Free Functions
	def("getSceneManager", getSceneManager, PYAPI_RETURN_REF);
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

		// import the module by default
		omega::PythonInterpreter* interp = SystemManager::instance()->getScriptInterpreter();
		interp->eval("from cyclops import *");
	}
}

#endif