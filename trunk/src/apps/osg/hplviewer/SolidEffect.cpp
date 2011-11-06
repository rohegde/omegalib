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
#include "SolidEffect.h"

#include <osgFX/Registry>

#include <osg/PolygonOffset>
#include <osg/Texture1D>
#include <osg/VertexProgram>
#include <osg/PolygonMode>
#include <osg/CullFace>
#include <osg/Image>
#include <osg/TexEnv>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/Program>
#include <osg/Shader>

#include <sstream>

using namespace osgFX;
using namespace hpl;

///////////////////////////////////////////////////////////////////////////////////////////////////
class DefaultTechnique : public Technique 
{
public:
    DefaultTechnique(SolidEffect* fx)
        : Technique(), myFx(fx) {}

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
    void define_passes()
    {
		osg::StateSet* ss = new osg::StateSet();
        //osg::PolygonMode* polymode = new osg::PolygonMode();
        //polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		//ss->setAttribute(polymode);

		osg::Program* prog = myFx->getProgram();
		prog->addBindAttribLocation ("Tangent", 6);

		ss->setAttributeAndModes(prog, osg::StateAttribute::ON);

		ss->addUniform( new osg::Uniform("colorMap", 0) );
		ss->setTextureAttribute(0, myFx->getDiffuseTexture());

		ss->addUniform( new osg::Uniform("normalMap", 1) );
		ss->setTextureAttribute(1, myFx->getNMapTexture());

		ss->addUniform( new osg::Uniform("heightMap", 2) );
		ss->setTextureAttribute(2, myFx->getHeightTexture());

		addPass(ss);
    }

private:
	SolidEffect* myFx;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
SolidEffect::SolidEffect(): 
	Effect()
{
	mySceneManager = SceneManager::getInstance();
	myMaterial = new osg::Material();

	myDiffuse = NULL;
	myNMap = NULL;
	mySpecular = NULL;
	myHeight = NULL;

	myProgram = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SolidEffect::SolidEffect(const SolidEffect& copy, const osg::CopyOp& copyop):
	Effect(copy, copyop)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void SolidEffect::load(TiXmlElement* xdata)
{
	TiXmlElement* xtextures = xdata->FirstChildElement("TextureUnits");
	TiXmlElement* xchild = xtextures->FirstChildElement();
	while(xchild)
	{
		String type = xchild->Value();
		String file = xchild->Attribute("File");

		if(type == "Diffuse")
		{
			myDiffuse = mySceneManager->getTexture(file);
		}
		else if(type == "NMap")
		{
			myNMap = mySceneManager->getTexture(file);
		}
		else if(type == "Specular")
		{
			mySpecular = mySceneManager->getTexture(file);
		}
		else if(type == "Height")
		{
			myHeight = mySceneManager->getTexture(file);
		}
		xchild = xchild->NextSiblingElement();
	}

	myProgram = mySceneManager->getProgram("solid", "shaders/parallaxv4.vert", "shaders/parallaxv4.frag");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SolidEffect::define_techniques()
{
    addTechnique(new DefaultTechnique(this));
    return true;
}
