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
    DefaultTechnique(osg::Material* mat)
        : Technique(), myMaterial(mat) {}

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
    void define_passes()
    {
		osg::StateSet* ss = new osg::StateSet();
        osg::PolygonMode* polymode = new osg::PolygonMode();
        polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		ss->setAttribute(polymode);
		addPass(ss);
    }

private:
	osg::ref_ptr<osg::Material> myMaterial;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
SolidEffect::SolidEffect(): 
	Effect()
{
	myMaterial = new osg::Material();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
SolidEffect::SolidEffect(const SolidEffect& copy, const osg::CopyOp& copyop)
:    Effect(copy, copyop)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool SolidEffect::define_techniques()
{
    addTechnique(new DefaultTechnique(myMaterial));
    return true;
}
