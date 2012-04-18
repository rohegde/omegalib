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

using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Technique: public osgFX::Technique
{
public:
	Technique(const String& definition): myDefinition(definition)
	{}

    /// Validate.
    bool validate(osg::State&) const { return true; }
protected:
	virtual void define_passes()
	{
		if(StringUtils::startsWith(myDefinition, "colored")) define_passes_colored();
		if(StringUtils::startsWith(myDefinition, "textured")) define_passes_textured();
		if(StringUtils::startsWith(myDefinition, "bump")) define_passes_bump();
	}

	void define_passes_colored()
	{
		String effectName;
		String diffuse;
		libconfig::ArgumentHelper ah;
		ah.newString("effectName", "the effect name", effectName);
		ah.newNamedString('d', "diffuse", "diffuse material", "diffuse material color color", diffuse);
		ah.process(myDefinition.c_str());



		SceneManager* sm = SceneManager::instance();
		osg::StateSet* ss = new osg::StateSet();
		osg::Program* prog = NULL;
		prog = sm->getProgram("colored", "cyclops/common/Colored.vert", "cyclops/common/Colored.frag");

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
			ss->setAttributeAndModes(mat, osg::StateAttribute::ON);
		}

		addPass(ss);
	}

	void define_passes_textured()
	{
	}

	void define_passes_bump()
	{
	}

private:
	String myDefinition;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EffectNode::EffectNode() 
{
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
