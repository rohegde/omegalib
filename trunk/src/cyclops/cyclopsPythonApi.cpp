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
#include "cyclops/LineSet.h"

#ifdef OMEGA_USE_PYTHON

#include "omega/PythonInterpreterWrapper.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

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

	// PrimitiveType
	PYAPI_ENUM(ProgramAsset::PrimitiveType, PrimitiveType)
		PYAPI_ENUM_VALUE(ProgramAsset, Points)
		PYAPI_ENUM_VALUE(ProgramAsset, Triangles)
		PYAPI_ENUM_VALUE(ProgramAsset, TriangleStrip);

	// ProgramAsset
	PYAPI_REF_BASE_CLASS_WITH_CTOR(ProgramAsset)
		.def_readwrite("name", &ProgramAsset::name)
		.def_readwrite("vertexShaderName", &ProgramAsset::vertexShaderName)
		.def_readwrite("fragmentShaderName", &ProgramAsset::fragmentShaderName)
		.def_readwrite("geometryShaderName", &ProgramAsset::geometryShaderName)
		.def_readwrite("vertexShaderSource", &ProgramAsset::vertexShaderSource)
		.def_readwrite("fragmentShaderSource", &ProgramAsset::fragmentShaderSource)
		.def_readwrite("geometryShaderSource", &ProgramAsset::geometryShaderSource)
		.def_readwrite("geometryOutVertices", &ProgramAsset::geometryOutVertices)
		.def_readwrite("geometryInput", &ProgramAsset::geometryInput)
		.def_readwrite("geometryOutput", &ProgramAsset::geometryOutput)
		.def_readwrite("embedded", &ProgramAsset::embedded);

	PYAPI_REF_BASE_CLASS(ModelLoader);

	// SceneManager
	void (SceneManager::*loadModelAsync1)(ModelInfo*, const String&) = &SceneManager::loadModelAsync;
	PYAPI_REF_BASE_CLASS(SceneManager)
		PYAPI_METHOD(SceneManager, setMainLight)
		PYAPI_REF_GETTER(SceneManager, getMainLight)
		PYAPI_METHOD(SceneManager, loadModel)
		.def("loadModelAsync", loadModelAsync1)
		PYAPI_METHOD(SceneManager, setBackgroundColor)
		PYAPI_METHOD(SceneManager, loadScene)
		PYAPI_METHOD(SceneManager, addLoader)
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
		PYAPI_REF_GETTER(SceneManager, getGlobalUniforms)
		PYAPI_METHOD(SceneManager, addProgram)
		PYAPI_METHOD(SceneManager, updateProgram)
		PYAPI_REF_GETTER(SceneManager, createProgramFromString)
		PYAPI_REF_GETTER(SceneManager, createTexture)
		PYAPI_METHOD(SceneManager, reloadAndRecompileShaders)
		;

	// osg Texture2D
	class_<osg::Texture2D, boost::noncopyable, Ref<osg::Texture2D> >("Texture2D", no_init)
		;

	// UniformType
	PYAPI_ENUM(Uniform::Type, UniformType)
		PYAPI_ENUM_VALUE(Uniform, Vector2f)
		PYAPI_ENUM_VALUE(Uniform, Vector3f)
		PYAPI_ENUM_VALUE(Uniform, Color)
		PYAPI_ENUM_VALUE(Uniform, Float)
		PYAPI_ENUM_VALUE(Uniform, Int);

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
		PYAPI_METHOD(Uniform, setColor)
		PYAPI_GETTER(Uniform, getColor);

	// Uniforms
	PYAPI_REF_BASE_CLASS(Uniforms)
		PYAPI_REF_GETTER(Uniforms, addUniform)
		PYAPI_REF_GETTER(Uniforms, addUniformArray)
		PYAPI_REF_GETTER(Uniforms, getUniform)
		PYAPI_METHOD(Uniforms, removeAllUniforms);

	// Material
	PYAPI_REF_CLASS(Material, Uniforms)
		PYAPI_STATIC_REF_GETTER(Material, create)
		PYAPI_METHOD(Material, setColor)
		PYAPI_METHOD(Material, setTransparent)
		PYAPI_METHOD(Material, isTransparent)
		PYAPI_METHOD(Material, reset)
		PYAPI_METHOD(Material, setProgram)
		PYAPI_METHOD(Material, parse)
		PYAPI_METHOD(Material, setAdditive)
		PYAPI_METHOD(Material, isAdditive)
		PYAPI_METHOD(Material, setDepthTestEnabled)
		PYAPI_METHOD(Material, isDepthTestEnabled)
		PYAPI_METHOD(Material, setDoubleFace)
		PYAPI_METHOD(Material, isDoubleFace)
		PYAPI_METHOD(Material, setWireframe)
		PYAPI_METHOD(Material, isWireframe)
		PYAPI_METHOD(Material, setPolygonOffset)
		PYAPI_METHOD(Material, setDiffuseTexture)
		PYAPI_GETTER(Material, getDiffuseTexture)
		PYAPI_METHOD(Material, setNormalTexture)
		PYAPI_GETTER(Material, getNormalTexture)
		PYAPI_METHOD(Material, getShininess)
		PYAPI_METHOD(Material, setShininess)
		PYAPI_METHOD(Material, getGloss)
		PYAPI_METHOD(Material, setGloss)
		PYAPI_METHOD(Material, setAlpha)
		PYAPI_METHOD(Material, getAlpha);

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
		PYAPI_REF_GETTER(Entity, createContextMenu)
		PYAPI_REF_GETTER(Entity, getContextMenu)
		PYAPI_METHOD(Entity, deleteContextMenu)
		PYAPI_GETTER(Entity, listPieces)
		PYAPI_REF_GETTER(Entity, getPiece)
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

	// CylinderShape
	PYAPI_REF_CLASS(CylinderShape, Entity)
		PYAPI_STATIC_REF_GETTER(CylinderShape, create)
		;

	// Line
	class_<LineSet::Line, boost::noncopyable, Ref<LineSet::Line> >("Line", no_init)
		PYAPI_METHOD(LineSet::Line, setStart)
		PYAPI_METHOD(LineSet::Line, setEnd)
		PYAPI_METHOD(LineSet::Line, setThickness)
		PYAPI_METHOD(LineSet::Line, getThickness)
		;

	// LineSet
	PYAPI_REF_CLASS(LineSet, Entity)
		PYAPI_STATIC_REF_GETTER(LineSet, create)
		PYAPI_REF_GETTER(LineSet, addLine)
		PYAPI_METHOD(LineSet, removeLine)
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
		PYAPI_METHOD(Light, getSpotCutoff)
		PYAPI_METHOD(Light, setSpotCutoff)
		PYAPI_METHOD(Light, getSpotExponent)
		PYAPI_METHOD(Light, setSpotExponent)
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

	// SkyBox
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