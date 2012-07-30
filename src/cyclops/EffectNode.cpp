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
#include<osg/BlendFunc>

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
	ProgramAsset* getProgram(const String& name, const String& variant = "", bool vertexShaderVariant = false)
	{
		String shaderRoot = "cyclops/common";
		String progName = name;

		//String vertName = ostr("%1%/%2%.vert", %shaderRoot %name);
		String vertName = ostr("%1%/%2%.vert", %shaderRoot %name);
		String fragName;
		if(variant != "")
		{
			fragName = ostr("%1%/%2%-%3%.frag", %shaderRoot %name %variant);
			progName = ostr("%1%-%2%", %name %variant);
			if(vertexShaderVariant)
			{
				vertName = ostr("%1%/%2%-%3%.vert", %shaderRoot %name %variant);
			}
		}
		else
		{
			fragName = ostr("%1%/%2%.frag", %shaderRoot %name);
		}

		SceneManager* sm = SceneManager::instance();
		return sm->getProgram(progName, vertName, fragName);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	virtual void define_passes()
	{
		Vector<String> passes = StringUtils::split(myDefinition, "|");

		for(auto iter = passes.begin(); iter != passes.end(); iter++)
		{
			StringUtils::trim(*iter);
			if(StringUtils::startsWith(*iter, "colored")) define_passes_colored(*iter);
			else if(StringUtils::startsWith(*iter, "textured")) define_passes_textured(*iter);
			else if(StringUtils::startsWith(*iter, "bump")) define_passes_bump(*iter);
			else
			{
				ofwarn("EffectNode: could not create effect with definition '%1%'", %myDefinition);
				addPass(new osg::StateSet());
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	void define_passes_colored(const String& def)
	{
		String effectName;
		String diffuse;
		double shininess = 10;
		double gloss = 0;
		bool transparent = false;
		bool vertexShaderVariant = false;
		bool additive = false;
		String variation = "";
		libconfig::ArgumentHelper ah;
		ah.newString("effectName", "the effect name", effectName);
		ah.newNamedString('d', "diffuse", "diffuse material", "diffuse material color", diffuse);
		ah.newNamedDouble('s', "shininess", "shininess", "specular power - defines size of specular highlights", shininess);
		ah.newNamedDouble('g', "gloss", "gloss", "gloss [0 - 1] - reflectivity of surface", gloss);
		ah.newNamedString('v', "variation", "variation", "effect variation", variation);
		ah.newFlag('V', "Vertex", "enable vertex shader variant", vertexShaderVariant);
		ah.newFlag('t', "transparent", "enable transparency for this effect", transparent);
		ah.newFlag('a', "additive", "enable additive blending for this effect", additive);
		ah.process(def.c_str());

		SceneManager* sm = SceneManager::instance();
		osg::StateSet* ss = new osg::StateSet();
		ProgramAsset* asset = getProgram("Colored", variation, vertexShaderVariant);
		if(asset != NULL)
		{
			ss->setAttributeAndModes(asset->program, osg::StateAttribute::ON);
			ss->addUniform( new osg::Uniform("unif_Shininess", (float)shininess) );
			ss->addUniform( new osg::Uniform("unif_Gloss", (float)gloss) );
			if(transparent)
			{
				ss->setMode(GL_BLEND, osg::StateAttribute::ON);
				ss->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF); 
				ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				if(additive)
				{
					osg::BlendFunc* bf = new osg::BlendFunc();
					bf->setFunction(GL_SRC_ALPHA, GL_ONE);
					ss->setAttribute(bf);
				}
			}
		}


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
	void define_passes_textured(const String& def)
	{
		String effectName;
		String diffuse;
		String variant;
		double shininess = 10;
		double gloss = 0;
		bool transparent = false;
		bool additive = false;
		bool vertexShaderVariant = false;
		libconfig::ArgumentHelper ah;
		ah.newString("effectName", "the effect name", effectName);
		ah.newNamedString('d', "diffuse", "diffuse texture", "diffuse texture file name", diffuse);
		ah.newNamedString('v', "variant", "shader variant", "fragment shader variant", variant);
		ah.newFlag('V', "Vertex", "enable vertex shader variant", vertexShaderVariant);
		ah.newNamedDouble('s', "shininess", "shininess", "specular power - defines size of specular highlights", shininess);
		ah.newNamedDouble('g', "gloss", "gloss", "gloss [0 - 1] - reflectivity of surface", gloss);
		ah.newFlag('t', "transparent", "enable transparency for this effect", transparent);
		ah.newFlag('a', "additive", "enable additive blending for this effect", additive);
		ah.process(def.c_str());

		SceneManager* sm = SceneManager::instance();
		osg::StateSet* ss = new osg::StateSet();
		osg::Program* prog = NULL;
		ProgramAsset* asset = getProgram("Textured", variant, vertexShaderVariant);
		if(asset != NULL)
		{
			ss->setAttributeAndModes(asset->program, osg::StateAttribute::ON);
			ss->addUniform( new osg::Uniform("unif_DiffuseMap", 0) );
			ss->addUniform(new osg::Uniform("unif_TextureTiling", osg::Vec2(1, 1)));
			ss->addUniform( new osg::Uniform("unif_Shininess", (float)shininess) );
			ss->addUniform( new osg::Uniform("unif_Gloss", (float)gloss) );

			ss->setAttributeAndModes(asset->program, osg::StateAttribute::ON);

			if(transparent)
			{
				ss->setMode(GL_BLEND, osg::StateAttribute::ON);
				if(additive)
				{
					
				}
			}
		}

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
	void define_passes_bump(const String& def)
	{
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
