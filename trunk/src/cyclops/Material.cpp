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
#include "cyclops/Material.h"
#include "cyclops/MaterialParser.h"
#include "cyclops/SceneManager.h"

#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include<osg/BlendFunc>

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
Material::Material(osg::StateSet* ss, SceneManager* sm): Uniforms(ss), myStateSet(ss), myTransparent(false), mySceneManager(sm)
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Material::parse(const String& definition)
{
	return MaterialParser::parseMaterialString(this, definition);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setColor(const Color& diffuse, const Color& emissive)
{
	if(myMaterial == NULL)
	{
		myMaterial = new osg::Material();
		myStateSet->setAttributeAndModes(myMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
	myMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, COLOR_TO_OSG(diffuse));
	myMaterial->setEmission(osg::Material::FRONT_AND_BACK, COLOR_TO_OSG(emissive));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setShininess(float value)
{
	myShininess->setFloat(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setGloss(float value)
{
	myGloss->setFloat(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Material::getShininess()
{
	return myShininess->getFloat();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Material::getGloss()
{
	return myGloss->getFloat();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setDiffuseTexture(const String& name)
{
	if(!getUniform("unif_DiffuseMap"))	addUniform("unif_DiffuseMap", Uniform::Int)->setInt(0);
	osg::Texture2D* tex = mySceneManager->getTexture(name);
	if(tex != NULL)
	{
		tex->setResizeNonPowerOfTwoHint(false);
		myStateSet->setTextureAttribute(0, tex);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setNormalTexture(const String& name)
{
	if(!getUniform("unif_NormalMap"))	addUniform("unif_NormalMap", Uniform::Int)->setInt(0);
	osg::Texture2D* tex = mySceneManager->getTexture(name);
	if(tex != NULL)
	{
		tex->setResizeNonPowerOfTwoHint(false);
		myStateSet->setTextureAttribute(0, tex);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setTransparent(bool value)
{
	myTransparent = value;
	if(myTransparent)
	{
		myStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		myStateSet->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}
	else
	{
		myStateSet->setRenderingHint(osg::StateSet::OPAQUE_BIN);
		myStateSet->setMode(GL_BLEND, osg::StateAttribute::OFF| osg::StateAttribute::OVERRIDE);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setAdditive(bool value)
{
	myAdditive = value;
	if(myAdditive)
	{
		osg::BlendFunc* bf = new osg::BlendFunc();
		bf->setFunction(GL_SRC_ALPHA, GL_ONE);
		myStateSet->setAttribute(bf);
	}
	else
	{
		osg::BlendFunc* bf = new osg::BlendFunc();
		bf->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		myStateSet->setAttribute(bf);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setDepthTestEnabled(bool value)
{
	myDepthTest = value;
	myStateSet->setMode(GL_DEPTH_TEST, myDepthTest ? osg::StateAttribute::ON : osg::StateAttribute::OFF);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setDoubleFace(bool value)
{
	myDoubleFace = value;
	myStateSet->setMode(GL_CULL_FACE, myDoubleFace ? osg::StateAttribute::OFF : osg::StateAttribute::ON);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setWireframe(bool value)
{
	myWireframe = value;
	if(myWireframe)
	{
		myStateSet->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON);
	}
	else
	{
		myStateSet->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), osg::StateAttribute::ON);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setPolygonOffset(float factor, float units)
{
	if(factor == 0.0f && units == 0.0f)
	{
		myStateSet->setAttributeAndModes(new osg::PolygonOffset(0.0f, 0.0f), osg::StateAttribute::OFF);
	}
	else
	{
		myStateSet->setAttributeAndModes(new osg::PolygonOffset(factor, units), osg::StateAttribute::ON);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::setAlpha(float value)
{
	myAlpha->setFloat(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float Material::getAlpha()
{
	return myAlpha->getFloat();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Material::reset()
{
	removeAllUniforms();
	myStateSet->clear();
	myMaterial = NULL;

	myAlpha = addUniform("unif_Alpha", Uniform::Float);
	myAlpha->setFloat(1.0f);

	myGloss = addUniform("unif_Gloss", Uniform::Float);
	myGloss->setFloat(1.0f);

	myShininess = addUniform("unif_Shininess", Uniform::Float);
	myGloss->setFloat(1.0f);

	// Reset flags to default values.
	setTransparent(false);
	setDepthTestEnabled(true);
	setAdditive(false);
	setDoubleFace(false);

	myStateSet->setNestRenderBins(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////
bool Material::setProgram(const String& name)
{
	String pname;
	String pvar;

	// If we have two arguments, second one is the variant name.
	Vector<String> args = StringUtils::split(name, " ");
	if(args.size() == 2)
	{
		pname = args[0];
		pvar = args[1];
	}
	else
	{
		pname = name;
		pvar = "";
	}

	ProgramAsset* pa = getOrCreateProgram(pname, pvar);
	if(pa != NULL)
	{
		myStateSet->setAttributeAndModes(pa->program, osg::StateAttribute::ON);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
ProgramAsset* Material::getOrCreateProgram(const String& name, const String& variant)
{
	String shaderRoot = "cyclops/common";
	String progName = name;

	String vertName = ostr("%1%/%2%.vert", %shaderRoot %name);

	//String vertName = ostr("%1%/%2%.vert", %shaderRoot %name);
	String fragName;
	// The @ character in the variant name is used to generate a new separate program variation using the same shaders.
	// This is useful, for instance, to decouple effects with different numbers of lights applied at the same time in 
	// the scene.
	if(variant != "" && variant[0] != '@')
	{
		// If the name already contains a slash do not use the default cyclops shader path: this allows the user to
		// select custom shaders.
		if(variant.find('/') != String::npos)
		{
			fragName = ostr("%1%.frag", %variant);
		}
		else
		{
			fragName = ostr("%1%/%2%-%3%.frag", %shaderRoot %name %variant);
		}

		progName = ostr("%1%-%2%", %name %variant);
	}
	else
	{
		fragName = ostr("%1%/%2%.frag", %shaderRoot %name);
		if(variant[0] == '@')
		{
			progName = ostr("%1%-%2%", %name %variant);
		}
	}

	return mySceneManager->getOrCreateProgram(progName, vertName, fragName);
}