/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "cyclops/Uniforms.h"

#include <osg/StateSet>

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniform::Uniform(osg::Uniform* uniform, Type type, uint elements):
	myOsgUniform(uniform),
	myType(type),
	myNumElements(elements)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniform::~Uniform()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Uniform::setFloat(float value)
{
	myOsgUniform->set(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float Uniform::getFloat()
{
	float value;
	myOsgUniform->get(value);
	return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Uniform::setInt(int value)
{
	myOsgUniform->set(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Uniform::getInt()
{
	int value;
	myOsgUniform->get(value);
	return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Uniform::setVector2f(const omega::Vector2f& value)
{ 
	osg::Vec2 ov(value[0], value[1]);
	myOsgUniform->set(ov);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::Vector2f Uniform::getVector2f()
{
	osg::Vec2 value;
	myOsgUniform->get(value);
	return omega::Vector2f(value[0], value[1]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Uniform::setVector3f(const omega::Vector2f& value)
{
	osg::Vec3 ov(value[0], value[1], value[2]);
	myOsgUniform->set(ov);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::Vector3f Uniform::getVector3f()
{
	osg::Vec3 value;
	myOsgUniform->get(value);
	return omega::Vector3f(value[0], value[1], value[2]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Uniform::setVector4f(const omega::Vector4f& value)
{
	osg::Vec4 ov(value[0], value[1], value[2], value[3]);
	myOsgUniform->set(ov);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
omega::Vector4f Uniform::getVector4f()
{
	osg::Vec4 value;
	myOsgUniform->get(value);
	return omega::Vector4f(value[0], value[1], value[2], value.w());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniforms::Uniforms(osg::StateSet* stateset):
	myStateSet(stateset)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniforms::~Uniforms()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniform* Uniforms::addUniform(const String& name, Uniform::Type type)
{
	if(myUniformDictionary.find(name) == myUniformDictionary.end())
	{
		// Create uniform now.
		osg::Uniform* u = myStateSet->getOrCreateUniform(name, toOsgUniformType(type), 1);
		myUniformDictionary[name] = new Uniform(u, type, 1);
	}
	return myUniformDictionary[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniform* Uniforms::addUniformArray(const String& name, Uniform::Type type, uint elements)
{
	if(myUniformDictionary.find(name) == myUniformDictionary.end())
	{
		// Create uniform now.
		osg::Uniform* u = myStateSet->getOrCreateUniform(name, toOsgUniformType(type), elements);
		myUniformDictionary[name] = new Uniform(u, type, elements);
	}
	return myUniformDictionary[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Uniform* Uniforms::getUniform(const String& name)
{
	if(myUniformDictionary.find(name) == myUniformDictionary.end()) return NULL;
	return myUniformDictionary[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Uniforms::removeAllUniforms()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Uniform::Type Uniforms::toOsgUniformType(Uniform::Type type)
{
	switch(type)
	{
	case Uniform::Int: return osg::Uniform::INT;
	case Uniform::Float: return osg::Uniform::FLOAT;
	case Uniform::Vector2f: return osg::Uniform::FLOAT_MAT2;
	case Uniform::Vector3f: return osg::Uniform::FLOAT_MAT3;
	case Uniform::Vector4f: return osg::Uniform::FLOAT_MAT4;
	}
	return osg::Uniform::UNDEFINED;
}

