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
#include "cyclops/EffectNode.h"
#include "cyclops/SceneManager.h"

#include <osg/Material>
#include <osgFX/Technique>
#include <osg/PolygonMode>

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
class Technique: public osgFX::Technique
{
public:
	Technique(const String& definition): myDefinition(definition)
	{}

    /// Validate.
    bool validate(osg::State&) const { return true; }
protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
	osg::Program* getProgram(const String& name, const String& variant = "")
	{
		String shaderRoot = "cyclops/common";
		String progName = name;

		String vertName = ostr("%1%/%2%.vert", %shaderRoot %name);
		String fragName;
		if(variant != "")
		{
			fragName = ostr("%1%/%2%-%3%.frag", %shaderRoot %name %variant);
			progName = ostr("%1%-%2%", %name %variant);
		}
		else
		{
			fragName = ostr("%1%/%2%.frag", %shaderRoot %name);
		}

		SceneManager* sm = SceneManager::instance();
		return sm->getProgram(name, vertName, fragName);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	virtual void define_passes()
	{
		if(StringUtils::startsWith(myDefinition, "colored")) define_passes_colored();
		else if(StringUtils::startsWith(myDefinition, "textured")) define_passes_textured();
		else if(StringUtils::startsWith(myDefinition, "bump")) define_passes_bump();
		else if(StringUtils::startsWith(myDefinition, "blueprint")) define_passes_blueprint();
		else
		{
			ofwarn("EffectNode: could not create effect with definition '%1%'", %myDefinition);
			addPass(new osg::StateSet());
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	void define_passes_colored()
	{
		String effectName;
		String diffuse;
		String variant;
		libconfig::ArgumentHelper ah;
		ah.newString("effectName", "the effect name", effectName);
		ah.newNamedString('d', "diffuse", "diffuse material", "diffuse material color", diffuse);
		ah.newNamedString('v', "variant", "shader variant", "fragment shader variant", variant);
		ah.process(myDefinition.c_str());

		SceneManager* sm = SceneManager::instance();
		osg::StateSet* ss = new osg::StateSet();
		osg::Program* prog = NULL;
		prog = getProgram("Colored", variant);

		ss->setAttributeAndModes(prog, osg::StateAttribute::ON);

		// If we have colors, add material attribute
		if(diffuse != "")
		{
			Color diffuseColor(diffuse);
			osg::Material* mat = new osg::Material();
			mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
			mat->setDiffuse(osg::Material::FRONT_AND_BACK, COLOR_TO_OSG(diffuseColor));
			mat->setAmbient(osg::Material::FRONT_AND_BACK, COLOR_TO_OSG(diffuseColor));
			mat->setEmission(osg::Material::FRONT_AND_BACK, COLOR_TO_OSG(Color::Black));
			mat->setSpecular(osg::Material::FRONT_AND_BACK, COLOR_TO_OSG(Color::Black));
			ss->setAttributeAndModes(mat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}

		addPass(ss);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	void define_passes_textured()
	{
		String effectName;
		String diffuse;
		String variant;
		libconfig::ArgumentHelper ah;
		ah.newString("effectName", "the effect name", effectName);
		ah.newNamedString('d', "diffuse", "diffuse texture", "diffuse texture file name", diffuse);
		ah.newNamedString('v', "variant", "shader variant", "fragment shader variant", variant);
		ah.process(myDefinition.c_str());

		SceneManager* sm = SceneManager::instance();
		osg::StateSet* ss = new osg::StateSet();
		osg::Program* prog = NULL;
		prog = getProgram("Textured", variant);
		ss->addUniform( new osg::Uniform("unif_ColorMap", 0) );
		ss->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(1, 1)));
		ss->setAttributeAndModes(prog, osg::StateAttribute::ON);

		if(diffuse != "")
		{
			osg::Texture2D* tex = sm->getTexture(diffuse);
			if(tex != NULL)
			{
				ss->setTextureAttribute(0, tex);
			}
		}

		addPass(ss);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	void define_passes_bump()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	void define_passes_blueprint()
	{
		String tempDef = myDefinition;
		myDefinition = "colored -d blue";
		define_passes_colored();
		myDefinition = tempDef;
		osg::StateSet* ss = new osg::StateSet();
		osg::PolygonMode* pm = new osg::PolygonMode();
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		ss->setAttribute(pm);
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		addPass(ss);
	}

private:
	String myDefinition;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EffectNode::EffectNode() 
{
	dirtyTechniques();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EffectNode::setDefinition(const String& definition)
{
	myDefinition = definition;
	dirtyTechniques();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EffectNode::define_techniques()
{
	Technique* tech = new Technique(myDefinition);
	this->addTechnique(tech);
	return true;
}
